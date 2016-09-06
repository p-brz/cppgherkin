#ifndef PCRE2PLUSMATCHER_H
#define PCRE2PLUSMATCHER_H

#include "PCRE2Plus.h"

namespace cppgherkin {
namespace impl {

class MatcherResults{
public:
	using MatchObj = PCRE2Plus::re::MatchObject;
	using ReResult = std::unique_ptr<MatchObj>;
	using Groups = std::vector<std::string>;

	MatcherResults()
	{}
	MatcherResults(ReResult&& result) : mResult(std::move(result))
	{}

	Groups::const_iterator begin() const{
		return groups().begin();
	}
	Groups::const_iterator end() const{
		return groups().end();
	}

	bool empty() const{
		return !mResult;
	}

	std::string & operator[](size_t index){
		return cacheGroups()[index];
	}
	const
	std::string & operator[](size_t index) const{
		return groups()[index];
	}

	operator bool() const{
		return mResult ? true : false;
	}

	operator const Groups &() const{
		return groups();
	}

	const Groups & groups() const{
		return cacheGroups();
	}

private:
	Groups & cacheGroups() const{
		if(_groups.empty() && mResult){
			_groups = mResult->groups();
		}
		return _groups;
	}

	mutable Groups _groups;
	ReResult mResult;
};

class Pcre2PlusMatcher{
private:
	using re = PCRE2Plus::re;
	using RegexPtr = std::shared_ptr<re::RegexObject>;
public:
	using Results = MatcherResults;

	Pcre2PlusMatcher(){}

	Pcre2PlusMatcher(const std::string & pattern){
		build(pattern);
	}

	void build(const std::string & pattern){
		regex = re::compile(pattern);
	}

	Results match(const std::string & text){
		//TODO: check if regex is valid
		auto m = regex->search(text);
		if(m){
			return Results(std::move(m));
		}
		return Results();
	}

private:
	RegexPtr regex;
};

}//namespace
}//namespace

#endif // PCRE2PLUSMATCHER_H
