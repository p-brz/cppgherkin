#ifndef SECTION_H
#define SECTION_H

#include "SectionLine.h"
#include "../utils/MakeProperty.h"

namespace cppgherkin {

class Section{
public:
	using Line = SectionLine;
	using Lines = std::vector<Line>;

public:
	Section() = default;

	Section(const std::string& key, const std::string& title=std::string())
		: _key(key)
		, _title(title)
	{}


	MAKE_PROPERTY(Section, std::string, title)
	MAKE_PROPERTY(Section, std::string, key)
	MAKE_PROPERTY(Section, Lines, lines)


	bool operator==(const Section& other)const{
		return title() == other.title()
			&& key() == other.key()
			&& lines() == other.lines();
	}
public:
	Section & addLine(const Line & l){
		_lines.push_back(l);
		return *this;
	}

	const Line & line(int index) const{
		CHECK_BOUNDS(_lines, index);
		return _lines[index];
	}

private:
	std::string _key, _title;
	Lines _lines;
};

inline std::ostream & operator<<(std::ostream & os, const Section & sect){
	os << sect.key() << ": " << sect.title() << std::endl;

	for(const auto & line : sect.lines()){
		os << "\t";
		os << line;
		os << std::endl;
	}

	return os;
}

}//namespace

#endif // SECTION_H
