#include "test.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <fstream>
#include <chrono>

#include "cppgherkin/utils/files.h"

#include "helpers/TemporaryDir.h"

using namespace cppgherkin;

#define FIXTURE FileUtilsFixture
#define TEST_TAG "[files][utils][FileUtils]"

constexpr const size_t  maxFilename = 16;

class FileUtilsFixture{
public:

public:
	using PathsList = deque<string>;

	void mkfile(const std::string & fileName){
		std::ofstream outfile (fileName);
		outfile.close();
	}

	string tmpname(size_t size=maxFilename);

	string givenUnexistentFilename(){
		string name;
		do{
			name = tmpname();
		}while(files::exist(name));
		return name;
	}

	string givenEmptyDir(){
		string name = givenUnexistentFilename();
		files::mkdir(name);
		REQUIRE(files::exist(name));

		return name;
	}

	PathsList givenDirWithFiles(int numFiles=3, const std::string & baseDir=std::string()){
		PathsList files;
		return givenDirWithFiles(files, numFiles, baseDir);
	}

	PathsList & givenDirWithFiles(PathsList & files, int numFiles=3, const std::string & baseDir=std::string()){
		string dirName = tmpname();
		if(!baseDir.empty()){
			dirName = files::join(baseDir, dirName);
		}
		files::mkdir(dirName);

		files.push_back(dirName);

		for (int i = 0; i < numFiles; ++i) {
			string filename = dirName + files::separator() + tmpname();
			mkfile(filename);

			files.push_back(filename);
		}

		std::sort(files.begin() + 1, files.end());

		return files;
	}

	template<typename PathIter>
	int checkIterable(files::FilesIterable & iterable, PathIter expectedIter, PathIter end){
		int count = 0;
		CHECK(iterable.begin() != iterable.end());

//		for(auto iter = iterable.begin(); iter != iterable.end() && expectedIter != end; ++iter){
		for(const auto & elem : iterable){
			CHECK(elem == *expectedIter);
			++expectedIter;
			++count;
		}
		return count;
	}


	PathsList listPaths(files::FilesIterable & iterable){
		PathsList paths;

		for(auto iter =iterable.begin(); iter != iterable.end(); ++iter){
			paths.push_back(*iter);
		}
		return paths;
	}

	void removeFiles(const PathsList & list){
		for(auto iter = list.rbegin(); iter != list.rend(); ++iter){
			try{
				files::rmfile(*iter);
			}
			catch(...){
				CAPTURE(*iter);
				files::rmdir(*iter);
			}
		}
	}
};

FIXTURE_SCENARIO("files::exist should check if the given path exist"){
	CHECK(files::exist(".")); //current dir should exist
	CHECK(files::exist("..")); //assumes that we are not at root
	CHECK(!files::exist(tmpname())); //temporary path should not exist
}

FIXTURE_SCENARIO("create and remove empty dir"){
	auto dirName = givenUnexistentFilename();

	REQUIRE_NOTHROW(files::mkdir(dirName));
	CHECK(files::exist(dirName));

	REQUIRE_NOTHROW(files::rmdir(dirName));
	CHECK(!files::exist(dirName));
}

FIXTURE_SCENARIO("remove file"){
	auto fileName = tmpname() + ".txt";
	REQUIRE(!files::exist(fileName));

	mkfile(fileName);
	REQUIRE(files::exist(fileName));

	files::rmfile(fileName);
	REQUIRE(!files::exist(fileName));
}

FIXTURE_SCENARIO("iterate on empty directory"){
	auto emptyDir = givenEmptyDir();

	auto iterable = files::iterate(emptyDir);

	CHECK(iterable.begin() == iterable.end());

	files::rmdir(emptyDir);
}

FIXTURE_SCENARIO("iterate on directory"){
	auto dir_files = givenDirWithFiles();
	REQUIRE(dir_files.size() > 1);

	auto iterable = files::iterate(dir_files[0]);

	checkIterable(iterable, dir_files.begin() + 1, dir_files.end());

	files::removeAll(dir_files.begin() + 1, dir_files.end());
	files::rmdir(dir_files[0]);
}

FIXTURE_SCENARIO("iterate recursively"){
	auto files_list = givenDirWithFiles(1);
	auto baseDir = files_list.front();
	files_list.pop_front();

	givenDirWithFiles(files_list, 0, baseDir);

	auto iterable = files::recursive_iterate(baseDir);

	checkIterable(iterable, files_list.begin(), files_list.end());

	removeFiles(files_list);
	files::rmdir(baseDir);
}

FIXTURE_SCENARIO("iterable should list all files and dirs in for-each"){
	auto files_list = givenDirWithFiles(1);
	auto baseDir = files_list.front();

	//add children
	auto children = givenDirWithFiles(2, baseDir);
	files_list.push_back(children.front()); //add dir

	int count =0;
	auto iter= files_list.begin()+1;

	for(const auto & file: files::iterate(baseDir)){
		CHECK(file == *iter);

		++iter;
		++count;
	}

	CHECK(count == files_list.size() - 1);

	removeFiles(children);
	files_list.pop_back();

	files::rmdir(baseDir, true);
}

FIXTURE_SCENARIO("remove directory recursively"){
	auto dir_files = givenDirWithFiles();
	auto dirname = dir_files.front();

	auto children1 = givenDirWithFiles(2, dirname);
	auto children2 = givenDirWithFiles(2, children1.front());

	CHECK_NOTHROW(files::rmdir(dirname, true));
	CHECK_FALSE(files::exist(dirname));
}

FIXTURE_SCENARIO("last path segment is the filename"){

}

/* ************************************** Test implementation ***********************************************/



TEST_CASE("create dir"){


}

/* ******************************************** Helpers *****************************************************/

string FileUtilsFixture::tmpname(size_t size){
	return TemporaryDir::randomName(size);
}
