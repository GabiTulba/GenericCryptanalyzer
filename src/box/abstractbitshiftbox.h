#ifndef ABSTRACTBITSHIFTBOX_H
#define ABSTRACTBITSHIFTBOX_H

#include "abstractbox.h"

class AbstractBitShiftBox : public AbstractBox {
  protected:
    vector<size_t> bit_src;
    void apply_transformation();

  public:
    AbstractBitShiftBox(
        size_t in_size, size_t out_size,
        const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
        const vector<size_t> &bit_src);
    AbstractBitShiftBox(size_t in_size, size_t out_size,
                        const vector<size_t> &bit_src);
    void determine_next() override;
};

typedef std::shared_ptr<AbstractBitShiftBox> AbstractBitShiftBoxPtr;

#endif // ABSTRACTBITSHIFTBOX_H
