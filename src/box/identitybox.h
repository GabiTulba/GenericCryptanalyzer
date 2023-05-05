#ifndef IDENTITYBOX_H
#define IDENTITYBOX_H

#include "abstractbox.h"

class IdentityBox : public AbstractBox {
  public:
    IdentityBox(size_t data_size,
                const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes);
    IdentityBox(size_t data_size);

    void determine_next() override;
};

typedef std::shared_ptr<IdentityBox> IdentityBoxPtr;

#endif // IDENTITYBOX_H
