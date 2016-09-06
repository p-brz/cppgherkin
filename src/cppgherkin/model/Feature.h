#ifndef FEATURE_H
#define FEATURE_H

#include "SectionDescription.h"
#include "Section.h"

namespace cppgherkin {

class Feature : public Section{
public:
	Feature(){
		key("Feature");
	}

private:
};

}//namespace

#endif // FEATURE_H
