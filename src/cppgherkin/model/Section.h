#ifndef SECTION_H
#define SECTION_H

#include "SectionDescription.h"
#include "../utils/MakeProperty.h"

namespace cppgherkin {

class Section{
public:
	Section() = default;
	template<typename Key, typename Title>
	Section(Key&& key, Title&& title, const SectionDescription & desc ={})
		: _key(key)
		, _title(title)
		, _description(desc)
	{}

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

	for(const auto & line : sect.description().lines()){
		os << "\t";
		os << line;
		os << std::endl;
	}

	return os;
}

}//namespace

#endif // SECTION_H
