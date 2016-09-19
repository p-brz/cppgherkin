#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#elif (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#define OS_POSIX
#endif

#endif // PLATFORM_H
