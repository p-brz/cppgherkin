#include "test.h"

#include <functional>
#include <fstream>
#include <map>

#include <fakeit.hpp>

#include "cppgherkin/runner/FeaturesFinder.h"

#include "helpers/TemporaryDir.h"

using namespace cppgherkin;
using namespace fakeit;

#define FIXTURE FeaturesFinderTest
#define TEST_TAG "[FeaturesFinder]"

constexpr const int NUM_FEATURES = 1;
constexpr const auto FEATURE_FILES = std::array<const char *, NUM_FEATURES>{"basic.feature"};

static const auto FEATURES_MAP = [](){
	Feature features[] = {
		Feature{"basic feature"}.scenarios({
			Scenario{"basic scenario"}.lines({
				{"Given", "some condition"},
				{"When"	, "doing something"},
				{"Then"	, "a result should be met"}
			})
		})
	};

	map<string, Feature> featuresMap;
	for (int i = 0; i < NUM_FEATURES; ++i) {
		featuresMap[FEATURE_FILES[i]] = std::move(features[i]);
	}

	return featuresMap;
}();

class FeaturesFinderTest{
public:
	using FeaturesList = std::vector<Feature>;

public:
	void testListFiles();
	void testFeaturesIterator();
	void testFeaturesIteratorCache();

public:
	TemporaryDir givenDirWithFeatures();

public:
	FeaturesFinder finder;
};


FIXTURE_SCENARIO_METHOD("FeaturesFinder can list all feature files on a directory", testListFiles)
FIXTURE_SCENARIO_METHOD("FeaturesFinder can return an iterator to the features on a dir", testFeaturesIterator)
FIXTURE_SCENARIO_METHOD("FeaturesFinder iterators should share cache", testFeaturesIteratorCache)

/* ************************************** Test implementation ***********************************************/


void FeaturesFinderTest::testListFiles(){

GIVEN("a directory with some feature files"){
	TemporaryDir featuresDir = givenDirWithFeatures();

WHEN("passing that dir path to the FeaturesFinder"){
	auto featureFiles = finder.featureFiles(featuresDir.path());

THEN("it should list all feature files from that dir"){
	std::sort(begin(featureFiles), end(featureFiles));

	CHECK(featureFiles.size() == NUM_FEATURES);

	auto filesIter = begin(featureFiles);
	auto expectedFilesIter = begin(FEATURE_FILES);

	while(filesIter != end(featureFiles) && expectedFilesIter != end(FEATURE_FILES)){
		CHECK(*filesIter == *expectedFilesIter);

		++filesIter;
		++expectedFilesIter;
	}

}//THEN
}//WHEN
}//GIVEN

}//test

void FeaturesFinderTest::testFeaturesIterator(){

GIVEN("a directory with some feature files"){
	TemporaryDir featuresDir = givenDirWithFeatures();

WHEN("listing the features on that dir"){
	auto features = finder.features(featuresDir.path());

THEN("the returned container should allow iterate over each feature of that dir"){
	auto featPairIter = FEATURES_MAP.begin();

	int count = 0;
	for (const auto & feat : features) {
		CHECK(feat == featPairIter->second);

		++featPairIter;
		++count;
	}

	CHECK(count == NUM_FEATURES);

AND_THEN("the parsed features should be cached by iterator"){
	auto & cache = *features.cache();

	CHECK(cache.size() == NUM_FEATURES);

}//THEN
}//THEN
}//WHEN
}//GIVEN

}//test

void FeaturesFinderTest::testFeaturesIteratorCache(){
GIVEN("a valid features container"){
	TemporaryDir tempDir = givenDirWithFeatures();
	auto featuresIterable = finder.features(tempDir.path());

WHEN("iterating over it"){
	auto iter1 = featuresIterable.begin();
	auto iter2 = featuresIterable.begin();

THEN("the iterators should share the features cache"){
	CHECK(iter1.cache().get() == iter2.cache().get());

THEN("the features address should be the same"){
	CHECK(addressof(*iter1) == addressof(*iter2));

}//THEN
}//THEN
}//WHEN

}//GIVEN

}//test


/* ******************************************** Helpers *****************************************************/

TemporaryDir FeaturesFinderTest::givenDirWithFeatures(){
	TemporaryDir dir;

	for(auto const & featPair : FEATURES_MAP){
		ofstream out(files::join(dir.path(), featPair.first));
		out << featPair.second;
		out.close();
	}

	return dir;
}

