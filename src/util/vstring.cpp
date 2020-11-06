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

#include "vstring.h"

size_t strsize(const char *str) {
    size_t size = 0;
    while (*str++ != '\0') {
        ++size;
    }
    
    return size + 1;
}

size_t utf8_strlen(const char *str) {
    if (!str) {
        return 0;
    }

    size_t c, i, l;

    for (l = 0, i = 0; i < strsize(str) - 1; ++i, ++l) {
        c = (unsigned char)str[i];
        if (c >= 0 && c <= 127) {
            i += 0;
        } else if ((c & 0xE0u) == 0xC0) {
            i += 1;
        } else if ((c & 0xF0u) == 0xE0) {
            i += 2;
        } else if ((c & 0xF8u) == 0xF0) {
            i += 3;
        } else {
            return 0;
        }
    }

    return l;
}

size_t strcmpv(const char *lhs, const char *rhs) {
    size_t ls = strsize(lhs);
    size_t rs = strsize(rhs);
    
    if (ls < rs) {
        return -1;
    } else if (rs > ls) {
        return 1;
    }
    
    size_t diff = 0;
    for (size_t i = 0; i < ls; ++i) {
        if (*lhs++ != *rhs++) {
            ++diff;
        }
    }
    
    return diff;
}

size_t strcmpcnt(const char *lhs, size_t count, const char *rhs) {
    size_t diff = 0;
    for (size_t i = 0; i < count; ++i) {
        if (*lhs++ != *rhs++) {
            ++diff;
        }
    }
    
    return diff;
}

char *cptocstr(uchar32 cp, int &size) {
    char *str = null;
    
    if (cp <= 0x7F) {
        size = 2;
        str = new char[size];
        str[0] = (char)cp;
        str[1] = '\0';
    } else if (cp <= 0x7FF) {
        size = 3;
        str = new char[size];
        str[0] = (char)(((cp >> 6u) & 0x1Fu) | 0xC0u);
        str[1] = (char)(((cp >> 0u) & 0x3Fu) | 0x80u);
        str[2] = '\0';
    } else if (cp <= 0xFFFF) {
        size = 4;
        str = new char[size];
        str[0] = (char)(((cp >> 12u) & 0x0Fu) | 0xE0u);
        str[1] = (char)(((cp >>  6u) & 0x3Fu) | 0x80u);
        str[2] = (char)(((cp >>  0u) & 0x3Fu) | 0x80u);
        str[3] = '\0';
    } else if (cp <= 0x10FFFF) {
        size = 5;
        str = new char[size];
        str[0] = (char)(((cp >> 18u) & 0x07u) | 0xF0u);
        str[1] = (char)(((cp >> 12u) & 0x3Fu) | 0x80u);
        str[2] = (char)(((cp >>  6u) & 0x3Fu) | 0x80u);
        str[3] = (char)(((cp >>  0u) & 0x3Fu) | 0x80u);
        str[4] = '\0';
    } else {
        size = 0;
    }
    
    --size;
    return str;
}

String::String() {
    init(null);
}

String::String(const char *str) {
    init(str);
}

String::String(const std::string &str) {
    init(str.c_str());
}

String::String(const String &str) {
    init(str._str);
}

String::String(int n) {
    init(std::to_string(n).c_str());
}

String::String(uint32 n) {
    init(std::to_string(n).c_str());
}

String::String(float n) {
    init(std::to_string(n).c_str());
}

String::String(double n) {
    init(std::to_string(n).c_str());
}

void String::init(const char *str) {
    _str = null;
    
    if (str != null) {
        _allocated = strsize(str);
        if (_allocated > 1) {
            _str = new char[_allocated];
            strcpy_s(_str, _allocated, str);
            _len = utf8_strlen(_str);
            _bsize = strsize(_str);
            return;
        }
    }
    
    reset();
}

void String::reset() {
    if (_str != null) {
        delete [] _str;
        _str = null;
    }
    
    reserve(1);
}

size_t String::length() const {
    return _len;
}

size_t String::size() const {
    return _allocated;
}

void String::clear() {
    reset();
}

bool String::empty() const {
    return (_len == 0);
}

size_t String::indexOf(uchar32 cp) const {
    return indexOfNext(cp, 0);
}

size_t String::indexOfNext(uchar32 cp, size_t startFrom) const {
    for (size_t i = startFrom; i < _len; ++i) {
        if (cp == codepoint(i)) {
            return i;
        }
    }
    
    return -1;
}

size_t String::indexOfLast(uchar32 cp) const {
    for (size_t i = _len - 1; i >= 0; --i) {
        if (cp == codepoint(i)) {
            return i;
        }
    }
    
    return -1;
}

bool String::contains(uchar32 cp) const {
    return (indexOfNext(cp, 0) != -1);
}

bool String::isNumber(uchar32 cp) {
    return (cp >= '0' && cp <= '9');
}

bool String::isAlpha(uchar32 cp) {
    return ((cp >= 'a' && cp <= 'z') || (cp >= 'A' && cp <= 'Z'));
}

bool String::isLower(uchar32 cp) {
    return (cp >= 'a' && cp <= 'z');
}

