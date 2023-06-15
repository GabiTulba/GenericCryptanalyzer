#include "pboxbuilder.h"

PBoxBuilder::PBoxBuilder(vector<size_t> bit_perm) : bit_perm(bit_perm) {}

AbstractBoxPtr PBoxBuilder::build() { return std::make_shared<PBox>(bit_perm.size(), bit_perm); }

AbstractBoxBuilderPtr make_pbox_builder(vector<size_t> bit_perm) { return std::make_shared<PBoxBuilder>(bit_perm); }
