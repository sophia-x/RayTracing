#include <unordered_set>

#include "KD_Tree.hpp"

Node::Node(const AABB &box, const vector<const Primitive *> &primitives, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, size_t depth): __left_ptr(0), __right_ptr(0) {
	size_t size = primitives.size();
	__box = box;

	if (size == 0)
		return;

	bool leaf = true;
	if (size > SIZE && depth < MAX_DEPTH) {
		const vec3 &box_size = __box.getSize();

		__axis = 0;
		if (box_size[1] >= box_size[0] && box_size[1] >= box_size[2])
			__axis = 1;
		if (box_size[2] >= box_size[0] && box_size[2] >= box_size[1])
			__axis = 2;

		int left, right;
		float cost = splitCost(primitives, min_ps, max_ps, left, right);
		if (cost < size * COST_INTERSECT) {
			leaf = false;

			vector<const vec3 *> left_min_ps(left), right_min_ps(right), left_max_ps(left), right_max_ps(right);
			vector<const Primitive *> left_primitives(left), right_primitives(right);
			int left_p = 0, right_p = 0;

			AABB left_box, right_box;
			splitBox(left_box, right_box, __split_point);
			for (size_t i = 0; i < size; i ++) {
				if (primitives[i]->intersect(left_box)) {
					left_min_ps[left_p] = min_ps[i];
					left_max_ps[left_p] = max_ps[i];
					left_primitives[left_p] = primitives[i];
					left_p ++;
				}

				if (primitives[i]->intersect(right_box)) {
					right_min_ps[right_p] = min_ps[i];
					right_max_ps[right_p] = max_ps[i];
					right_primitives[right_p] = primitives[i];
					right_p ++;
				}
			}
			__left_ptr = new Node(left_box, left_primitives, left_min_ps, left_max_ps, depth + 1);
			__right_ptr = new Node(right_box, right_primitives, right_min_ps, right_max_ps, depth + 1);
		}
	}

	if (leaf) {
		__primitives.reserve(size);
		for (size_t i = 0; i < size; i ++) {
			__primitives.push_back(primitives[i]);
		}
	}
}

bool Node::intersect(const Ray &ray, float &t, Primitive const* &hit_ptr) const {
	float t_near, t_far;
	if (!__box.intersect(ray, t_near, t_far)) {
		return false;
	}

	float tmp_t;
	Primitive const* tmp_ptr;

	if (__left_ptr != 0) {
		Node *near_ptr = 0;
		Node *far_ptr = 0;

		nearPtr(ray, near_ptr, far_ptr, t_near, t_far);

		if (near_ptr == 0)
			return false;

		t = numeric_limits<float>::max();
		bool hit = near_ptr->intersect(ray, tmp_t, tmp_ptr);

		if (hit) {
			t = tmp_t;
			hit_ptr = tmp_ptr;
		}

		if (far_ptr == 0)
			return hit;

		if (far_ptr->intersect(ray, tmp_t, tmp_ptr)) {
			if (tmp_t < t) {
				t = tmp_t;
				hit_ptr = tmp_ptr;
			}
			return true;
		}

		return hit;
	}

	t = numeric_limits<float>::max();
	for (size_t i = 0; i < __primitives.size(); i++) {
		if (!__primitives[i]->intersect(ray, tmp_t))
			continue;
		if (tmp_t < t) {
			t = tmp_t;
			hit_ptr = __primitives[i];
		}
	}

	return t != numeric_limits<float>::max();
}

bool Node::intersect(const Ray &ray, float len, unsigned long hash_code) const {
	float t_near, t_far;
	if (!__box.intersect(ray, t_near, t_far))
		return false;
	if (t_near >= len)
		return false;

	if (__left_ptr != 0) {
		Node *near_ptr = 0;
		Node *far_ptr = 0;

		nearPtr(ray, near_ptr, far_ptr, t_near, t_far);

		if (near_ptr != 0 && near_ptr->intersect(ray, len, hash_code))
			return true;
		if (far_ptr != 0 && far_ptr->intersect(ray, len, hash_code))
			return true;
		return false;
	}

	for (size_t i = 0; i < __primitives.size(); i++) {
		if (__primitives[i]->getHashCode() == hash_code)
			continue;

		if (__primitives[i]->intersect_seg(ray, len)) {
			return true;
		}
	}
	return false;
}

inline void Node::nearPtr(const Ray &ray, Node* &near_ptr, Node* &far_ptr, float t_near, float t_far) const {
	const vec3 &position = ray.getPosition();
	const vec3 &direction = ray.getDirection();

	float entry_p = position[__axis] + t_near * direction[__axis];
	float exit_p = position[__axis] + t_far * direction[__axis];

	if (entry_p >= __split_point && exit_p >= __split_point) {
		near_ptr = __right_ptr;
		return;
	}
	if (exit_p <= __split_point && entry_p <= __split_point) {
		near_ptr = __left_ptr;
		return;
	}
	if (entry_p < __split_point && exit_p > __split_point) {
		near_ptr = __left_ptr;
		far_ptr = __right_ptr;
		return;
	}
	if (entry_p > __split_point && exit_p <= __split_point) {
		near_ptr = __right_ptr;
		far_ptr = __left_ptr;
		return;
	}
}

inline float Node::splitCost(const vector<const Primitive *> &primitives, const vector<const vec3 *> &min_ps, const vector<const vec3 *> &max_ps, int &left, int &right) {
	unordered_set<float> splits;
	for (size_t i = 0; i < min_ps.size(); i++) {
		splits.insert((*min_ps[i])[__axis]);
		splits.insert((*max_ps[i])[__axis]);
	}

	float min_cost = numeric_limits<float>::max(); int tmp_left, tmp_right;
	for (auto it = splits.begin(); it != splits.end(); ++it ) {
		float cost = calculateCost(primitives, *it, tmp_left, tmp_right);
		if (cost < min_cost) {
			min_cost = cost;
			__split_point = *it;
			left = tmp_left;
			right = tmp_right;
		}
	}

	return min_cost;
}

inline float Node::calculateCost(const vector<const Primitive *> &primitives, float split_point, int &left, int &right) const {
	AABB left_box, right_box;
	splitBox(left_box, right_box, split_point);

	left = 0, right = 0;
	for (size_t i = 0; i < primitives.size(); i ++) {
		if (primitives[i]->intersect(left_box))
			left ++;
		if (primitives[i]->intersect(right_box))
			right ++;
	}

	return COST_TRAVEL + COST_INTERSECT * (left_box.area() * left + right_box.area() * right) / __box.area();
}

inline void Node::splitBox(AABB &left_box, AABB &right_box, float split_point) const {
	const vec3 &box_min_pos = __box.getMinP();
	const vec3 &box_max_pos = __box.getMaxP();
	vec3 left_max_pos = box_max_pos; left_max_pos[__axis] = split_point;
	vec3 right_min_pos = box_min_pos; right_min_pos[__axis] = split_point;

	left_box = AABB((left_max_pos + box_min_pos) / 2.0f, left_max_pos - box_min_pos);
	right_box = AABB((box_max_pos + right_min_pos) / 2.0f, box_max_pos - right_min_pos);
}