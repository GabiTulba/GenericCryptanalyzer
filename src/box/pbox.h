#ifndef PBOX_H
#define PBOX_H

#include "abstractbitshiftbox.h"

class PBox : public AbstractBitShiftBox {
  public:
    PBox(size_t bits_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes, const vector<size_t> &bit_perm);
    PBox(size_t bits_size, const vector<size_t> &bit_perm);
};

typedef std::shared_ptr<PBox> PBoxPtr;

#endif // PBOX_H
