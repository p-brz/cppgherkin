#include "test.h"

#include <functional>

#include "cppgherkin/runner/runner.h"
#include "cppgherkin/model/Feature.h"
#include "cppgherkin/steps.h"

#include <fakeit.hpp>
using namespace cppgherkin;
using namespace fakeit;

#define FIXTURE RunnerTest
#define TEST_TAG "GherkinRunner"

template<typename Ret, typename ...Args>
struct Functor{
	virtual Ret operator()(Args... args)=0;
};

class RunnerTest{
public:
	template<typename ...Args>
	using MockupStep = Mock<Functor<void, Args...>>;
public:
	void testRunSimpleSteps();
	void testRunFeature();

public:
	RunnerTest(){
		StepsStorage::instance().clear();
	}

	template<typename ...Args>
	void registerMockupStep(MockupStep<Args...> & step, const std::string & text){
		Fake(Method(step, operator()));

		runner.registerStep(text, std::ref(step.get()));
	}

public:
	GherkinRunner runner;
};

FIXTURE_SCENARIO_METHOD("Runner should execute the matching steps of a given scenario"	, testRunSimpleSteps)
FIXTURE_SCENARIO_METHOD("Runner should execute all scenarios of a given feature"		, testRunFeature)

/* ************************************** Test implementation ***********************************************/

void RunnerTest::testRunSimpleSteps(){

GIVEN("some registered steps"){
	string givenStep = "some condition",
			whenStep = "doing something",
			thenStep = "a result is achieved";

	Mock<Functor<void>> mockStep;
	Fake(Method(mockStep, operator()));
	auto stepRef = std::ref(mockStep.get());

	runner.registerStep(givenStep, stepRef);
	runner.registerStep(whenStep, stepRef);
	runner.registerStep(thenStep, stepRef);

GIVEN("a scenario instance with lines that match those steps"){
	Scenario scenario{"example scenario"};
	scenario.lines({
		"Given " + givenStep,
		"When " + whenStep,
		"Then " + thenStep
	});

	CAPTURE(scenario);

WHEN("a GherkinRunner execute that feature"){
	runner.execute(scenario);

THEN("those steps should be called"){
	Verify(Method(mockStep, operator())).Exactly(3);

}//THEN
}//WHEN
}//GIVEN
}//GIVEN

}//test


void RunnerTest::testRunFeature(){

GIVEN("some registered steps"){
	MockupStep<int, int> givenTwoIntegers;
	MockupStep<> whenSumming;
	MockupStep<int> thenResult;

	registerMockupStep(givenTwoIntegers , "two integer numbers: {int} and {int}");
	registerMockupStep(whenSumming		, "summing them");
	registerMockupStep(thenResult		, "the result should be {int}");

GIVEN("a feature instance with multiple scenarios"){
	Feature feature{"Summing numbers"};
	feature.add(
		Scenario{"Sum positive numbers"}.lines({
				"Given two integer numbers: 1 and 2",
				"When summing them",
				"Then the result should be 3"
		}),
		Scenario{"Sum negative numbers"}.lines({
				"Given two integer numbers: -11 and -1",
				"When summing them",
				"Then the result should be -12"
		})
	);


WHEN("the runner executes it"){
	runner.execute(feature);

THEN("those scenarios should be executed"){
	Verify(Method(givenTwoIntegers, operator()).Using(1, 2)).Once();
	Verify(Method(givenTwoIntegers, operator()).Using(-11, -1)).Once();
	Verify(Method(whenSumming, operator())).Exactly(2);
	Verify(Method(thenResult, operator()).Using(3)).Once();
	Verify(Method(thenResult, operator()).Using(-12)).Once();

}//THEN
}//WHEN
}//GIVEN
}//GIVEN

}//test

/* ******************************************** Helpers *****************************************************/
