#include "roundfunction.h"

bool RoundFunction::advance_state() {
    while (curr_state_box_idx > 0 &&
           boxes[curr_state_box_idx]->is_determined()) {
        boxes[curr_state_box_idx--]->reset_determination();
    }

    if (curr_state_box_idx == 0 && boxes[0]->is_determined()) {
        return false;
    }

    boxes[curr_state_box_idx]->determine_next();

    if (curr_state_box_idx == 0) {
        partial_probabilities[curr_state_box_idx] =
            boxes[curr_state_box_idx]->get_probability();
    } else {
        partial_probabilities[curr_state_box_idx] =
            partial_probabilities[curr_state_box_idx - 1] *
            boxes[curr_state_box_idx]->get_probability();
    }

    if (curr_state_box_idx > 0 &&
        partial_probabilities[curr_state_box_idx] < beta_threshold) {
        boxes[curr_state_box_idx--]->reset_determination();
        return true;
    } else if (curr_state_box_idx == 0 &&
               partial_probabilities[curr_state_box_idx] < beta_threshold) {
        return false;
    }

    boxes[curr_state_box_idx]->notify_all();

    if (curr_state_box_idx < boxes.size()) {
        curr_state_box_idx++;
    }

    return true;
}

void RoundFunction::top_sort_boxes(
    std::shared_ptr<AbstractBox> src,
    vector<std::shared_ptr<AbstractBox>> &sort,
    map<std::shared_ptr<AbstractBox>, bool> &visited) {
    visited[src] = true;
    for (auto &dest_box : src->dest_boxes) {
        if (visited.find(dest_box.first) == visited.end()) {
            top_sort_boxes(dest_box.first, sort, visited);
        }
    }
    sort.push_back(src);
}

vector<std::shared_ptr<AbstractBox>>
RoundFunction::sort_boxes(std::shared_ptr<AbstractBox> src) {
    vector<std::shared_ptr<AbstractBox>> boxes;
    map<std::shared_ptr<AbstractBox>, bool> visited;

    top_sort_boxes(src, boxes, visited);
    reverse(boxes.begin(), boxes.end());

    return boxes;
}

RoundFunction::RoundFunction(
    string src_id, string dst_id,
    map<string, function<std::shared_ptr<AbstractBox>()>> constructors,
    map<string, vector<pair<string, pair<bits_range, bits_range>>>>
        connections) {
    beta_threshold = 0.0;
    determined = false;
    partial_probabilities.resize(constructors.size());

    map<string, std::shared_ptr<AbstractBox>> box_map;
    for (auto &entry : constructors) {
        box_map[entry.first] = entry.second();
    }

    for (auto &entry : connections) {
        for (auto &dst_box : entry.second) {
            box_map[entry.first]->add_dest(box_map[dst_box.first],
                                           dst_box.second.first,
                                           dst_box.second.second);
        }
    }

    src = box_map[src_id];
    dst = box_map[dst_id];
    boxes = sort_boxes(src);

    curr_state_box_idx = 0;
}

bool RoundFunction::is_determined() { return determined; }

pair<dynamic_bitset<>, double> RoundFunction::get_next_differential() {
    while (curr_state_box_idx < boxes.size()) {
        if (!advance_state()) {
            determined = true;
            return {dynamic_bitset<>(0), 0.0};
        }
    }
    curr_state_box_idx--;
    return {dst->get_output(), partial_probabilities.back()};
}

void RoundFunction::set_input(const dynamic_bitset<> bits, bits_range range) {
    curr_state_box_idx = 0;
    determined = false;
    for (auto &box : boxes) {
        box->reset_determination();
    }
    src->set_input(bits, range);
}

void RoundFunction::set_threshold(double beta) { beta_threshold = beta; }
