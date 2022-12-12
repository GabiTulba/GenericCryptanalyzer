#include "identitybox.h"

IdentityBox::IdentityBox(
    size_t data_size,
    const vector<pair<std::shared_ptr<AbstractBox>,
                      pair<bits_range, bits_range>>> &dest_boxes)
    : AbstractBox(data_size, data_size, dest_boxes) {}

IdentityBox::IdentityBox(size_t data_size)
    : AbstractBox(data_size, data_size) {}

void IdentityBox::determine_next() {
    output_bits = input_bits;
    determined = true;
    probability = 1.0;
}
