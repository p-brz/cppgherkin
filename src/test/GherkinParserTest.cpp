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
	void testBasicScenario();
	void testScenarioWithSteps();
	void testMultipleScenarios();

public:

	template<typename Coll>
	std::string join(Coll & coll, const std::string & separator);
public:
	GherkinParser parser;
};

FIXTURE_SCENARIO_METHOD("Basic feature file has a 'Feature:<title>'"			 , testBasicFeature)
FIXTURE_SCENARIO_METHOD("Feature files can have a multiline description"		 , testFeatureDescription)
FIXTURE_SCENARIO_METHOD("Parser should be able to handle features with scenarios", testBasicScenario)
FIXTURE_SCENARIO_METHOD("Each scenario in a feature file can have many steps"	 , testScenarioWithSteps)
FIXTURE_SCENARIO_METHOD("A feature file can have many scenarios"				 , testMultipleScenarios)

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
			.lines({
				"As a BDD tester"
				,"I want to describe my user story"
				,"To easy communicate the feature purpose."
			});

	stringstream content;
	content << expected << endl;

WHEN("parsing it"){
	Feature f = parser.parse(content);

THEN("the returned feature should have the given title"){
	CHECK(f.title() == expected.title());

THEN("the feature should keep the description lines"){
	CHECK(f.lines().size() == 3);
	auto&& lines = expected.lines();
	for (int i = 0; i < lines.size(); ++i) {
		CAPTURE(i);
		auto&& descLine = f.line(i);
		auto&& expected = lines[i];

		CAPTURE(descLine);
		CAPTURE(expected);
		CHECK(descLine == expected);
	}

}//THEN
}//THEN
}//WHEN
}//GIVEN

}

void ParserTest::testBasicScenario(){
GIVEN("a feature file with a basic scenario"){
	string scenarioTitle = "basic scenario";
	stringstream featureContent;
	featureContent <<
R"(
Feature: parse scenarios

Scenario: )"
	<< scenarioTitle << endl;

	CAPTURE(featureContent.str());

WHEN("parsing it"){
	Feature f = parser.parse(featureContent);

THEN("the returned feature should have one scenario"){
	CHECK(f.scenarios().size() == 1);

THEN("the scenario should have the given title and a 'scenario' key"){
	const auto & scenario = f.scenario(0);
	CHECK(scenario.title() == scenarioTitle);
	CHECK(scenario.key() == "Scenario");

}//THEN
}//THEN
}//WHEN
}//GIVEN

}

void ParserTest::testScenarioWithSteps(){

GIVEN("a feature with a multisteps scenario"){
	vector<string> lines = {
	"Feature: parse multisteps scenario",
	"",
	"Scenario: multisteps scenario"};

	using Pair = pair<string,string>;
	vector<Pair> scenario_steps = {
		Pair{"Given", "some condition"},
		Pair{"When", "doing some action"},
		Pair{"Then", "a result should occur"},
		Pair{"And", "another side effect"}
	};

	stringstream content;
	content << join(lines, "\n\t") << "\n\t\t";

	for(const auto & step : scenario_steps){
		content << step.first << " " << step.second << "\n\t\t";
	 }

	CAPTURE(content.str());

WHEN("parsing this feature"){
	Feature f = parser.parse(content);

THEN("the feature scenario should contains the right number of steps"){
	CHECK(f.scenarios().size() == 1);

	const Section & scenario = f.scenario(0);

	CAPTURE(scenario.key());
	CHECK(scenario.title() == "multisteps scenario");

	auto & lines = scenario.lines();

	CHECK(lines.size() == scenario_steps.size());

THEN("each step should contain the key (first word) and the phrase"){
	for (int i = 0; i < scenario_steps.size(); ++i) {
		CAPTURE(i);
		CHECK(scenario_steps[i].first == scenario.line(i).key());
		CHECK(scenario_steps[i].second == scenario.line(i).phrase());
	}

}//THEN
}//THEN
}//WHEN
}//GIVEN

}//test


void ParserTest::testMultipleScenarios(){
GIVEN("a feature file with multiple scenarios"){
	stringstream content;
	content << "Feature: parse feature multi-scenarios\n\n";

	auto scenarios = vector<Section>{
		Section{"Scenario", "foobar"}.lines({
			{"Given", "foo"},
			{"When", "bar"},
			{"Then", "bazz"}
		}),
		Section{"Scenario", "one more"}.lines({
			{"Given", "some condition"},
			{"When", "doing some action"},
			{"When", "do other action"},
			{"Then", "a result should occur"},
			{"And", "another side effect"}
		}),
		Section{"Scenario", "minimum"}
	};

	content << join(scenarios, "\n\n") << endl;

	CAPTURE(content.str());

WHEN("parsing it"){
	Feature f = parser.parse(content);

THEN("the returned feature should contain all scenarios"){
	CHECK(f.scenarios().size() == scenarios.size());

	for (size_t i = 0; i < scenarios.size(); ++i) {
		auto&& scenario = f.scenario(i);
		CHECK(scenario.lines().size() == scenarios[i].lines().size());

		for (size_t j = 0; j < scenario.lines().size(); ++j) {
			CHECK(scenario.line(i) == scenarios[i].line(i));
		}
	}
}//THEN
}//WHEN
}//GIVEN

}

/* ******************************************** Helpers *****************************************************/

template<typename Coll>
std::string ParserTest::join(Coll & coll, const std::string & separator){
	stringstream out;
	auto begin = std::begin(coll);
	for(auto iter = begin; iter != std::end(coll); ++iter){
		if(iter != begin){
			out << separator;
		}
		out << *iter;
	}

	return std::move(out.str());
}
