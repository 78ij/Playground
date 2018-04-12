#ifndef GEOCONTEXT_H
#define GEOCONTEXT_H

#include "Base.h"
#include "Shapes.h"

class Geocontext {
public:
	Geocontext(){}
private:
	set<Shapes *> shapes;
};


#endif // GEOCONTEXT_H
