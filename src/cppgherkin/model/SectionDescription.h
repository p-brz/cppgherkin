#ifndef FEATUREDESCRIPTION_H
#define FEATUREDESCRIPTION_H

#include <string>
#include <vector>

#include "commonex/commonex.h"

namespace cppgherkin {

class SectionDescription{
public:
	using Line = std::string;
	using Lines = std::vector<Line>;

public:
	SectionDescription & addLine(const Line & l){
		_lines.push_back(l);
		return *this;
	}

	const Line & line(int index) const;

	const Lines & lines() const;
	SectionDescription & lines(const Lines & lines);

private:
	Lines _lines;
};

inline
const SectionDescription::Line &SectionDescription::line(int index) const{
	CHECK_BOUNDS(lines(), index);
	return _lines[index];
}

inline
const SectionDescription::Lines &SectionDescription::lines() const {
	return _lines;
}

inline
SectionDescription &SectionDescription::lines(const SectionDescription::Lines & lines) {
	_lines = lines;
	return *this;
}

}//namespace

#endif // FEATUREDESCRIPTION_H
