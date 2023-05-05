#ifndef EBOX_H
#define EBOX_H

#include "abstractbitshiftbox.h"

class EBox : public AbstractBitShiftBox {
  public:
    EBox(size_t in_size, size_t out_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes,
         const vector<size_t> &bit_expansion);
    EBox(size_t in_size, size_t out_size, const vector<size_t> &bit_expansion);
};

typedef std::shared_ptr<EBox> EBoxPtr;

#endif // EBOX_H
