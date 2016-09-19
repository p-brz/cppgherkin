#ifndef PATH_H
#define PATH_H

#include <string>

#include "../platform.h"

namespace cppgherkin {
namespace files {

#ifdef OS_WIN
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

class Path{
public:
	static constexpr char separator(){
		return PATH_SEPARATOR;
	}

public:
	Path() = default;

	Path(const std::string & path)
	{
		operator=(path);
	}

	Path & operator =(const std::string & path){
		this->_path = path;
		auto last_sep = this->_path.rfind(separator());
		auto last_segment = last_sep == std::string::npos ? 0 : last_sep + 1;

		this->_filename = std::string(path.begin() + last_segment, path.end());

		return *this;
	}

	const std::string & path() const{
		return _path;
	}
	const std::string & filename() const{
		return _filename;
	}

	operator const std::string &() const{
		return path();
	}

	bool operator==(const Path& other) const{
		return path() == other.path();
	}
	bool operator==(const std::string& otherPath) const{
		return path() == otherPath;
	}

private:
	std::string _path, _filename;
};

}//namespace
}//namespace
#endif // PATH_H
