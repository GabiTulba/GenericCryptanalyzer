#ifndef EBOX_H
#define EBOX_H

#include "abstractbitshiftbox.h"

class EBox : public AbstractBitShiftBox {
  public:
    EBox(size_t input_size, size_t output_size,
         const vector<pair<std::shared_ptr<AbstractBox>,
                           pair<bits_range, bits_range>>> &dest_boxes,
         const vector<size_t> &bit_expansion);
    EBox(size_t input_size, size_t output_size,
         const vector<size_t> &bit_expansion);
};

#endif // EBOX_H
