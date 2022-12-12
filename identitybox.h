#ifndef IDENTITYBOX_H
#define IDENTITYBOX_H

#include "abstractbox.h"

class IdentityBox : public AbstractBox {
  public:
    IdentityBox(size_t data_size,
                const vector<pair<std::shared_ptr<AbstractBox>,
                                  pair<bits_range, bits_range>>> &dest_boxes);
    IdentityBox(size_t data_size);

    void determine_next() override;
};

#endif // IDENTITYBOX_H
