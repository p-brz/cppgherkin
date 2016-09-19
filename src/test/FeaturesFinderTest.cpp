#include "test.h"

#include <functional>

#include "cppgherkin/runner/FeaturesFinder.h"

#include <fakeit.hpp>
using namespace cppgherkin;
using namespace fakeit;

#define FIXTURE FeaturesFinderTest
#define TEST_TAG "[FeaturesFinder]"

constexpr const auto EXPECTED_FEATURE_FILES = {"basic.feature"};

class FeaturesFinderTest{
public:
	void testListFiles();
	void testFeaturesIterator();

public:
	std::string givenDirWithFeatures();

public:
	FeaturesFinder finder;
};


FIXTURE_SCENARIO_METHOD("FeaturesFinder can list all feature files on a directory", testListFiles)
FIXTURE_SCENARIO_METHOD("FeaturesFinder can return an iterator to the features on a dir", testFeaturesIterator)

/* ************************************** Test implementation ***********************************************/


void FeaturesFinderTest::testListFiles(){

GIVEN("a directory with some feature files"){
	string featuresDir = givenDirWithFeatures();

WHEN("passing that dir path to the FeaturesFinder"){
	auto featureFiles = finder.featureFiles(featuresDir);

THEN("it should list all feature files from that dir"){
	std::sort(begin(featureFiles), end(featureFiles));

	CHECK(featureFiles.size() == EXPECTED_FEATURE_FILES.size());

	auto filesIter = begin(featureFiles);
	auto expectedFilesIter = begin(EXPECTED_FEATURE_FILES);

	while(filesIter != end(featureFiles) && expectedFilesIter != end(EXPECTED_FEATURE_FILES)){
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
	string featuresDir = givenDirWithFeatures();

WHEN("listing the features on that dir"){
	auto features = finder.features(featuresDir);

THEN("the returned container should allow iterate over each feature of that dir"){
	FAIL("not implemented");
}//THEN
}//WHEN
}//GIVEN

}//test

/* ******************************************** Helpers *****************************************************/

string FeaturesFinderTest::givenDirWithFeatures(){
	//TODO: the tests should be independent from the running dir
	return "./src/test/data/example_features";
}

