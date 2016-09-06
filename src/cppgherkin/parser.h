#ifndef PARSER_H
#define PARSER_H

#include "parser/FeatureParser.h"

#include <string>
#include <istream>
#include <fstream>

namespace cppgherkin {

class GherkinParser{
public:
	Feature parse(const std::string & filepath);
	Feature parse(std::istream & featureStream);
};

Feature GherkinParser::parse(const std::string & filepath){
	std::ifstream input{filepath, std::ios_base::in};
	return parse(input);
}

inline Feature GherkinParser::parse(std::istream & featureStream){
	FeatureParser parser(featureStream);
	parser.parse();

	return std::move(parser.feature());
}

}//namespace

#endif // PARSER_H
