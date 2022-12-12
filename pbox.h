#ifndef PBOX_H
#define PBOX_H

#include "abstractbitshiftbox.h"

class PBox : public AbstractBitShiftBox {
  public:
    PBox(size_t bits_size,
         const vector<pair<std::shared_ptr<AbstractBox>,
                           pair<bits_range, bits_range>>> &dest_boxes,
         const vector<size_t> &bit_permutation);
    PBox(size_t bits_size, const vector<size_t> &bit_permutation);
};

#endif // PBOX_H
