#ifndef SBOX_H
#define SBOX_H

#include "abstractbox.h"
#include "src/helpers/helpers.h"

class SBox : public AbstractBox {
  protected:
    ProbTable prob_table;
    size_t table_idx;
    size_t table_entry;

    size_t convert_to_index(const dynamic_bitset<> &bits);

  public:
    SBox(size_t in_size, size_t out_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
         const ProbTable &prob_table);
    SBox(size_t in_size, size_t out_size, const ProbTable &prob_table);

    void determine_next() override;
    void reset_determination() override;

    void set_input(dynamic_bitset<> bits, const BitsRange &rng) override;
};

typedef std::shared_ptr<SBox> SBoxPtr;

#endif // SBOX_H
