#ifndef SUBZONE_H_
#define SUBZONE_H_

#include <omnetpp.h>

using namespace omnetpp;

class SubZone : public cSimpleModule
{
  protected:
    virtual void initialize() override;

  protected:
//    double width; // Width of the sub-zone
//    double height; // Height of the sub-zone

  public:
    SubZone() {}
    virtual ~SubZone() {}
};

#endif /* SUBZONE_H_ */
