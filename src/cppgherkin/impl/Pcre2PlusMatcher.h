#ifndef PCRE2PLUSMATCHER_H
#define PCRE2PLUSMATCHER_H

#include "PCRE2Plus.h"

namespace cppgherkin {
namespace impl {

class Pcre2PlusMatcher{
private:
	using re = PCRE2Plus::re;
	using RegexPtr = std::shared_ptr<re::RegexObject>;
public:
	using Results = std::vector<std::string>;

	void build(const std::string & pattern){
		regex = re::compile(pattern);
	}

	Results match(const std::string & text){
		//TODO: check if regex is valid
		auto m = regex->search(text);
		if(m){
			return m->groups();
		}
		return Results();
	}

private:
	RegexPtr regex;
};

}//namespace
}//namespace

#endif // PCRE2PLUSMATCHER_H
