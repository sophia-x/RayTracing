#ifndef BOUNDING_BOX
#define BOUNDING_BOX

#include <limits>
#include <algorithm>
#include <memory>

#include "Box.hpp"
#include <iostream>
using namespace std;

template <class T>
class Node {
private:
	Node *left_ptr, *right_ptr;

	vector<const T *> model_vec;
	Box box;

public:
	Node(const vector<const T *> &models, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, size_t threshold): left_ptr(0), right_ptr(0) {
		size_t size = models.size();
		vec3 min_p(numeric_limits<float>::max()), max_p(numeric_limits<float>::min());
		for (size_t i = 0; i < size; i ++) {
			min_p = glm::min(min_p, *min_ps[i]);
			max_p = glm::max(max_p, *max_ps[i]);
		}
		box = Box((min_p + max_p) / 2.0f, max_p - min_p);

		if (size <= threshold) {
			model_vec.reserve(size);
			for (size_t i = 0; i < size; i ++) {
				model_vec.push_back(models[i]);
			}
		} else {
			int min_diff = numeric_limits<int>::max();
			int axis = -1; float split_point;
			int left, right;
			for (int i = 0; i < 3; i ++) {
				float sp; int tmp_left, tmp_right;
				int diff = split(min_ps, max_ps, i, sp, tmp_left, tmp_right);
				if (diff < min_diff) {
					min_diff = diff;
					axis = i;
					split_point = sp;
					left = tmp_left;
					right = tmp_right;
				}
			}

			vector<const vec3 *> left_min_ps(left), right_min_ps(right), left_max_ps(left), right_max_ps(right);
			vector<const T *> left_models(left), right_models(right);
			int left_p = 0, right_p = 0;

			for (size_t i = 0; i < size; i ++) {
				if ((*min_ps[i])[axis] < split_point) {
					left_min_ps[left_p] = min_ps[i];
					left_max_ps[left_p] = max_ps[i];
					left_models[left_p] = models[i];
					left_p ++;
				}

				if ((*max_ps[i])[axis] > split_point) {
					right_min_ps[right_p] = min_ps[i];
					right_max_ps[right_p] = max_ps[i];
					right_models[right_p] = models[i];
					right_p ++;
				}
			}

			left_ptr = new Node(left_models, left_min_ps, left_max_ps, threshold);
			right_ptr = new Node(right_models, right_min_ps, right_max_ps, threshold);
		}
	}

	~Node() {
		if (left_ptr != 0) {
			delete left_ptr;
			delete right_ptr;
		}
	}

	inline bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
		int idx;
		if (!box.intersect(position, direction, t, idx))
			return false;

		if (left_ptr != 0) {
			float min_t = numeric_limits<float>::max();
			vec4 tmp_n; vec3 tmp_color;
			bool hit = false;
			if (left_ptr->intersect(position, direction, t, tmp_n, tmp_color)) {
				hit = true;
				min_t = t;
				hit_normal = tmp_n;
				hit_surface_color = tmp_color;
			}

			if (right_ptr->intersect(position, direction, t, tmp_n, tmp_color)) {
				hit = true;
				if (t < min_t) {
					min_t = t;
					hit_normal = tmp_n;
					hit_surface_color = tmp_color;
				}
			}

			return hit;
		}

		float min_t = numeric_limits<float>::max();
		vec4 n; vec3 color;
		for (size_t i = 0; i < model_vec.size(); i++) {
			if (!model_vec[i]->intersect(position, direction, t, n, color))
				continue;
			if (t < min_t) {
				min_t = t;
				hit_normal = n;
				hit_surface_color = color;
			}
		}

		if (min_t == numeric_limits<float>::max())
			return false;

		t = min_t;
		return true;
	}

private:
	inline int split(const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, int axis, float & split_point, int &left, int &right) const {
		size_t size = min_ps.size();
		vector<float> centers(size);
		for (size_t i = 0; i < size; i ++)
			centers[i] = ((*max_ps[i])[axis] + (*min_ps[i])[axis]) / 2.0f;

		nth_element(centers.begin(), centers.begin() + size / 2, centers.end());
		split_point = centers[size / 2];

		left = right = 0;
		for (size_t i = 0; i < size; i ++) {
			if ((*min_ps[i])[axis] < split_point)
				left ++;
			if ((*max_ps[i])[axis] > split_point)
				right ++;
		}

		return abs(right - left);
	}
};

template <class T>
class BoundingBox {
private:
	shared_ptr<Node<T> > root;

public:
	BoundingBox(): root() {}

	BoundingBox(vector<T> &models, size_t threshold) {
		size_t size = models.size();
		vector<const T *> model_ptrs(size);
		vector<vec3> min_ps(size); vector<vec3> max_ps(size);
		vector<const vec3 *> min_ptrs(size); vector<const vec3 *> max_ptrs(size);

		for (size_t i = 0; i < size; i ++) {
			model_ptrs[i] = &models[i];
			min_ps[i] = models[i].getMinPs();
			max_ps[i] = models[i].getMaxPs();

			min_ptrs[i] = &min_ps[i];
			max_ptrs[i] = &max_ps[i];
		}

		root = shared_ptr<Node<T> >(new Node<T>(model_ptrs, min_ptrs, max_ptrs, threshold));
	}

	inline bool intersect(const vec4 &position, const vec4 &direction, float &t, vec4 &hit_normal, vec3 &hit_surface_color) const {
		return root->intersect(position, direction, t, hit_normal, hit_surface_color);
	}
};

#endif