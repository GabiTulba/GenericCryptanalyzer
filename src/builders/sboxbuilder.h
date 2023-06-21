#ifndef SBOXBUILDER_H
#define SBOXBUILDER_H

#include "abstractboxbuilder.h"
#include <box/sbox.h>

class SBoxBuilder : public AbstractBoxBuilder {
  private:
    size_t bits_in_size;
    size_t bits_out_size;
    ProbTablePtr prob_table;
    bool is_exhaustive;

  public:
    SBoxBuilder(size_t bits_in_size, size_t bits_out_size, ProbTablePtr prob_table, bool is_exhaustive);

    AbstractBoxPtr build() override;
};

AbstractBoxBuilderPtr make_sbox_builder(size_t bits_in_size, size_t bits_out_size, ProbTablePtr prob_table,
                                        bool is_exhaustive);
#endif // SBOXBUILDER_H
