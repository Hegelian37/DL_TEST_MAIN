#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <cstdio>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;

extern int K;
extern int N;
extern double C;

extern vector<double> gains;
extern vector<double> price;

void init();
double calcs();
void getinput();
void getinput_fix();
void getoutput();
void pricecheck();
void clearInputBuffer();
bool isValidNumber(const string& str);
vector<string> preprocessAndSplitInput(const string& input);

#endif
