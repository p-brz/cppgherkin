#ifndef FEATURESFINDER_H
#define FEATURESFINDER_H

#include "../utils/files.h"


namespace cppgherkin {

class FeaturesContainer{

};

class FeaturesFinder{
public:

using ListPaths = std::vector<std::string>;

ListPaths featureFiles(const std::string & featuresDir){
	return files::listDir(featuresDir);
}

FeaturesContainer features(const std::string & featuresDir){
	return {};
}

};

}//namespace

#endif // FEATURESFINDER_H
