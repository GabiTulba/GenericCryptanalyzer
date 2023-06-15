#ifndef PBOXBUILDER_H
#define PBOXBUILDER_H

#include "abstractboxbuilder.h"
#include <box/pbox.h>

class PBoxBuilder : public AbstractBoxBuilder {
  private:
    vector<size_t> bit_perm;

  public:
    PBoxBuilder(vector<size_t> bit_perm);

    AbstractBoxPtr build() override;
};

AbstractBoxBuilderPtr make_pbox_builder(vector<size_t> bit_perm);

#endif // PBOXBUILDER_H
