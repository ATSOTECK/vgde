/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020 Skyler Burwell
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#ifndef __VGDE_VSTRING_H__
#define __VGDE_VSTRING_H__

#include "config.h"

#include <iostream>
#include <locale>
#include <string>

class String {
public:
    String();
    String(const char *str);
    String(const std::string &str);
    String(const String &str);
    String(int n);
    String(uint32 n);
    String(float n);
    String(double n);
    
    keep size_t length() const;
    keep size_t size() const;
    
    void clear();
    keep bool isEmpty() const;
    
    keep size_t indexOf(uchar32 cp) const;
    keep size_t indexOfNext(uchar32 cp, size_t startFrom) const;
    keep size_t indexOfLast(uchar32 cp) const;
    keep bool contains(uchar32 cp) const;
    
    static bool isNumber(uchar32 cp);
    static bool isAlpha(uchar32 cp);
    static bool isLower(uchar32 cp);
    static bool isUpper(uchar32 cp);
    static bool isAlphaNumeric(uchar32 cp);
    static bool isWhitespace(uchar32 cp);
    
    keep bool startsWith(const String &str, bool ignoreWhitespace = false) const;
    keep bool endsWith(const String &str, bool ignoreWhitespace = false) const;
    
    String stringAfterLast(uchar32 c);
    String stringAfterLast(uchar32 c, uchar32 c1);
    
    String stringAfterFirst(uchar32 c);
    String stringAfterFirst(uchar32 c, uchar32 c1);
    
    String stringBeforeFirst(uchar32 c);
    String stringBeforeFirst(uchar32 c, uchar32 c1);
    
    String stringBeforeLast(uchar32 c);
    String stringBeforeLast(uchar32 c, uchar32 c1);
    
    void erase(size_t index, size_t count = 1);
    
    void trimLeadingWhitespace();
    void trimTrailingWhitespace();
    void trimWhitespace();
    
    void append(uchar32 c, size_t buffSize = 0);
    void append(const String &string, size_t buffSize = 0);
    void prepend(uchar32 c, size_t buffSize = 0);
    void prepend(const String &string, size_t buffSize = 0);
    
    ///Only works with ASCII.
    keep String toUpper() const;
    keep String toLower() const;
    keep String swapCase() const;
    keep String reverse() const;
    
    keep char *c_str() const;
    keep std::string stdString() const;
    
    keep int toInt() const;
    keep float toFloat() const;
    keep double toDouble() const;
    
    void reserve(size_t size);
    
    keep size_t offsetForCharIndex(size_t index) const;
    keep uint32 codepoint(size_t index) const;

    operator std::string() const;
    
    String &operator =(const String &other);
    String &operator =(const char *s);
    String &operator =(const std::string &s);
    //String &operator=(const char c);
    String operator +(const String &right);
    String operator +(const char *right);
    String operator+=(const String &right);
    String operator+=(uchar32 c);
    String operator+=(const char *c);
    String operator+=(std::string s);
    
    uchar32 operator[](size_t index) const;
   // uchar32 &operator[](size_t index);

private:
    void init(const char *str);
    void reset();
    
    friend bool operator==(const String &lhs, const String &rhs);
    friend bool operator==(const String &lhs, const char *rhs);
    
    friend std::ostream &operator <<(std::ostream &os, String &str);
    friend std::ostream &operator <<(std::ostream &os, const String &str);

    char *_str;
    size_t _size;
    size_t _len;
};

bool operator==(const String &lhs, const String &rhs);
bool operator!=(const String &lhs, const String &rhs);

String operator+(char *lhs, String rhs);
String operator+(std::string lhs, String rhs);

bool operator ==(const String &lhs, const char *rhs);
bool operator !=(const String &lhs, const char *rhs);

std::ostream &operator <<(std::ostream &os, String &str);
std::ostream &operator <<(std::ostream &os, const String &str);

#endif //__VGDE_VSTRING_H__
