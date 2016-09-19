#ifndef FILESEXCEPTION_H
#define FILESEXCEPTION_H

#include "commonex/IOException.h"

namespace cppgherkin {
namespace files {

COMMONEX_SUBCLASS(FilesException, commonex::IOException);

namespace impl {
inline void checkStatusError(int statusCode){
	if(statusCode != 0){
		throw FilesException(strerror(errno));
	}
}
}//namespace

}//namespace
}//namespace

#endif // FILESEXCEPTION_H
