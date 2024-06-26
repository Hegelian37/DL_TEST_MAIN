#include "header.h"

bool isValidNumber(const string& str)
{
    if (str.empty()) return false;
    for (char ch : str)
    {
        if (!isdigit(ch)) return false;
    }
    return true;
}

string preprocessInput(const string& input)
{
    string result;
    for (char ch : input)
    {
        if (isdigit(ch) || ch == '-')
        {
            result += ch;
        }
        else
        {
            result += ' ';
        }
    }
    return result;
}
