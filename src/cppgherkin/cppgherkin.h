#ifndef CPPGHERKIN_H
#define CPPGHERKIN_H

#include "utils/StoredCallable.h"
#include "utils/TupleIterator.h"
#include "impl/Pcre2PlusMatcher.h"

#include <map>
#include <sstream>
#include <regex>
#include <list>

#include <iostream>

namespace cppgherkin {

class ExtractParameters{
public:

	ExtractParameters(const std::string & pattern, const std::string & text)
		: pattern(pattern)
		, text(text)
	{}

	template<typename T>
	void operator()(T&& value){
		if(start < text.size()){
			extract(value);
		}
	}

	template<typename T>
	void extract(T&& value){
		if(start >= pattern.size()){
			return;
		}

		size_t end = pattern.find('{', start);

		std::cout << "match: '" << pattern.substr(start, end-start) << "'" << std::endl;

		start = pattern.find('}', end) + 1;
	}

	std::string pattern;
	std::string text;
	size_t start = 0;
};

class ParametersFiller{
public:
	using ParametersHolder = std::reference_wrapper<std::istream> ;

	ParametersFiller(ParametersHolder params)
		: params(params)
	{}

	template<typename T>
	void operator()(T&& value){
		if(hasParams()){
			extract(value);
		}
	}

	template<typename T>
	void extract(T&& value){
		params.get() >> value;
	}

	bool hasParams() const{
		return params.get();
	}

	ParametersHolder params;
};

template<typename Collection=std::list<std::string>>
class ParametersFillerColl{
public:
	using ParametersHolder = std::reference_wrapper<Collection> ;

	ParametersFillerColl(ParametersHolder params)
		: params(params)
	{}

	template<typename T>
	void operator()(T&& value){
		if(hasParams()){
			extract(value);
		}
	}

	template<typename T>
	void extract(T&& value){
		extractValue(std::forward<T>(value));

		params.get().pop_front();
	}

	template<typename T>
	void extractValue(T&& value){
		std::stringstream extractor(std::move(params.get().front()));
		extractor >> value;
	}
	void extractValue(std::string& value){
		value = params.get().front();
	}

	bool hasParams() const{
		return !params.get().empty();
	}

	ParametersHolder params;
};

#define INT_PATT "[+|-]?[\\d]+"

class Step{
public:
	using RegexMatcher = impl::Pcre2PlusMatcher;

private:
	using Params = std::list<std::string>;
	using StepFun = std::function<void(Params & params)>;

public:
	Step()
	{}

	template<typename Fun>
	Step(Fun f, const std::string & pattern){
		stepFun = [f](Params & params) -> void{
			auto callable = make_callable2(f);
			ParametersFillerColl<Params> filler(params);
			callable(filler);
		};

		build_regex(pattern);
	}

	bool operator()(const std::string & text){
		return execute(text);
	}
	bool execute(const std::string & text){
		Params parameters;
		if(match(text, parameters)){
			stepFun(parameters);
			return true;
		}
		return false;
	}

private:
	bool match(const std::string & text, Params & outParams){
		auto results = re.match(text);
		if(results.empty()){
			return false;
		}

		for (auto x: results) {
			outParams.push_back(x);
		}

		return true;
	}

	//From: http://stackoverflow.com/a/3418285
	void replaceAll(std::string& str, const std::string& from, const std::string& to) {
		if(from.empty())
			return;
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	void build_regex(const std::string & pattern){
		std::string regexPattern = pattern;
		replaceAll(regexPattern, "{text}", "'([^']*)'");
		replaceAll(regexPattern, "{int}", "(" INT_PATT ")");
		replaceAll(regexPattern, "{float}", "(" INT_PATT "(?:\\.\\d+)?)");

		re.build(regexPattern);
	}

private:
	StepFun stepFun;

	RegexMatcher re;
};

class StepsStorage{
public:
	using StepMap = std::map<std::string, Step>;

public:
	static StepsStorage & instance(){
		static StepsStorage singleton;
		return singleton;
	}

	template<typename Fun>
	void registerStep(const std::string & pattern, Fun f){
		steps[pattern] = Step(f, pattern);
	}
	template<typename Ret, typename ...Args, typename Fun>
	void registerStep(const std::string & pattern, Fun f){
		steps[pattern] = Step(std::function<Ret(Args...)>(f), pattern);
	}

	void executeStep(const std::string & text){
		//TODO: how to make it more efficient?
		for(auto iter = steps.begin(); iter != steps.end(); ++iter){
			if(iter->second.execute(text)){
				return;
			}
		}
	}

	StepMap steps;
};

template<typename Fun>
class AutoRegister{
public:
	AutoRegister(Fun f, const std::string& pattern){
		StepsStorage::instance().registerStep(pattern, f);
	}
};



#define GEN_NAME(name) name##__LINE__

#define REGISTER_STEP(fn, desc)\
	static auto GEN_NAME(__auto_register__) = AutoRegister<decltype(fn)>(fn, desc);

#define INTERNAL_STEP(name, desc, ...)\
	static void name(__VA_ARGS__);\
	REGISTER_STEP(name, desc)\
	static void name(__VA_ARGS__)


#define STEP(desc, ...) INTERNAL_STEP(GEN_NAME(____GK____T_E_S_T____), desc, __VA_ARGS__)


}//namespace

#endif // CPPGHERKIN_H
