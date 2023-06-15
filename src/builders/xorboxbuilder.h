#ifndef XORBOXBUILDER_H
#define XORBOXBUILDER_H

#include "abstractboxbuilder.h"
#include <box/xorbox.h>

class XorBoxBuilder : public AbstractBoxBuilder {
    size_t data_size;

  public:
    XorBoxBuilder(size_t data_size);

    AbstractBoxPtr build() override;
};

typedef std::shared_ptr<XorBoxBuilder> XorBoxBuilderPtr;

XorBoxBuilderPtr make_xorbox_builder(size_t data_size);

#endif // XORBOXBUILDER_H
