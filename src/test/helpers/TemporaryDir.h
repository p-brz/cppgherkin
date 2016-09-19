#ifndef TEMPORARYDIR_H
#define TEMPORARYDIR_H

#include <cstdio>
#include <random>

#include "cppgherkin/utils/files.h"

namespace cppgherkin {
class TemporaryDir{
public:
	static std::string randomName(size_t size);
	static std::string uniqueName(const std::string & dir= std::string(), size_t size = 16);
public:
	TemporaryDir(const std::string & baseDir = {});
	~TemporaryDir();
	TemporaryDir(const TemporaryDir & other)=delete;
	TemporaryDir(TemporaryDir && other);

	const std::string & path() const {return _path;}
	void remove();

	operator bool() const{ return _path.empty(); }
private:
	std::string _path;
};

inline TemporaryDir::TemporaryDir(const std::string & baseDir)
	: _path(uniqueName())
{
	files::mkdir(_path);
}

inline TemporaryDir::~TemporaryDir(){
	remove();
}

inline TemporaryDir::TemporaryDir(TemporaryDir && other)
	: _path(std::move(other._path))
{
	other._path.clear();
}

inline void TemporaryDir::remove(){
	if(!_path.empty()){
		files::rmdir(_path, true);
	}
}

inline std::string TemporaryDir::randomName(size_t size){
	static std::default_random_engine eng((std::random_device())());
	static std::uniform_int_distribution<char> char_distribution('a', 'z');

	std::string name;
	name.reserve(size);

	for (size_t i = 0; i < size; ++i) {
		name.push_back(char_distribution(eng));
	}

	return name;
}

inline std::string TemporaryDir::uniqueName(const std::string & dir, size_t size){
	std::string name;
	do{
		name = randomName(size);
	}while(files::exist(name));

	return name;
}

}//namespace

#endif // TEMPORARYDIR_H
