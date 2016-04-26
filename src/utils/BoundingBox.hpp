#ifndef BOUNDING_BOX
#define BOUNDING_BOX

#include <unordered_set>
#include <memory>

#include "AABB.hpp"
#include "../common.hpp"

static const size_t MAX_DEPTH = 20;
static const size_t SIZE = 5;
static const float COST_INTERSECT = 1.0f;
static const float COST_TRAVEL = 0.3f;

template <class T>
class Node {
private:
	Node *left_ptr, *right_ptr;

	vector<const T *> model_vec;
	AABB box;
	float split_point;
	float axis;

public:
	Node(const AABB &aabb, const vector<const T *> &models, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, size_t depth): left_ptr(0), right_ptr(0) {
		size_t size = models.size();
		box = aabb;
		if (size == 0)
			return;

		vec3 box_size = box.getSize();
		bool leaf = true;
		if (size > SIZE && depth < MAX_DEPTH) {
			axis = 0;
			if (box_size[1] >= box_size[0] && box_size[1] >= box_size[2])
				axis = 1;
			if (box_size[2] >= box_size[0] && box_size[2] >= box_size[1])
				axis = 2;

			int left, right;
			float score = split(models, min_ps, max_ps, axis, split_point, left, right);

			if (score < size * COST_INTERSECT) {
				leaf = false;

				vector<const vec3 *> left_min_ps(left), right_min_ps(right), left_max_ps(left), right_max_ps(right);
				vector<const T *> left_models(left), right_models(right);
				int left_p = 0, right_p = 0;

				AABB left_box, right_box;
				splitBox(left_box, right_box, split_point, axis);

				for (size_t i = 0; i < size; i ++) {
					if (models[i]->intersect(left_box)) {
						left_min_ps[left_p] = min_ps[i];
						left_max_ps[left_p] = max_ps[i];
						left_models[left_p] = models[i];
						left_p ++;
					}

					if (models[i]->intersect(right_box)) {
						right_min_ps[right_p] = min_ps[i];
						right_max_ps[right_p] = max_ps[i];
						right_models[right_p] = models[i];
						right_p ++;
					}
				}
				left_ptr = new Node(left_box, left_models, left_min_ps, left_max_ps, depth + 1);
				right_ptr = new Node(right_box, right_models, right_min_ps, right_max_ps, depth + 1);
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

	inline bool intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
		float t_near, t_far;
		if (!box.intersect(position, direction, inv_direction, t_near, t_far))
			return false;

		float min_t = numeric_limits<float>::max(); vec3 tmp_n, tmp_color;
		BasicModel const* tmp_model;
		if (left_ptr != 0) {
			Node *near_ptr = 0;
			Node *far_ptr = 0;

			float entry_p = position[axis] + t_near * direction[axis];
			float exit_p = position[axis] + t_far * direction[axis];

			if (entry_p >= split_point && exit_p >= split_point) {
				near_ptr = right_ptr;
			}
			if (exit_p <= split_point && entry_p <= split_point) {
				near_ptr = left_ptr;
			}
			if (entry_p < split_point && exit_p > split_point) {
				near_ptr = left_ptr;
				far_ptr = right_ptr;
			}
			if (entry_p > split_point && exit_p <= split_point) {
				near_ptr = right_ptr;
				far_ptr = left_ptr;
			}

			bool hit = near_ptr->intersect(position, direction, inv_direction, t, tmp_n, tmp_color, tmp_model);
			if (hit) {
				min_t = t;
				hit_normal = tmp_n;
				hit_surface_color = tmp_color;
				hit_model = tmp_model;
			}
			if (far_ptr == 0)
				return hit;

			if (far_ptr->intersect(position, direction, inv_direction, t, tmp_n, tmp_color, tmp_model)) {
				if (t < min_t) {
					min_t = t;
					hit_normal = tmp_n;
					hit_surface_color = tmp_color;
					hit_model = tmp_model;
				}
				t = min_t;
				return true;
			} else {
				t = min_t;
				return hit;
			}
		}

		min_t = numeric_limits<float>::max();
		for (size_t i = 0; i < model_vec.size(); i++) {
			if (!model_vec[i]->intersect(position, direction, inv_direction, t, tmp_n, tmp_color, tmp_model))
				continue;
			if (t < min_t) {
				min_t = t;
				hit_normal = tmp_n;
				hit_surface_color = tmp_color;
				hit_model = tmp_model;
			}
		}
		if (min_t == numeric_limits<float>::max())
			return false;
		t = min_t;
		return true;
	}

	inline bool intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float len) const {
		float t_near, t_far;
		if (!box.intersect(position, direction, inv_direction, t_near, t_far))
			return false;
		if (t_near >= len)
			return false;

		if (left_ptr != 0) {
			float entry_p = position[axis] + t_near * direction[axis];
			float exit_p = position[axis] + t_far * direction[axis];

			Node *near_ptr = 0;
			Node *far_ptr = 0;

			if (entry_p >= split_point && exit_p >= split_point) {
				near_ptr = right_ptr;
			}
			if (exit_p <= split_point && entry_p <= split_point) {
				near_ptr = left_ptr;
			}
			if (entry_p < split_point && exit_p > split_point) {
				near_ptr = left_ptr;
				far_ptr = right_ptr;
			}
			if (entry_p > split_point && exit_p <= split_point) {
				near_ptr = right_ptr;
				far_ptr = left_ptr;
			}

			if (near_ptr->intersect(position, direction, inv_direction, len))
				return true;
			if (far_ptr != 0 && far_ptr->intersect(position, direction, inv_direction, len))
				return true;
			return false;
		}

		for (size_t i = 0; i < model_vec.size(); i++) {
			if (model_vec[i]->intersect(position, direction, inv_direction, len))
				return true;
		}
		return false;
	}

	inline const AABB& getBox() const {
		return box;
	}

private:
	inline float split(const vector<const T *> &models, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, int axis, float & __split_point, int &left, int &right) const {
		unordered_set<float> splits;
		for (size_t i = 0; i < min_ps.size(); i++) {
			splits.insert((*min_ps[i])[axis]);
			splits.insert((*max_ps[i])[axis]);
		}

		float best_score = numeric_limits<float>::max(); int tmp_left, tmp_right;
		for (auto it = splits.begin(); it != splits.end(); ++it ) {
			float score = calculateScore(models, min_ps, max_ps, axis, *it, tmp_left, tmp_right);
			if (score < best_score) {
				best_score = score;
				__split_point = *it;
				left = tmp_left;
				right = tmp_right;
			}
		}

		return best_score;
	}

	inline float calculateScore(const vector<const T *> &models, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, int axis, float __split_point, int &left, int &right) const {
		AABB left_box, right_box;
		splitBox(left_box, right_box, __split_point, axis);

		left = 0, right = 0;
		for (int i = 0; i < models.size(); i ++) {
			if (models[i]->intersect(left_box))
				left ++;
			if (models[i]->intersect(right_box))
				right ++;
		}

		return COST_TRAVEL + COST_INTERSECT * (left_box.area() * left + right_box.area() * right) / box.area();
	}

	inline void splitBox(AABB &left_box, AABB &right_box, float __split_point, int axis) const {
		const vec3 &box_min_pos = box.getMinPs();
		const vec3 &box_max_pos = box.getMaxPs();
		vec3 left_max_pos = box_max_pos; left_max_pos[axis] = __split_point;
		vec3 right_min_pos = box_min_pos; right_min_pos[axis] = __split_point;

		left_box = AABB((left_max_pos + box_min_pos) / 2.0f, left_max_pos - box_min_pos);
		right_box = AABB((box_max_pos + right_min_pos) / 2.0f, box_max_pos - right_min_pos);
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

		vec3 min_p(numeric_limits<float>::max()), max_p(numeric_limits<float>::min());
		for (size_t i = 0; i < size; i ++) {
			min_p = glm::min(min_p, min_ps[i]);
			max_p = glm::max(max_p, max_ps[i]);
		}
		vec3 box_size = max_p - min_p;
		AABB box = AABB((min_p + max_p) / 2.0f, box_size);

		root = shared_ptr<Node<T> >(new Node<T>(box, model_ptrs, min_ptrs, max_ptrs, 0));
	}

	inline bool intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float &t, vec3 &hit_normal, vec3 &hit_surface_color, BasicModel const* &hit_model) const {
		return root->intersect(position, direction, inv_direction, t, hit_normal, hit_surface_color, hit_model);
	}

	inline bool intersect(const vec3 &position, const vec3 &direction, const vec3 &inv_direction, float len) const {
		return root->intersect(position, direction, inv_direction, len);
	}

	inline bool intersect(const AABB &box) const {
		return root->getBox().intersect(box);
	}

	inline vec3 getMinPs() const {
		return root->getBox().getMinPs();
	}

	inline vec3 getMaxPs() const {
		return root->getBox().getMaxPs();
	}
};

#endif