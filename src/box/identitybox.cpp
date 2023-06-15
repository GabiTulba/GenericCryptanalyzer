#include "identitybox.h"

double IdentityBox::get_best_prob() { return 1.0; }

IdentityBox::IdentityBox(size_t data_size) : AbstractBox(data_size, data_size) {}

void IdentityBox::determine_next() {
    if (!is_det) {
        out_bits = in_bits;
        is_det = true;
        prob = 1.0;
    }
}
