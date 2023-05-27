#include "roundfunction.h"

bool RoundFunction::advance_state() {
    while (curr_box_idx > 0 && boxes[curr_box_idx]->is_determined()) {
        boxes[curr_box_idx--]->reset_determination();
    }

    if (curr_box_idx == 0 && boxes[0]->is_determined()) {
        return false;
    }

    boxes[curr_box_idx]->determine_next();

    if (curr_box_idx == 0) {
        partial_prob[curr_box_idx] = boxes[curr_box_idx]->get_probability();
    } else {
        partial_prob[curr_box_idx] = partial_prob[curr_box_idx - 1] * boxes[curr_box_idx]->get_probability();
    }

    if (curr_box_idx > 0 && partial_prob[curr_box_idx] < beta_thresh) {
        boxes[curr_box_idx--]->reset_determination();
        return true;
    } else if (curr_box_idx == 0 && partial_prob[curr_box_idx] < beta_thresh) {
        return false;
    }

    boxes[curr_box_idx]->notify_all();

    if (curr_box_idx < boxes.size()) {
        curr_box_idx++;
    }

    return true;
}

void RoundFunction::top_sort_boxes(AbstractBoxPtr src, vector<AbstractBoxPtr> &sort,
                                   map<AbstractBoxPtr, bool> &is_visited) {
    is_visited[src] = true;
    for (auto &dst_box : src->dst_boxes) {
        if (is_visited.find(dst_box.first) == is_visited.end()) {
            top_sort_boxes(dst_box.first, sort, is_visited);
        }
    }
    sort.push_back(src);
}

vector<AbstractBoxPtr> RoundFunction::sort_boxes(AbstractBoxPtr src) {
    vector<AbstractBoxPtr> boxes;
    map<AbstractBoxPtr, bool> is_visited;

    top_sort_boxes(src, boxes, is_visited);
    reverse(boxes.begin(), boxes.end());

    return boxes;
}

RoundFunction::RoundFunction(string src_id, string dst_id, map<string, AbstractBoxConstructor> constrs,
                             map<string, vector<NamedConnection>> conns) {
    beta_thresh = 0.0;
    is_det = false;
    partial_prob.resize(constrs.size());

    map<string, AbstractBoxPtr> box_map;
    for (auto &constr : constrs) {
        box_map[constr.first] = constr.second();
    }

    for (auto &conn : conns) {
        for (auto &dst_box : conn.second) {
            box_map[conn.first]->add_dest(box_map[dst_box.first], dst_box.second.first, dst_box.second.second);
        }
    }

    src = box_map[src_id];
    dst = box_map[dst_id];
    boxes = sort_boxes(src);

    curr_box_idx = 0;
}

bool RoundFunction::is_determined() { return is_det; }

ProbEntry RoundFunction::get_next_state() {
    while (curr_box_idx < boxes.size()) {
        if (!advance_state()) {
            is_det = true;
            return {dynamic_bitset<>(0), 0.0};
        }
    }
    curr_box_idx--;
    return {dst->get_output(), partial_prob.back()};
}

void RoundFunction::set_input(const dynamic_bitset<> bits, BitsRange rng) {
    curr_box_idx = 0;
    is_det = false;
    for (auto &box : boxes) {
        box->reset_determination();
    }
    src->set_input(bits, rng);
}

void RoundFunction::set_threshold(double beta) { beta_thresh = beta; }
