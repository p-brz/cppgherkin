#include "test.h"

#include "cppgherkin/parser.h"

#include <sstream>
#include <vector>

using namespace cppgherkin;

#define FIXTURE ParserTest
#define TEST_TAG "GherkinParser"

class ParserTest{
public:
	void testBasicFeature();
	void testFeatureDescription();
public:
	GherkinParser parser;
};

FIXTURE_SCENARIO_METHOD("Basic feature file has a 'Feature:<title>'"	, testBasicFeature)
FIXTURE_SCENARIO_METHOD("Feature files can have a multiline description", testFeatureDescription)

/* ******************************************** Implementation *****************************************************/

void ParserTest::testBasicFeature(){
GIVEN("a basic feature with a title"){
	stringstream content{"Feature: parse feature files"};

WHEN("parsing that feature"){
	Feature f = parser.parse(content);

THEN("the returned Feature should have that title"){
	CHECK(f.title() == "parse feature files");

}//THEN
}//WHEN
}//GIVEN

}

void ParserTest::testFeatureDescription(){
GIVEN("a feature file with title and description"){
	Feature expected;
	expected.title("some title")
		.description()
			.addLine("As a BDD tester")
			.addLine("I want to describe my user story")
			.addLine("To easy communicate the feature purpose.");

	stringstream content;
	content << expected << endl;

WHEN("parsing it"){
	Feature f = parser.parse(content);

THEN("the returned feature should have the given title"){
	CHECK(f.title() == expected.title());

THEN("the feature should keep the description lines"){
	CHECK(f.description().lines().size() == 3);
	auto&& lines = expected.description().lines();
	for (int i = 0; i < lines.size(); ++i) {
		CAPTURE(i);
		auto&& descLine = f.description().line(i);
		auto&& expected = lines[i];

		CAPTURE(descLine);
		CAPTURE(expected);
		CHECK(descLine == expected);
	}

}//THEN
}//THEN
}//WHEN
}//GIVEN

}//test

