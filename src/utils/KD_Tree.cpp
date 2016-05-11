#include <unordered_set>
#include <thread>
#include <algorithm>

#include "KD_Tree.hpp"

void thread_node(Node **node_ptr, const AABB &box, const vector<size_t> &indexes, const vector<Primitive *> &primitives, const vector<vec3> &min_ps, const vector<vec3> &max_ps, size_t depth) {
	(*node_ptr) = new Node(box, indexes, primitives, min_ps, max_ps, depth);
}

Node::Node(const AABB &box, const vector<size_t> &indexes, const vector<Primitive *> &primitives, const vector<vec3> &min_ps, const vector<vec3> &max_ps, size_t depth): __left_ptr(0), __right_ptr(0) {
	size_t size = indexes.size();
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
		float cost = splitCost(indexes, primitives, min_ps, max_ps, left, right);
		if (cost < size * COST_INTERSECT) {
			leaf = false;

			vector<size_t> left_indexes(left), right_indexes(right);
			int left_p = 0, right_p = 0;

			AABB left_box, right_box;
			splitBox(left_box, right_box, __split_point);
			for (size_t idx = 0; idx < size; idx ++) {
				size_t i = indexes[idx];
				if (min_ps[i][__axis] <= __split_point && primitives[i]->intersect(left_box)) {
					left_indexes[left_p] = i;
					left_p ++;
				}

				if (max_ps[i][__axis] >= __split_point && primitives[i]->intersect(right_box)) {
					right_indexes[right_p] = i;
					right_p ++;
				}
			}

			thread l_thread(thread_node, &__left_ptr, left_box, left_indexes, primitives, min_ps, max_ps, depth + 1);
			thread r_thread(thread_node, &__right_ptr, right_box, right_indexes, primitives, min_ps, max_ps, depth + 1);
			l_thread.join();
			r_thread.join();
		}
	}

	if (leaf) {
		__primitives.reserve(size);
		for (size_t idx = 0; idx < size; idx ++) {
			size_t i = indexes[idx];
			__primitives.push_back(primitives[i]);
		}
	}
}

bool Node::intersect(const Ray &ray, float &t, Primitive * &hit_ptr) const {
	float t_near, t_far;
	if (!__box.intersect(ray, t_near, t_far)) {
		return false;
	}

	float tmp_t;
	Primitive * tmp_ptr;

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

inline float Node::splitCost(const vector<size_t> &indexes, const vector<Primitive *> &primitives, const vector<vec3> &min_ps, const vector<vec3> &max_ps, int &left, int &right) {
	unordered_set<float> splits_set;
	for (size_t idx = 0; idx < indexes.size(); idx++) {
		size_t i = indexes[idx];
		splits_set.insert(min_ps[i][__axis]);
		splits_set.insert(max_ps[i][__axis]);
	}
	vector<float> splits(splits_set.begin(), splits_set.end());
	sort(splits.begin(), splits.end());

	vector<bool> is_lefts(indexes.size(), false);

	float min_cost = numeric_limits<float>::max(); int tmp_left, tmp_right;
	for (auto it = splits.begin(); it != splits.end(); ++it ) {
		float cost = calculateCost(indexes, is_lefts, primitives, *it, tmp_left, tmp_right, min_ps, max_ps);
		if (cost < min_cost) {
			min_cost = cost;
			__split_point = *it;
			left = tmp_left;
			right = tmp_right;
		}
	}

	return min_cost;
}

inline float Node::calculateCost(const vector<size_t> &indexes, vector<bool> &is_lefts, const vector<Primitive *> &primitives, float split_point, int &left, int &right, const vector<vec3> &min_ps, const vector<vec3> &max_ps) const {
	AABB left_box, right_box;
	splitBox(left_box, right_box, split_point);

	left = 0, right = 0;
	for (size_t idx = 0; idx < indexes.size(); idx ++) {
		size_t i = indexes[idx];
		if (is_lefts[idx] || (min_ps[i][__axis] <= split_point && primitives[i]->intersect(left_box))) {
			left ++;
			if (!is_lefts[idx])
				is_lefts[idx] = true;
		}
		if (max_ps[i][__axis] >= split_point && primitives[i]->intersect(right_box))
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