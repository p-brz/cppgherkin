#ifndef STOREDCALLABLE_H
#define STOREDCALLABLE_H

#include <functional>
#include <regex>
#include "TupleIterator.h"

#include "function_traits.hpp"


namespace fn_traits {
	//Extending function traits to support reference_wrapper
	template<typename T>
	struct function_traits<std::reference_wrapper<T>>
		: public function_traits<fn_result_of<decltype(&std::reference_wrapper<T>::get)>>
	{};
}//namespace

namespace cppgherkin {

/** Code adapted from: http://stackoverflow.com/a/32737462*/

template<int ...> struct seq {};

template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };


template<typename Callable>
class StoredCallable{
public:
	typedef fn_traits::function_traits<Callable> func_traits;
	typedef typename func_traits::tuple_type StoredParams;
	typedef typename func_traits::return_type return_type;

	StoredCallable(Callable c, StoredParams p = {})
		: func(c)
		, _params(p)
	{}

	return_type operator()(){
		return callFunc(typename gens<func_traits::arity>::type());
	}

	template<typename ParamBuilder>
	return_type operator()(ParamBuilder&& builder){
		iterate(builder, params());
		return this->operator()();
	}

	StoredParams & params(){ return _params;}
	const StoredParams & params() const{ return _params;}

	Callable & callable() const{ return func;}

private:
	template<int ...S>
	return_type callFunc(seq<S...>){
		return func(std::get<S>(_params) ...);
	}

	Callable func;
	StoredParams _params;
};

template<typename Callable>
StoredCallable<Callable> makeStoredCallable(Callable c){
	return {c};
}

}//namespace

#endif // STOREDCALLABLE_H
