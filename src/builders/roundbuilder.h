#ifndef ROUNDBUILDER_H
#define ROUNDBUILDER_H

#include "abstractboxbuilder.h"
#include <roundfunction.h>

class RoundBuilder {
    map<string, AbstractBoxBuilderPtr> builders;
    map<string, vector<NamedConnection>> connections;
    string src;
    string dst;

  public:
    RoundBuilder();

    void add_builder(string name, AbstractBoxBuilderPtr builder);
    void add_connection(string src, string dst, BitsRange out_rng, BitsRange in_rng);
    void set_src(string src);
    void set_dst(string dst);

    RoundFunctionPtr build();
};

typedef std::shared_ptr<RoundBuilder> RoundBuilderPtr;

RoundBuilderPtr make_round_builder();

#endif // ROUNDBUILDER_H
