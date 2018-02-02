#ifndef DRIFT_SPACE_H
#define DRIFT_SPACE_H


#include "element.h"

class Drift : public Element {
public:
  Drift(double length, std::string name="Drift")
    : Element(0, length, name) {}
};

#endif
