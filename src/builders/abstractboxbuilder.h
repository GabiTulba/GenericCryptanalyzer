#ifndef ABSTRACTBOXBUILDER_H
#define ABSTRACTBOXBUILDER_H

#include <box/abstractbox.h>

class AbstractBoxBuilder {
  public:
    AbstractBoxBuilder();

    virtual AbstractBoxPtr build() = 0;
};

typedef std::shared_ptr<AbstractBoxBuilder> AbstractBoxBuilderPtr;

#endif // ABSTRACTBOXBUILDER_H
