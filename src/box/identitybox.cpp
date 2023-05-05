#include "identitybox.h"

IdentityBox::IdentityBox(
    size_t data_size, const vector<pair<AbstractBoxPtr, Connection>> &dst_boxes)
    : AbstractBox(data_size, data_size, dst_boxes) {}

IdentityBox::IdentityBox(size_t data_size)
    : AbstractBox(data_size, data_size) {}

void IdentityBox::determine_next() {
    out_bits = in_bits;
    is_det = true;
    prob = 1.0;
}
