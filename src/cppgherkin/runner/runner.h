#ifndef RUNNER_H
#define RUNNER_H

/** Definition of classes to 'execute' a feature instance. */

#include "../model/Feature.h"
#include "cppgherkin/steps.h"

namespace cppgherkin {

class GherkinRunner{
public:
	template<typename Str, typename Fun>
	void registerStep(Str&& description, Fun&& step){
		StepsStorage::instance()
			.registerStep(std::forward<Str>(description), std::forward<Fun>(step));
	}

	void execute(const Feature & f){
		for(const auto & scenario: f.scenarios()){
			execute(scenario);
		}
	}
	void execute(const Scenario & scenario){
		for(const auto & step: scenario.lines()){
			StepsStorage::instance().executeStep(step);
		}
	}
};

}//namespace

#endif // RUNNER_H
