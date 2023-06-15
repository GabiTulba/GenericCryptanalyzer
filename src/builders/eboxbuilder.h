#ifndef EBOXBUILDER_H
#define EBOXBUILDER_H

#include "abstractboxbuilder.h"
#include <box/ebox.h>

class EBoxBuilder : public AbstractBoxBuilder {
  private:
    vector<size_t> bit_expansion;

  public:
    EBoxBuilder(vector<size_t> bit_expansion);

    AbstractBoxPtr build() override;
};

AbstractBoxBuilderPtr make_ebox_builder(vector<size_t> bit_expansion);

#endif // EBOXBUILDER_H
