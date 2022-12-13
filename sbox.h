#ifndef SBOX_H
#define SBOX_H

#include "abstractbox.h"
#include "helpers.h"

class SBox : public AbstractBox {
  protected:
    vector<vector<pair<dynamic_bitset<>, double>>> differential_table;
    size_t table_index;
    size_t table_entry;

    size_t convert_to_index(const dynamic_bitset<> &bits);

  public:
    SBox(size_t input_size, size_t output_size,
         const vector<pair<std::shared_ptr<AbstractBox>,
                           pair<bits_range, bits_range>>> &dest_boxes,
         const vector<vector<pair<dynamic_bitset<>, double>>>
             &differential_table);
    SBox(size_t input_size, size_t output_size,
         const vector<vector<pair<dynamic_bitset<>, double>>>
             &differential_table);

    void determine_next() override;
    void reset_determination() override;

    void set_input(dynamic_bitset<> bits, const bits_range &range) override;
};

vector<vector<pair<dynamic_bitset<>, double>>>
compute_differential_table(const vector<size_t> &sbox);

#endif // SBOX_H
