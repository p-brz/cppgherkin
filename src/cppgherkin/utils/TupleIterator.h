#ifndef TUPLEITERATOR_H
#define TUPLEITERATOR_H

#include <tuple>

namespace cppgherkin {

template<int index, typename... Ts>
struct tuple_iterator {
	template<typename Visitor>
	void operator() (Visitor&& v, std::tuple<Ts...>& t) {
		tuple_iterator<index - 1, Ts...>{}(std::forward<Visitor>(v), t);
		v(std::get<index>(t));
	}
};

template<typename... Ts>
struct tuple_iterator<0, Ts...> {
	template<typename Visitor>
	void operator() (Visitor&& v, std::tuple<Ts...>& t) {
		v(std::get<0>(t));
	}
};

template<typename Visitor, typename... Ts>
void iterate(Visitor&& v, std::tuple<Ts...>& t) {
	const auto size = sizeof...(Ts);
	tuple_iterator<size - 1, Ts...>{}(std::forward<Visitor>(v), t);
}

}//namespace

#endif // TUPLEITERATOR_H
