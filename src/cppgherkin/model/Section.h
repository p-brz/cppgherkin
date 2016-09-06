#ifndef SECTION_H
#define SECTION_H

#include "SectionDescription.h"

#define MAKE_PROPERTY_GETTER(type, name)\
	const type & name() const{\
		return _##name;\
	}
#define MAKE_PROPERTY_SETTER(Class, type, name)\
	template<typename T=type>\
	Class & name(T&& value){\
		_##name = value;\
		return *this;\
	}

#define MAKE_PROPERTY(Class, type, name)\
	MAKE_PROPERTY_GETTER(type, name)\
	MAKE_PROPERTY_SETTER(Class, type, name)

namespace cppgherkin {

class Section{
public:
	MAKE_PROPERTY(Section, std::string, title)
	MAKE_PROPERTY(Section, std::string, key)

	const
	SectionDescription & description() const { return _description;}
	SectionDescription & description()	   { return _description;}

private:
	std::string _key, _title;
	SectionDescription _description;
};

std::ostream & operator<<(std::ostream & os, const Section & sect){
	os << sect.key() << ": " << sect.title() << std::endl;

	bool first=true;
	for(const auto & line : sect.description().lines()){
		if(!first){
			os << "\n\t";
		}
		os << line;
		first = false;
	}

	return os;
}

}//namespace

#endif // SECTION_H
