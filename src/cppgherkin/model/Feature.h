#ifndef FEATURE_H
#define FEATURE_H

#include "SectionLine.h"
#include "Section.h"

#include <vector>

namespace cppgherkin {

class Scenario : public Section{
public:
	using super = Section;

	static constexpr const char * Key(){ return "Scenario";}

	Scenario(const Section & sec)
		: super(sec)
	{}
	Scenario(const std::string & title=std::string())
		: super(Key(), title)
	{}

};

class Feature : public Section{
public:
	using super = Section;
	using ScenarioList = std::vector<Scenario>;

public:
	Feature() : Feature(std::string()){}

	Feature(const std::string & title)
		: super("Feature", title)
	{}

public:
	Feature & add(const Scenario & scenario){
		return addImpl(scenario);
	}
	Feature & add(Scenario && scenario){
		return addImpl(std::move(scenario));
	}

	Feature & add(const ScenarioList& scenarios){
		return addAllImpl(scenarios);
	}

	template<typename ...Args>
	Feature & add(const Scenario& scenario, Args... scenarios){
		add(scenario);
		return add(scenarios...);
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
		this->_scenarios.emplace_back(std::forward<T>(scenario));
		return *this;
	}


	template<typename ScenariosContainer>
	Feature & addAllImpl(ScenariosContainer&& scenarios){
		for(auto&& element : scenarios){
			add(std::forward<decltype(element)>(element));
		}
		return *this;
	}
private:
	ScenarioList _scenarios;
};

inline std::ostream & operator<<(std::ostream & out, const Feature & f){
	out << (const Section &)f << std::endl;

	for (const auto& scenario : f.scenarios()) {
		out << scenario << std::endl;
	}

	return out;
}

}//namespace

#endif // FEATURE_H
