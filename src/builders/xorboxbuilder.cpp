#include "xorboxbuilder.h"

XorBoxBuilder::XorBoxBuilder(size_t data_size) : data_size(data_size) {}

AbstractBoxPtr XorBoxBuilder::build() { return std::make_shared<XorBox>(data_size); }

XorBoxBuilderPtr make_xorbox_builder(size_t data_size) { return std::make_shared<XorBoxBuilder>(data_size); }
