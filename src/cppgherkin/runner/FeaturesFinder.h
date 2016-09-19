#ifndef FEATURESFINDER_H
#define FEATURESFINDER_H

#include <vector>
#include <string>

#include "../utils/files.h"
#include "FeaturesIterator.h"

namespace cppgherkin {

class FeaturesFinder{
public:

using ListPaths = std::vector<std::string>;

ListPaths featureFiles(const std::string & featuresDir){
	return files::listDir(featuresDir);
}

FeaturesContainer features(const std::string & featuresDir){
	return {featuresDir};
}

};

}//namespace

#endif // FEATURESFINDER_H
