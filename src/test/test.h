#ifndef TEST_H
#define TEST_H

#include "catch.hpp"

using namespace std;

#define FIXTURE_SCENARIO(msg, ...) \
		SCENARIO_METHOD(FIXTURE, msg, TEST_TAG __VA_ARGS__)

#define FIXTURE_SCENARIO_METHOD(msg, method, ...) \
		FIXTURE_SCENARIO(msg, __VA_ARGS__){\
			method(); \
		}

#define TEST_METHOD FIXTURE_SCENARIO_METHOD

#endif // TEST_H
