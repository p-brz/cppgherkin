#ifndef FILESITERATOR_H
#define FILESITERATOR_H

#include <vector>
#include <string>
#include <cstdio>
#include <deque>
#include <memory>

#include "tinydir.h"

#include "FilesException.h"
#include "Path.h"

namespace cppgherkin {
namespace files {


class FilesIterator{
protected:
	using dir_ptr = std::unique_ptr<tinydir_dir>;
	using file_ptr = std::unique_ptr<tinydir_file>;
	using DirsList = std::deque<std::string>;

public:

	FilesIterator(bool recursive=false)
		: recursive(recursive)
		, dir_counter(0)
	{
	}

	FilesIterator(const std::string & baseDir, bool recursive=false)
		: FilesIterator(recursive)
	{
		current_file = file_ptr(new tinydir_file{});
		openDir(baseDir.c_str());

		advance();
	}

	FilesIterator(const FilesIterator & other)
		: FilesIterator(other.recursive)
	{
		if(!other.isEnd()){
			this->nextDirs = other.nextDirs;

			openFile(other.current_file->path);
			openDir(other.current_dir->path);

			this->dir_counter = other.dir_counter;
		}
	}

	FilesIterator(FilesIterator && other) = default;

	bool operator!=(const FilesIterator & other) const{
		return !operator ==(other);
	}
	bool operator==(const FilesIterator & other) const{
		if(isEnd()){
			return other.isEnd();
		}
		else if(other.isEnd()){
			return false;
		}

		return recursive == other.recursive
				&& (dir_counter == other.dir_counter)
				&& (nextDirs == other.nextDirs);
	}

	FilesIterator& operator++(){
		return advance();
	}

	FilesIterator& advance(){
		if(!isEnd()){
			if(!next()){
				current_file.release();
				current_dir.release();
			}
		}

		return *this;
	}

	Path operator*(){
		return isEnd() ? Path() : Path{current_file->path};
	}

protected:

	bool isEnd() const{
		return (!current_file || !current_dir);
	}

	bool next(){
		return nextFile() || nextDir();
	}

	bool nextFile(){
		while(hasNext()){
			readFile();

			//ignore hidden files, current dir and parent dir
			if(current_file->name[0] == '.'){
				continue;
			}

			if(current_file->is_dir){
				//queue dir to be handled after files
				pushDir(current_file->path);
				continue;
			}

			return true;
		}

		return false;
	}

	bool nextDir(){
		if(!nextDirs.empty()){

			if(recursive){
				openDir(nextDirs.front().c_str());
			}

			openFile(nextDirs.front().c_str());
			nextDirs.pop_front();
			return true;
		}

		return false;
	}

	void pushDir(const char * dirPath){
		nextDirs.push_back({dirPath});
	}

	bool hasNext() const{
		return current_dir && dir_counter < current_dir->n_files;
	}

	void openDir(const char * path){
		if(!current_dir){
			current_dir = dir_ptr(new tinydir_dir{});
		}
		impl::checkStatusError(tinydir_open_sorted(current_dir.get(), path));
	}

	void releaseDir()
	{
		if(current_dir){
			tinydir_close(current_dir.get());
			current_dir.release();
		}
		dir_counter = 0;
	}

	void openFile(const char * path){
		if(!current_file){
			current_file = file_ptr(new tinydir_file{});
		}
		impl::checkStatusError(tinydir_file_open(current_file.get(), path));
	}

	void readFile(){
		tinydir_readfile_n(current_dir.get(), current_file.get(), dir_counter);
		++dir_counter;
	}

private:
	bool recursive = false;
	int dir_counter = 0;
	DirsList nextDirs;

	dir_ptr current_dir;
	file_ptr current_file;
};

class FilesIterable{
public:
	FilesIterable(const std::string & dir, bool recursive=false)
		: baseDir(dir)
		, recursive(recursive)
	{}

	FilesIterator begin(){
		return FilesIterator(baseDir, recursive);
	}

	const FilesIterator & end(){
		static const FilesIterator _end = {};
		return _end;
	}


private:
	std::string baseDir;
	bool recursive;
};

inline FilesIterable iterate(const std::string & rootDir, bool recursive=false){
	return {rootDir, recursive};
}
inline FilesIterable recursive_iterate(const std::string & rootDir){
	return iterate(rootDir, true);
}

inline std::vector<std::string> listDir(const std::string & path, bool recursive=false){
	std::vector<std::string> paths;

	tinydir_dir dir;

	tinydir_open(&dir, path.c_str());

	while (dir.has_next){
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if(file.name[0] != '.'){
			paths.push_back(file.name);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	return paths;
}

}//namespace
}//namespace

#endif // FILESITERATOR_H
