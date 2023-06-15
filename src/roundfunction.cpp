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

    boxes[curr_box_idx]->notify_all();

    for (size_t idx = curr_box_idx == 0 ? 1 : end_det[curr_box_idx - 1] + 1; idx <= end_det[curr_box_idx]; idx++) {
        partial_best_prob[idx] = partial_best_prob[idx - 1] * boxes[idx]->get_best_prob();
    }

    if (curr_box_idx > 0 && partial_prob[curr_box_idx] * partial_best_prob[end_det[curr_box_idx]] <
                                beta_thresh * partial_best_prob[curr_box_idx]) {
        boxes[curr_box_idx--]->reset_determination();
        return true;
    } else if (curr_box_idx == 0 && partial_prob[curr_box_idx] * partial_best_prob[end_det[curr_box_idx]] <
                                        beta_thresh * partial_best_prob[curr_box_idx]) {
        return false;
    }

    if (curr_box_idx < boxes.size()) {
        curr_box_idx++;
    }

    return true;
}

vector<AbstractBoxPtr> RoundFunction::sort_boxes(AbstractBoxPtr src) {
    vector<AbstractBoxPtr> boxes;
    map<AbstractBoxPtr, size_t> visited_cnt;
    queue<AbstractBoxPtr> q;

    q.push(src);
    while (!q.empty()) {
        src = q.front();
        q.pop();

        for (auto &[dst_box, conn] : src->dst_boxes) {
            visited_cnt[dst_box]++;
            if (visited_cnt[dst_box] == dst_box->src_boxes.size()) {
                q.push(dst_box);
            }
        }

        boxes.push_back(src);
        end_det.push_back(boxes.size() + q.size() - 1);
    }

    return boxes;
}

RoundFunction::RoundFunction(string src_id, string dst_id, map<string, AbstractBoxBuilderPtr> constrs,
                             map<string, vector<NamedConnection>> conns) {
    beta_thresh = 0.0;
    is_det = false;
    partial_prob.resize(constrs.size());
    partial_best_prob.resize(constrs.size());

    partial_best_prob[0] = 1.0;

    map<string, AbstractBoxPtr> box_map;
    for (auto &[box_name, box_constructor] : constrs) {
        box_map[box_name] = box_constructor->build();
    }

    for (auto &[src_box, named_conn] : conns) {
        for (auto &[dst_box, conn] : named_conn) {
            box_map[src_box]->add_dest(box_map[dst_box], conn.first, conn.second);
            box_map[dst_box]->add_src(box_map[src_box], conn.first, conn.second);
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
