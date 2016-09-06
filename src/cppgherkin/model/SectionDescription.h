#ifndef FEATUREDESCRIPTION_H
#define FEATUREDESCRIPTION_H

#include "../utils/MakeProperty.h"

#include <string>
#include <vector>

#include "commonex/commonex.h"

namespace cppgherkin {

class SectionLine{
public:
	SectionLine()=default;
	SectionLine(const char * line) : _phrase(line)
	{}
	SectionLine(const std::string & line) : _phrase(line)
	{}
	SectionLine(const std::string & key, const std::string & phrase) :
		_key(key), _phrase(phrase)
	{}

public:
	MAKE_PROPERTY_GETTER(std::string, key)
	MAKE_PROPERTY_SETTER(SectionLine, std::string, key, clear_join())

	MAKE_PROPERTY_GETTER(std::string, phrase)
	MAKE_PROPERTY_SETTER(SectionLine, std::string, phrase, clear_join())

	bool operator==(const SectionLine & other) const{
		return key() == other.key() && phrase() == other.phrase();
	}

	const std::string & join() const{
		if(key().empty()){
			return phrase();
		}

		if(joined.size() < (key().size() + phrase().size())){
			joined.clear();
			if(!key().empty()){
				joined += key() + " ";
			}
			joined += phrase();
		}

		return joined;
	}

	operator const std::string &() const{
		return join();
	}
private:
	void clear_join() const{
		joined.clear();
	}
private:
	std::string _key, _phrase;
	mutable std::string joined;
};

std::ostream & operator<<(std::ostream & out, const SectionLine & line){
	out << line.key() << " " << line.phrase();
	return out;
}

class SectionDescription{
public:
	using Line = SectionLine;
	using Lines = std::vector<Line>;

public:
	SectionDescription() = default;

	template<typename LinesT>
	SectionDescription(LinesT&& lines) : _lines(lines)
	{}

	size_t size() const{
		return lines().size();
	}

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
