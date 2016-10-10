#ifndef SplitString_h
#define SplitString_h

#include <string>
#include <deque>

//空的不会包含到输出
extern int splitString(std::wstring str, std::wstring split, std::deque < std::wstring> &strArray);
extern int splitString(std::string str, std::string split, std::deque < std::string> &strArray);

extern int splitStringHaveEmpty(std::wstring str, std::wstring split, std::deque < std::wstring> &strArray);
extern int splitStringHaveEmpty(std::string str, std::string split, std::deque < std::string> &strArray);

#endif

