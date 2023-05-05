#ifndef XORBOX_H
#define XORBOX_H

#include "abstractbox.h"

class XorBox : public AbstractBox {
  public:
    XorBox(size_t data_size,
           const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes);
    XorBox(size_t data_size);

    void determine_next() override;
};

typedef std::shared_ptr<XorBox> XorBoxPtr;

#endif // XORBOX_H
