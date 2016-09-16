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

inline std::ostream & operator<<(std::ostream & out, const SectionLine & line){
	out << line.key() << " " << line.phrase();
	return out;
}

}//namespace

#endif // FEATUREDESCRIPTION_H
