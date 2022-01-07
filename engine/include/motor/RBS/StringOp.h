#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <windows.h>

using namespace std;

namespace CStringOp
{
    size_t Find(const string& str, const string& sub, size_t offset = 0);
    size_t Find_Last_Of(const string& str, const string& sub);
    size_t FindAny(const string& str, const vector<string>& subs, string& foundStr);

    bool IsFindAll(const string& str, const vector<string>& subs);
    bool Split(vector<string> &arrResult, const string& strSource, const string& strSep);
    vector<string> Split(const string& strSource, const vector<string>& strsSep, const bool remove_null, const bool remove_sep);
    vector<string> getSequence(const std::string& str);
    size_t getSequenceCnt(const std::string& str);
    string tolowercase(const string& str);

    string trim(const string& str,const string& delimiter=" ");
    template <class CT> CT FromString(const string& str);
    template <class CT> string ToString(CT t);	

    long long GetNumberAfterFirstKey(const string& content, const string& key);

    long long GetNumberBeforeFirstKey(const string& content, const string& key);
    size_t GetLength(const string& str);

    std::string replaceAll(const std::string& str, const std::string& target, const std::string& replacement);

    bool startsWith(const std::string& str, const std::string& prefix);

    std::vector<std::string> split(const std::string& str, const std::string& sep);

    std::vector<std::string> split(const std::string & str, const std::vector<std::string>& seps, bool remove_sep);

    size_t lcsCnt(const std::string& origin, const std::string& suggestion);

    std::string repeat(const std::string& str, int repeatCnt);
    bool isChinese(const std::string& str);
    bool isDigit(const std::string& str);
    bool isAlpha(const std::string& str);

    string GBKToUTF8(const std::string& strGBK);

    string UTF8ToGBK(const std::string& strUTF8);

    bool GetWordAndPos(const string& sLine, vector<string>& vecWords, vector<string>& vecPos);
    void GetWordAndPos(const string& str, string& word, string& pos);

    string ReverseString(const string& str);
    bool IsPunc(const string& str);
    bool IsAllDiffChar(const string& sWord1,const string& sWord2);
    int DiffCount(const string& sWord1,const string& sWord2);
    //bool DiffChar(vector<string>& vecDiff,const string& sWord1,const string& sWord2);
    int GetPosOfDiffChr(const string& sWord1,const string& sWord2);

    bool IsNum(const string& s);
    bool IsTime(const string& s);
    bool IsYear(const string& s);

    bool IsArabNum(const string& s);
    bool IsChnNum(const string& s);
};

template <class CT> CT CStringOp::FromString(const string& str)
{
    CT t;
    istringstream istr(str);
    istr >> t;
    return t;
}

template <class CT> 
string CStringOp::ToString(CT t)
{
    ostringstream ostr;
    ostr <<setprecision(8)<<fixed<< t;
    return (string)ostr.str();
}

//template <class CT> 
//string CStringOp::ToString(CT t)
//{
//	ostringstream ostr;
//	ostr << t;
//	return (string)ostr.str();
//}

enum StringType {
    STR_EMPTY = 0,
    STR_NUM = 1,
    STR_LETTER = 2,
    STR_CHINESE = 3,
    STR_OTHER = 4,
};