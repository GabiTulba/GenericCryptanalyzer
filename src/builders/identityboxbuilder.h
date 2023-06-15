#ifndef IDENTITYBOXBUILDER_H
#define IDENTITYBOXBUILDER_H

#include "abstractboxbuilder.h"
#include <box/identitybox.h>

class IdentityBoxBuilder : public AbstractBoxBuilder {
  private:
    size_t data_size;

  public:
    IdentityBoxBuilder(size_t data_size);

    AbstractBoxPtr build() override;
};

AbstractBoxBuilderPtr make_identitybox_builder(size_t data_size);

#endif // IDENTITYBOXBUILDER_H
