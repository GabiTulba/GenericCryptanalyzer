#ifndef SBOXBUILDER_H
#define SBOXBUILDER_H

#include "abstractboxbuilder.h"
#include <box/sbox.h>

class SBoxBuilder : public AbstractBoxBuilder {
  private:
    vector<size_t> sbox;
    bool is_exhaustive;

  public:
    SBoxBuilder(vector<size_t> sbox, bool is_exhaustive);

    AbstractBoxPtr build() override;
};

AbstractBoxBuilderPtr make_sbox_builder(vector<size_t> sbox, bool is_exhaustive);

#endif // SBOXBUILDER_H
