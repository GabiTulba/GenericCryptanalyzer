#include "identityboxbuilder.h"

IdentityBoxBuilder::IdentityBoxBuilder(size_t data_size) : data_size(data_size) {}

AbstractBoxPtr IdentityBoxBuilder::build() { return std::make_shared<IdentityBox>(data_size); }

AbstractBoxBuilderPtr make_identitybox_builder(size_t data_size) {
    return std::make_shared<IdentityBoxBuilder>(data_size);
}
