#ifndef FILES_H
#define FILES_H

#include <vector>
#include <string>
#include <cstdio>

#include "tinydir.h"
#include "commonex/IOException.h"

#include "platform.h"

#ifdef OS_WIN
#include <direct.h>
#else
#include <unistd.h>
#endif

#include "files/FilesIterator.h"
#include "files/Path.h"

namespace cppgherkin {

namespace files {

inline constexpr char separator(){
	return Path::separator();
}

namespace impl {
inline std::string & joinPath(std::string & result, const std::string & secondSegment){
	if(result.back() != separator()){
		result.push_back(separator());
	}
	result.append(secondSegment);

	return result;
}
template<typename ...Args>
inline std::string & joinPath(std::string & firstPath, const std::string & secondSegment, Args... paths){
	return joinPath(joinPath(firstPath, secondSegment), std::forward(paths)...);
}

}//namespace

template<typename ...Args>
inline std::string join(std::string firstPath, const std::string & secondSegment, Args... paths){
	return impl::joinPath(firstPath, secondSegment, std::forward(paths)...);
}

enum class path_type{
	empty =0,
	dir = 1,
	file = 2
};

inline path_type get_type(const std::string & path){
	tinydir_file file;
	if(tinydir_file_open(&file, path.c_str()) == 0){
		return file.is_dir ? path_type::dir : path_type::file;
	}
	return path_type::empty;
}

inline bool exist(const std::string & path){
	return get_type(path) != path_type::empty;
}

inline bool is_dir(const std::string & path){
	return get_type(path) == path_type::dir;
}
inline bool is_file(const std::string & path){
	return get_type(path) == path_type::file;
}

inline void mkdir(const std::string & path, int permission=0777){
#ifdef OS_POSIX
	impl::checkStatusError(::mkdir(path.c_str(), permission));
#else
	impl::checkStatusError(::mkdir(path.c_str()));
#endif
}

void rmdir(const std::string & path, bool recursive=false);

inline void rmfile(const std::string & path){
	impl::checkStatusError(std::remove(path.c_str()));
}

inline void remove(const std::string & path, bool force=false){
	auto type = get_type(path);
	if(type == path_type::file){
		rmfile(path);
	}
	else if(type == path_type::dir){
		rmdir(path, force);
	}
}

inline void rmdir(const std::string & path, bool recursive){
	if(recursive){
		auto children = iterate(path);
		for(const auto & file : children){
			remove(file, true);
		}
	}
	impl::checkStatusError(::rmdir(path.c_str()));
}

template<typename Iter>
inline void removeAll(Iter begin, Iter end){
	while(begin != end){
		files::remove(*begin);

		++begin;
	};
}

}//namespace
}//namespace

#endif // FILES_H
