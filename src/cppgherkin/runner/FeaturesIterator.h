#ifndef FEATURESITERATOR_H
#define FEATURESITERATOR_H

#include <memory>
#include <map>
#include <string>

#include "../utils/files.h"
#include "../model/Feature.h"
#include "../parser.h"

namespace cppgherkin {

using FeaturesCache			= std::map<std::string, Feature>;
using FeaturesSharedCache	= std::shared_ptr<FeaturesCache>;

class FeaturesIterator{
public:
	FeaturesIterator() = default;

	template<typename T=files::FilesIterator>
	FeaturesIterator(T&& iter, FeaturesSharedCache featCache)
		: filesIter(std::forward<files::FilesIterator>(iter))
		, featuresCache(featCache)
	{}

	bool operator !=(const FeaturesIterator & other){
		return !(operator ==(other));
	}

	bool operator ==(const FeaturesIterator & other){
		if(isEnd() || other.isEnd()){
			return isEnd() == other.isEnd();
		}

		return featuresCache.get() == other.featuresCache.get()
				&& filesIter == other.filesIter ;
	}

	FeaturesIterator & operator++(){
		++filesIter;
		return *this;
	}
	const Feature & operator*() const{
		if(!isEnd()){
			auto path = (*filesIter).path();
			auto iter = featuresCache->find(path);

			if(iter == featuresCache->end()){
				std::ifstream input(path);
				FeatureParser parser(input);
				parser.parse();

				iter = featuresCache->insert(FeaturesCache::value_type{path, parser.feature()}).first;
			}
			return iter->second;
		}
		static const Feature & empty{};
		return empty;
	}

	operator bool() const{
		return !isEnd();
	}

	FeaturesSharedCache cache() const{
		return featuresCache;
	}

protected:
	bool isEnd() const{
		return !(bool)filesIter;
	}

private:
	files::FilesIterator filesIter;
	mutable FeaturesSharedCache featuresCache;
};

class FeaturesContainer{
public:
	FeaturesContainer(const files::Path & dirPath)
		: featureFiles(dirPath, true)
		, featuresCache(std::make_shared<FeaturesCache>())
	{}

	FeaturesIterator begin(){
		return FeaturesIterator{featureFiles.begin(), featuresCache};
	}
	const FeaturesIterator & end() const{
		static FeaturesIterator _end;
		return _end;
	}

	FeaturesSharedCache cache() const{
		return featuresCache;
	}

protected:
	files::FilesIterable featureFiles;
	FeaturesSharedCache featuresCache;
};

}//namespace

#endif // FEATURESITERATOR_H
