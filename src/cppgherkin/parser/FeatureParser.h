#ifndef FEATUREPARSER_H
#define FEATUREPARSER_H

#include "../model/Feature.h"
#include "../utils/RegexMatcher.h"

#include <iostream>

namespace cppgherkin {

class FeatureParser{
public:
	FeatureParser(std::istream & input) : input(input)
	{}

	void parse(){
		parseDefinition(feature());
		parseDescription(feature().description());
	}

	Feature & feature(){ return _feature;}
	const
	Feature & feature() const{ return _feature;}

protected:
	void parseDefinition(Section & section);
	void parseDescription(SectionDescription & detail);

	template<typename ...Args>
	bool findPattern(const std::string & expectedToken, Args&&... outArgs);

private:
	std::istream & input;
	Feature _feature;
};

static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}
static inline std::string &rtrim(std::string &s) {
	while(!s.empty() && std::isspace(s.back())){
		s.pop_back();
	}
	return s;
}

template<typename Iter>
static inline void extract_args(Iter begin, Iter end)
{}
template<typename Iter, typename Arg, typename ...Args>
static inline void extract_args(Iter begin, Iter end, Arg & out, Args&&... args){
	if(begin != end){
		out = *begin;

		extract_args(++begin, end, std::forward<Args>(args)...);
	}
}

template<typename ...Args>
bool FeatureParser::findPattern(const std::string & pattern, Args&&... outArgs){
	RegexMatcher re = pattern;

	std::string line;
	while(getline(input, line)){
		auto results = re.match(line);
		if(!results.empty()){
			extract_args(results.begin(), results.end(), outArgs...);
			return true;
		}
	}


	return false;
}

inline
void FeatureParser::parseDefinition(Section & section){
	constexpr auto featurePattern = R"(^[\s]*Feature:[\s]*(.*)$)";

	std::string title;
	if(findPattern(featurePattern, title)){
		section.title(std::move(ltrim(title)));
	}

}

void FeatureParser::parseDescription(SectionDescription & detail){
	RegexMatcher nonEmptyLineMatcher{R"(^[\s]*(.*)$)"};

	std::string line;
	while(std::getline(input, line)){
		if(line.empty()){
			return;
		}
		auto result = nonEmptyLineMatcher.match(line);
		if(!result){
			return;
		}

		detail.addLine(rtrim(result[0]));
	}
}

}//namespace

#endif // FEATUREPARSER_H
