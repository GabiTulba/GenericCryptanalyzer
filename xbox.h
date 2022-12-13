#ifndef XBOX_H
#define XBOX_H

#include "abstractbox.h"

class XBox : public AbstractBox {
  public:
    XBox(size_t data_size,
         const vector<pair<std::shared_ptr<AbstractBox>,
                           pair<bits_range, bits_range>>> &dest_boxes);
    XBox(size_t data_size);

    void determine_next() override;
};

#endif // XBOX_H
