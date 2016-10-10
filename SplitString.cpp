#include "SplitString.h"


int splitString(std::wstring str, std::wstring split, std::deque < std::wstring> &strArray)
{
	strArray.clear();

	std::wstring strTemp = str; //此赋值不能少 
	int nIndex = 0; // 
	while (1)
	{
		nIndex = strTemp.find(split);
		if (nIndex != std::wstring::npos)
		{
			std::wstring addstr = strTemp.substr(0, nIndex);
			if (!addstr.empty())strArray.push_back(addstr);
			strTemp = strTemp.substr(nIndex + split.size());
		}
		else break;
	}
	if (!strTemp.empty())strArray.push_back(strTemp);
	return strArray.size();
}



int splitString(std::string str, std::string split, std::deque < std::string> &strArray)
{
	strArray.clear();

	std::string strTemp = str; //此赋值不能少 
	int nIndex = 0; // 
	while (1)
	{
		nIndex = strTemp.find(split);
		if (nIndex != std::string::npos)
		{
			std::string addstr = strTemp.substr(0, nIndex);
			if (!addstr.empty())strArray.push_back(addstr);
			strTemp = strTemp.substr(nIndex + split.size());
		}
		else break;
	}
	if (!strTemp.empty())strArray.push_back(strTemp);
	return strArray.size();
}











int splitStringHaveEmpty(std::wstring str, std::wstring split, std::deque < std::wstring> &strArray)
{
	strArray.clear();

	std::wstring strTemp = str; //此赋值不能少 
	int nIndex = 0; // 
	while (1)
	{
		nIndex = strTemp.find(split);
		if (nIndex != std::wstring::npos)
		{
			std::wstring addstr = strTemp.substr(0, nIndex);
			strArray.push_back(addstr);
			strTemp = strTemp.substr(nIndex + split.size());
		}
		else break;
	}
	strArray.push_back(strTemp);
	return strArray.size();
}



int splitStringHaveEmpty(std::string str, std::string split, std::deque < std::string> &strArray)
{
	strArray.clear();

	std::string strTemp = str; //此赋值不能少 
	int nIndex = 0; // 
	while (1)
	{
		nIndex = strTemp.find(split);
		if (nIndex != std::string::npos)
		{
			std::string addstr = strTemp.substr(0, nIndex);
			strArray.push_back(addstr);
			strTemp = strTemp.substr(nIndex + split.size());
		}
		else break;
	}
	strArray.push_back(strTemp);
	return strArray.size();
}