bool String::isUpper(uchar32 cp) {
    return (cp >= 'A' && cp <= 'Z');
}

bool String::isAlphaNumeric(uchar32 cp) {
    return (isAlpha(cp) || isNumber(cp));
}

bool String::isWhitespace(uchar32 cp) {
    return (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r');
}

bool String::startsWith(const String &str, bool ignoreWhitespace) const {
    size_t i = 0;
    size_t s = 0;
    
    if (ignoreWhitespace) {
        while (isWhitespace(codepoint(i))) {++i;}
    }
    
    return (strcmpcnt(_str + i, str._bsize - 1, str._str + s) == 0);
}

bool String::endsWith(const String &str, bool ignoreWhitespace) const {
    TODO("Skyler", "Make work.");
    return false;
    size_t i = 0;
    //size_t s = 0;
    
    if (ignoreWhitespace) {
        while (isWhitespace(codepoint(_len - i - 1))) {++i;}
        //while (isWhitespace(str[str._len - s - 1])) {++s;}
    }
    
    return (strcmpcnt(_str + (_len - 0), str._allocated - 1, str._str) == 0);
}

void String::erase(size_t index, size_t count) {
    if (_len == 0 || count == 0 || index + count > _len) {
        return;
    }
    
    size_t end = offsetForCharIndex(index + count);
    index = offsetForCharIndex(index);
    
    memcpy(_str + index, _str + end, _allocated - end);
    _str[offsetForCharIndex(_len) - ((index - end) + 1)] = '\0';
    _len = utf8_strlen(_str);
    _bsize = strsize(_str);
}

void String::trimLeadingWhitespace() {
    size_t i = 0;
    while (isWhitespace(codepoint(i)) && i <= _len) {
        ++i;
    }
    
    if (i != 0) {
        erase(0, i);
    }
}

void String::trimTrailingWhitespace() {
    size_t i = 0;
    while (isWhitespace(codepoint(_len + i - 1)) && i >= 0) {
        --i;
    }
    
    if (i != 0) {
        erase(_len + i, -i);
    }
}

void String::trimWhitespace() {
    trimLeadingWhitespace();
    trimTrailingWhitespace();
}

void String::append(uchar32 cp, size_t buffSize) {
    int cps;
    char *c = cptocstr(cp, cps);
    
    if (buffSize > 0) {
        resize(_allocated + buffSize);
    } else if (_bsize + cps > _allocated) {
        resize(_allocated + cps);
    }
    
    if (_len == 0) {
        strcpy_s(_str, cps + 1, c);
        _bsize = cps + 1;
    } else {
        strcat_s(_str, _bsize + cps, c);
        _bsize += cps;
    }
    
    ++_len;
}

void String::append(const String &string, size_t buffSize) {
    if (buffSize > 0 && buffSize > string._bsize) {
        resize(_allocated + buffSize);
    } else if (_bsize + string._bsize > _allocated) {
        resize(_allocated + string._bsize);
    }
    
    if (_len == 0) {
        strcpy_s(_str, string._bsize, string._str);
        _bsize = string._bsize;
    } else {
        strcat_s(_str, _bsize + string._bsize, string._str);
        _bsize += string._bsize;
    }
    
    _len += string._len;
}

String String::toUpper() const {
    String ret = *this;
    
    for (size_t i = 0; i < _len; ++i) {
        if (isLower(ret[i])) {
            ret._str[ret.offsetForCharIndex(i)] -= 32;
        }
    }
    
    return ret;
}

String String::toLower() const {
    String ret = *this;
    
    for (size_t i = 0; i < _len; ++i) {
        if (isUpper(ret[i])) {
            ret._str[ret.offsetForCharIndex(i)] += 32;
        }
    }
    
    return ret;
}

String String::swapCase() const {
    String ret = *this;
    
    for (size_t i = 0; i < _len; ++i) {
        uchar32 cp = ret[i];
        if (isUpper(cp)) {
            ret._str[ret.offsetForCharIndex(i)] += 32;
        } else if (isLower(cp)) {
            ret._str[ret.offsetForCharIndex(i)] -= 32;
        }
    }
    
    return ret;
}

String String::reverse() const {
    String ret;
    ret.reserve(_allocated);
    TODO("Skyler", "Make work.");
    return ret;
}

char *String::c_str() const {
    return _str;
}

std::string String::stdString() const {
    return std::string(_str);
}

int String::toInt(int base) const {
    return strtol(_str, null, base);
}

float String::toFloat() const {
    return (float)strtod(_str, null);
}

double String::toDouble() const {
    return strtod(_str, null);
}

void String::reserve(size_t size) {
    if (_str != null) {
        return;
    }
    
    _allocated = size;
    _bsize = 1;
    _len = 0;
    _str = new char[size];
    _str[0] = '\0';
}

void String::resize(size_t size) {
    if (size == 0 || size < _allocated) {
        return;
    }
    
    if (_allocated == 0) {
        _allocated = size + 1;
        _str = new char[_allocated];
        memset(_str, 0, _allocated);
        _str[0] = '\0';
        _bsize = 1;
        _len = 0;
    } else if (_allocated < size) {
        _allocated = size;
        
        char *newStr = new char[_allocated];
        
        if (_len != 0) {
            strcpy_s(newStr, _bsize, _str);
        }
        
        delete[] _str;
        _str = newStr;
        
        if (_len == 0) {
            _str[0] = '\0';
        }
    }
}

size_t String::offsetForCharIndex(size_t index) const {
    if (!_str || index > _len || index == 0) {
        return 0;
    }
    
    size_t c, i, l;
    
    for (l = 0, i = 0; i < _bsize - 1; ++i, ++l) {
        c = (unsigned char)_str[i];
        if (c >= 0 && c <= 127) {
            i += 0;
        } else if ((c & 0xE0u) == 0xC0) {
            i += 1;
        } else if ((c & 0xF0u) == 0xE0) {
            i += 2;
        } else if ((c & 0xF8u) == 0xF0) {
            i += 3;
        } else {
            return 0;
        }
        
        if (index == l + 1) {
            return i + 1;
        }
    }
    
    return 0;
}

uint32 String::codepoint(size_t index) const {
    if (index > _len) {
        return 0;
    }
    
    uint32 cp = 0;
    index = offsetForCharIndex(index);
    uint32 c = (uchar)_str[index];
    
    if ((c & 0x80u) == 0) {
        cp = (unsigned char)c;
    } else if ((c & 0xE0u) == 0xC0) {
        cp =  ((uint32)_str[index]     & 0x1Fu) << 6u;
        cp |= ((uint32)_str[index + 1] & 0x3Fu);
    } else if ((c & 0xF0u) == 0xE0) {
        cp =  ((uint32)_str[index]     & 0xFu)  << 12u;
        cp |= ((uint32)_str[index + 1] & 0x3Fu) << 6u;
        cp |= ((uint32)_str[index + 2] & 0x3Fu);
    } else if ((c & 0xF8u) == 0xF0) {
        cp =  ((uint32)_str[index]     & 0x7u)  << 18u;
        cp |= ((uint32)_str[index + 1] & 0x3Fu) << 12u;
        cp |= ((uint32)_str[index + 2] & 0x3Fu) << 6u;
        cp |= ((uint32)_str[index + 3] & 0x3Fu);
    } else if ((c & 0xFCu) == 0xF8) {
        cp =  ((uint32)_str[index]     & 0x3u)  << 24u;
        cp |= ((uint32)_str[index]     & 0x3Fu) << 18u;
        cp |= ((uint32)_str[index]     & 0x3Fu) << 12u;
        cp |= ((uint32)_str[index]     & 0x3Fu) << 6u;
        cp |= ((uint32)_str[index]     & 0x3Fu);
    } else if ((c & 0xFEu) == 0xFC) {
        cp =  ((uint32)_str[index]     & 0x1u)  << 30u;
        cp |= ((uint32)_str[index]     & 0x3Fu) << 24u;
        cp |= ((uint32)_str[index]     & 0x3Fu) << 18u;
        cp |= ((uint32)_str[index]     & 0x3Fu) << 12u;
        cp |= ((uint32)_str[index]     & 0x3Fu) << 6u;
        cp |= ((uint32)_str[index]     & 0x3Fu);
    }
    
    return cp;
}

String::operator std::string() const {
    return stdString();
}

String &String::operator =(const String &other) {
    if (this == &other) {
        return *this;
    }
    
    reset();
    init(other._str);
    return *this;
}

String &String::operator=(const char *other) {
    reset();
    init(other);
    
    return *this;
}

String &String::operator=(const std::string &other) {
    reset();
    init(other.c_str());
    
    return *this;
}

String String::operator +(const String &other) {
    String s(*this);
    s.append(other);
    
    return s;
}

String String::operator +(const char *other) {
    String s(*this);
    s.append(other);
    
    return s;
}

String String::operator+=(const String &other) {
    append(other);
    
    return *this;
}

String String::operator+=(uchar32 cp) {
    append(cp);
    
    return *this;
}

String String::operator+=(const char *other) {
    append(other);
    
    return *this;
}

String String::operator+=(const std::string &other) {
    append(other);
    
    return *this;
}

uchar32 String::operator[](size_t index) const {
    return codepoint(index);
}

/*
uchar32 &String::operator[](size_t index) {
    return codepoint(index);
}
*/

bool operator==(const String &lhs, const String &rhs) {
    if (lhs._len != rhs._len) {
        return false;
    }
    
    return (strcmpv(lhs._str, rhs._str) == 0);
}

bool operator!=(const String &lhs, const String &rhs) {
    return !(lhs == rhs);
}

bool operator==(const String &lhs, const char *rhs) {
    return (strcmpv(lhs._str, rhs) == 0);
}

bool operator!=(const String &lhs, const char *rhs) {
    return !(lhs == rhs);
}

std::ostream &operator <<(std::ostream &os, String &str) {
    os << str._str;
    return os;
}

std::ostream &operator <<(std::ostream &os, const String &str) {
    os << str._str;
    return os;
}
