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
		parseDescription(feature());
		parseScenarios();
	}

	Feature & feature(){ return _feature;}
	const
	Feature & feature() const{ return _feature;}

protected:
	bool parseDefinition(Section & section);
	void parseDescription(Section & section, bool extractKey=false);
	void parseScenarios();

	template<typename ...Args>
	bool findPattern(const std::string & expectedToken, Args&&... outArgs);

private:
	std::istream & input;
	Feature _feature;

	RegexMatcher descriptionLineMatcher{R"(^[\s]*(.*)$)"},
		emptyLineMatcher{R"(^[\s]*$)"},
		stepMatcher{R"(^[\s]*([\S]*)[\s]*(.*)$)"};
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
bool FeatureParser::parseDefinition(Section & section){
	constexpr auto featurePattern = R"(^[\s]*([\S]*):[\s]*(.*)$)";

	std::string key, title;
	if(findPattern(featurePattern, key, title)){
		section.key(std::move(ltrim(key)));
		section.title(std::move(ltrim(title)));

		return true;
	}

	return false;
}

inline
void FeatureParser::parseDescription(Section & section, bool extractKey){

	std::string line;
	while(std::getline(input, line)){
		if(line.empty()){
			return;
		}
		if(emptyLineMatcher.match(line)){
			return;
		}

		auto & matcher = extractKey ? stepMatcher : descriptionLineMatcher;
		auto result = matcher.match(line);
		if(result.groups().size() == 1){
			section.addLine(rtrim(result[0]));
		}
		else{
			section.addLine(SectionLine().key(result[0]).phrase(result[1]));
		}
	}
}

inline
void FeatureParser::parseScenarios(){
	while(input){
		Scenario scenario;
		if(!parseDefinition(scenario)){
			break;
		}
		parseDescription(scenario, true);

		feature().add(std::move(scenario));
	}
}

}//namespace

#endif // FEATUREPARSER_H
