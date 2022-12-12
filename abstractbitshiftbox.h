#ifndef ABSTRACTBITSHIFTBOX_H
#define ABSTRACTBITSHIFTBOX_H

#include "abstractbox.h"

class AbstractBitShiftBox : public AbstractBox {
  protected:
    vector<size_t> bit_source;
    void apply_transformation();

  public:
    AbstractBitShiftBox(
        size_t input_size, size_t output_size,
        const vector<pair<std::shared_ptr<AbstractBox>,
                          pair<bits_range, bits_range>>> &dest_boxes,
        const vector<size_t> &bit_source);
    AbstractBitShiftBox(size_t input_size, size_t output_size,
                        const vector<size_t> &bit_source);
    void determine_next() override;
};

#endif // ABSTRACTBITSHIFTBOX_H
