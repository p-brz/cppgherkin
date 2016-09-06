#ifndef MAKEPROPERTY_H
#define MAKEPROPERTY_H

#define MAKE_PROPERTY_GETTER(type, name)\
	const type & name() const{\
		return _##name;\
	}
#define MAKE_PROPERTY_SETTER(Class, type, name, ...)\
	template<typename T=type>\
	Class & name(T&& value){\
		_##name = value;\
		__VA_ARGS__;\
		return *this;\
	}

#define MAKE_PROPERTY(Class, type, name)\
	MAKE_PROPERTY_GETTER(type, name)\
	MAKE_PROPERTY_SETTER(Class, type, name)


#endif // MAKEPROPERTY_H
