#ifndef STOREDCALLABLE_H
#define STOREDCALLABLE_H

#include <functional>
#include "TupleIterator.h"

namespace cppgherkin {

/** Code adapted from: http://stackoverflow.com/a/32737462*/

template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };

template <typename Ret, typename ...Args>
class save_it_for_later{
public:
	using Params = std::tuple<Args...>;
	using Callable = std::function<Ret(Args...)>;

public:
	save_it_for_later(Callable c, Params p = {})
		: func(c)
		, _params(p)
	{}

	Ret operator()(){
		return callFunc(typename gens<sizeof...(Args)>::type());
	}

	template<typename ParamBuilder>
	Ret operator()(ParamBuilder&& builder){
		iterate(builder, params());
		return this->operator()();
	}

	Params & params(){ return _params;}
	const Params & params() const{ return _params;}

	Callable & callable() const{ return func;}

private:
	template<int ...S>
	Ret callFunc(seq<S...>){
		return func(std::get<S>(_params) ...);
	}

	Callable func;
	Params _params;
};


template<typename Ret, typename ...Args>
std::function<Ret()> make_callable(Ret(* f)(Args...)){
	save_it_for_later<Ret, Args...> saved = {f};

	return std::function<Ret()>(saved);
}


template<typename Ret, typename ...Args>
save_it_for_later<Ret, Args...> make_callable2(Ret(* f)(Args...)){
	return {f};
}
template<typename Ret, typename ...Args>
save_it_for_later<Ret, Args...> make_callable2(std::function<Ret(Args...)> f){
	return {f};
}

template<typename ParamBuilder, typename Ret, typename ...Args>
std::function<Ret()> make_callable(ParamBuilder paramBuilder, std::function<Ret(Args...)> f){
	return std::function<Ret()>([=](){
		ParamBuilder p = paramBuilder;
		save_it_for_later<Ret, Args...> saved = {f};
		return saved(p);
	});
}


}//namespace

#endif // STOREDCALLABLE_H
