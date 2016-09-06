#ifndef FEATURE_H
#define FEATURE_H

#include "SectionDescription.h"
#include "Section.h"

#include <vector>

namespace cppgherkin {

class Feature : public Section{
public:
	using Scenario = Section;
	using ScenarioList = std::vector<Scenario>;

public:
	Feature(){
		key("Feature");
	}

	Feature & add(const Scenario & scenario){
		return addImpl(scenario);
	}
	Feature & add(const Scenario && scenario){
		return addImpl(scenario);
	}

	const Scenario & scenario(size_t index) const{
		CHECK_BOUNDS(_scenarios, index);
		return _scenarios[index];
	}
	const ScenarioList & scenarios() const{
		return _scenarios;
	}

private:
	template<typename T>
	Feature & addImpl(T&& scenario){
		this->_scenarios.emplace_back(scenario);
		return *this;
	}

private:
	ScenarioList _scenarios;
};

std::ostream & operator<<(std::ostream & out, const Feature & f){
	out << (Section)f << std::endl;

	for (const auto& scenario : f.scenarios()) {
		out << scenario << std::endl;
	}

	return out;
}

}//namespace

#endif // FEATURE_H
