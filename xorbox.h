#ifndef XORBOX_H
#define XORBOX_H

#include "abstractbox.h"

class XorBox : public AbstractBox {
  public:
    XorBox(size_t data_size,
           const vector<pair<std::shared_ptr<AbstractBox>,
                             pair<bits_range, bits_range>>> &dest_boxes);
    XorBox(size_t data_size);

    void determine_next() override;
};

#endif // XORBOX_H
