#include "roundbuilder.h"

RoundBuilder::RoundBuilder() {}

void RoundBuilder::add_builder(string name, AbstractBoxBuilderPtr builder) { builders.insert({name, builder}); }

void RoundBuilder::add_connection(string src, string dst, BitsRange out_rng, BitsRange in_rng) {
    connections[src].push_back({dst, {out_rng, in_rng}});
}

void RoundBuilder::set_src(string src) { this->src = src; }
void RoundBuilder::set_dst(string dst) { this->dst = dst; }

RoundFunctionPtr RoundBuilder::build() { return std::make_shared<RoundFunction>(src, dst, builders, connections); }

RoundBuilderPtr make_round_builder() { return std::make_shared<RoundBuilder>(); }
