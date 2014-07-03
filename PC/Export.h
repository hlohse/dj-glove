#ifndef DJ_GLOVE_PC_EXPORT_H_
#define DJ_GLOVE_PC_EXPORT_H_

#ifdef __linux__

#define EXPORT

#elif _WIN32

#define EXPORT __declspec(dllexport)
#pragma warning(disable : 4251) // Ignore not exported STL classes

#endif

#endif // DJ_GLOVE_PC_EXPORT_H_
