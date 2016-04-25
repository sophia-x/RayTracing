#ifndef BOUNDING_BOX
#define BOUNDING_BOX

#include <unordered_set>
#include <memory>

#include "Box.hpp"
#include "../common.hpp"

static const size_t MAX_DEPTH = 20;
static const size_t SIZE = 1;
static const float COST_INTERSECT = 1.0f;
static const float COST_TRAVEL = 0.3f;

template <class T>
class Node {
private:
	Node *left_ptr, *right_ptr;

	vector<const T *> model_vec;
	Box box;

public:
	Node(const vector<const T *> &models, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, size_t depth): left_ptr(0), right_ptr(0) {
		size_t size = models.size();

		vec3 min_p(numeric_limits<float>::max()), max_p(numeric_limits<float>::min());
		for (size_t i = 0; i < size; i ++) {
			min_p = glm::min(min_p, *min_ps[i]);
			max_p = glm::max(max_p, *max_ps[i]);
		}
		vec3 box_size = max_p - min_p;
		box = Box((min_p + max_p) / 2.0f, box_size);

		bool leaf = true;
		if (size > SIZE && depth < MAX_DEPTH) {
			int axis = 0;
			if (box_size[1] >= box_size[0] && box_size[1] >= box_size[2])
				axis = 1;
			if (box_size[2] >= box_size[0] && box_size[2] >= box_size[1])
				axis = 2;

			float split_point;
			int left, right;

			float score = split(models, min_ps, max_ps, axis, split_point, left, right);

			if (score < size * COST_INTERSECT) {
				leaf = false;

				vector<const vec3 *> left_min_ps(left), right_min_ps(right), left_max_ps(left), right_max_ps(right);
				vector<const T *> left_models(left), right_models(right);
				int left_p = 0, right_p = 0;

				Box left_box, right_box;
				splitBox(left_box, right_box, split_point, axis);

				for (size_t i = 0; i < size; i ++) {
					if (models[i]->intersectBox(left_box)) {
						left_min_ps[left_p] = min_ps[i];
						left_max_ps[left_p] = max_ps[i];
						left_models[left_p] = models[i];
						left_p ++;
					}

					if (models[i]->intersectBox(right_box)) {
						right_min_ps[right_p] = min_ps[i];
						right_max_ps[right_p] = max_ps[i];
						right_models[right_p] = models[i];
						right_p ++;
					}
				}

				left_ptr = new Node(left_models, left_min_ps, left_max_ps, depth + 1);
				right_ptr = new Node(right_models, right_min_ps, right_max_ps, depth + 1);
			}
		}

		if (leaf) {
			model_vec.reserve(size);
			for (size_t i = 0; i < size; i ++) {
				model_vec.push_back(models[i]);
			}
		}
	}

	~Node() {
		if (left_ptr != 0) {
			delete left_ptr;
			delete right_ptr;
		}
	}

	inline bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
		int idx;
		if (!box.intersect(position, direction, t, idx))
			return false;

		if (left_ptr != 0) {
			float min_t = numeric_limits<float>::max();
			vec3 tmp_n; vec3 tmp_color;
			BasicModel const* tmp_model;
			bool hit = false;
			if (left_ptr->intersect(position, direction, t, tmp_n, tmp_color, tmp_model)) {
				hit = true;
				min_t = t;
				hit_normal = tmp_n;
				hit_surface_color = tmp_color;
				hit_model = tmp_model;
			}

			if (right_ptr->intersect(position, direction, t, tmp_n, tmp_color, tmp_model)) {
				hit = true;
				if (t < min_t) {
					min_t = t;
					hit_normal = tmp_n;
					hit_surface_color = tmp_color;
					hit_model = tmp_model;
				}
			}

			return hit;
		}

		float min_t = numeric_limits<float>::max();
		vec3 n; vec3 color;
		BasicModel const* tmp_model;
		for (size_t i = 0; i < model_vec.size(); i++) {
			if (!model_vec[i]->intersect(position, direction, t, n, color, tmp_model))
				continue;
			if (t < min_t) {
				min_t = t;
				hit_normal = n;
				hit_surface_color = color;
				hit_model = tmp_model;
			}
		}

		if (min_t == numeric_limits<float>::max())
			return false;

		t = min_t;
		return true;
	}

	inline const Box& getBox() const {
		return box;
	}
private:
	inline float split(const vector<const T *> &models, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, int axis, float & split_point, int &left, int &right) const {
		unordered_set<float> splits;
		for (size_t i = 0; i < min_ps.size(); i++) {
			splits.insert((*min_ps[i])[axis]);
			splits.insert((*max_ps[i])[axis]);
		}

		float best_score = -1; int tmp_left, tmp_right;
		for (auto it = splits.begin(); it != splits.end(); ++it ) {
			float score = calculateScore(models, min_ps, max_ps, axis, *it, tmp_left, tmp_right);
			if (score > best_score) {
				best_score = score;
				split_point = *it;
				left = tmp_left;
				right = tmp_right;
			}
		}

		return best_score;
	}

	inline float calculateScore(const vector<const T *> &models, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, int axis, float split_point, int &left, int &right) const {
		Box left_box, right_box;
		splitBox(left_box, right_box, split_point, axis);

		left = 0, right = 0;
		for (int i = 0; i < models.size(); i ++) {
			if (models[i]->intersectBox(left_box))
				left ++;
			if (models[i]->intersectBox(right_box))
				right ++;
		}

		return COST_TRAVEL + COST_INTERSECT * (left_box.area() * left + right_box.area() * right) / box.area();
	}

	inline void splitBox(Box &left_box, Box &right_box, float split_point, int axis)const {
		vec3 box_min_pos = box.getMinPs();
		vec3 box_max_pos = box.getMaxPs();
		vec3 left_max_pos = box_max_pos; left_max_pos[axis] = split_point;
		vec3 right_min_pos = box_min_pos; right_min_pos[axis] = split_point;

		left_box = Box((left_max_pos + box_min_pos) / 2.0f, left_max_pos - box_min_pos);
		right_box = Box((box_max_pos + right_min_pos) / 2.0f, box_max_pos - right_min_pos);
	}
};

template <class T>
class BoundingBox {
private:
	shared_ptr<Node<T> > root;

public:
	BoundingBox(): root() {}

	BoundingBox(vector<T *> &models) {
		size_t size = models.size();
		vector<const T *> model_ptrs(size);
		vector<vec3> min_ps(size); vector<vec3> max_ps(size);
		vector<const vec3 *> min_ptrs(size); vector<const vec3 *> max_ptrs(size);

		for (size_t i = 0; i < size; i ++) {
			model_ptrs[i] = models[i];
			min_ps[i] = models[i]->getMinPs();
			max_ps[i] = models[i]->getMaxPs();

			min_ptrs[i] = &min_ps[i];
			max_ptrs[i] = &max_ps[i];
		}

		root = shared_ptr<Node<T> >(new Node<T>(model_ptrs, min_ptrs, max_ptrs, 0));
	}

	inline bool intersect(const vec3 &position, const vec3 &direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
		return root->intersect(position, direction, t, hit_normal, hit_surface_color, hit_model);
	}

	inline bool intersectBox(const Box &box) const {
		return root->getBox().intersectBox(box);
	}

	inline vec3 getMinPs() const {
		return root->getBox().getMinPs();
	}

	inline vec3 getMaxPs() const {
		return root->getBox().getMaxPs();
	}
};

#endif