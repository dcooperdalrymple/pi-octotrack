/*
   Chameleon.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#include "chameleon.h"

Chameleon::Chameleon(string const& value) {
    value_ = value;
}

Chameleon::Chameleon(const char* c) {
    value_ = c;
}

Chameleon::Chameleon(int32_t d) {
    stringstream s;
    s << d;
    value_ = s.str();
}

Chameleon::Chameleon(Chameleon const& other) {
    value_ = other.value_;
}

Chameleon& Chameleon::operator=(Chameleon const& other) {
    value_ = other.value_;
    return *this;
}

Chameleon& Chameleon::operator=(int32_t i) {
    stringstream s;
    s << i;
    value_ = s.str();
    return *this;
}

Chameleon& Chameleon::operator=(string const& s) {
    value_ = s;
    return *this;
}

Chameleon::operator string() const {
    return value_;
}

Chameleon::operator int32_t() const {
    return atof(value_.c_str());
}

const char* Chameleon::c_str() const {
    return value_.c_str();
}
