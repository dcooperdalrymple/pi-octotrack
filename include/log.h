#ifndef LOG_H
#define LOG_H

#include <iostream>
using namespace std;

#ifdef DEBUG
#define LOG(...) cout << __VA_ARGS__ << endl;
#else
#define LOG(...)
#endif

#endif
