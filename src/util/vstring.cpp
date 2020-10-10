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
        _size = strsize(str);
        if (_size > 1) {
            _str = new char[_size];
            strcpy_s(_str, _size, str);
            _len = utf8_strlen(_str);
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
    
    _size = 1;
    _len = 0;
    _str = new char[1];
    _str[0] = '\0';
}

size_t String::length() const {
    return _len;
}

size_t String::size() const {
    return _size;
}

void String::clear() {
    reset();
}

bool String::isEmpty() const {
    return (_len != 0);
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
        //while (isWhitespace(str[s])) {++s;}
    }
    
    return (strcmpcnt(_str + i, str._size - 1, str._str + s) == 0);
}

bool String::endsWith(const String &str, bool ignoreWhitespace) const {
    return false; //TODO(Skyler): Make work.
    size_t i = 0;
    //size_t s = 0;
    
    if (ignoreWhitespace) {
        while (isWhitespace(codepoint(_len - i - 1))) {++i;}
        //while (isWhitespace(str[str._len - s - 1])) {++s;}
    }
    
    return (strcmpcnt(_str + (_len - 0), str._size - 1, str._str) == 0);
}

void String::erase(size_t index, size_t count) {
    if (_len == 0 || count == 0 || index + count > _len) {
        return;
    }
    
    size_t end = offsetForCharIndex(index + count);
    index = offsetForCharIndex(index);
    
    memcpy(_str + index, _str + end, _size - end);
    _str[offsetForCharIndex(_len) - ((index - end) + 1)] = '\0';
    _len = utf8_strlen(_str);
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

char *String::c_str() const {
    return _str;
}

std::string String::stdString() const {
    return std::string(_str);
}

size_t String::offsetForCharIndex(size_t index) const {
    if (!_str || index > _len || index == 0) {
        return 0;
    }
    
    size_t c, i, l;
    
    for (l = 0, i = 0; i < _size - 1; ++i, ++l) {
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
        cp =  ((uint32)_str[index] & 0xFu)      << 12u;
        cp |= ((uint32)_str[index + 1] & 0x3Fu) << 6u;
        cp |= ((uint32)_str[index + 2] & 0x3Fu);
    } else if ((c & 0xF8u) == 0xF0) {
        cp =  ((uint32)_str[index] & 0x7u)      << 18u;
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

uchar32 String::operator[](size_t index) const {
    return codepoint(index);
}

uchar32 String::operator[](size_t index) {
    return codepoint(index);
}

bool operator==(const String &lhs, const String &rhs) {
    if (lhs._len != rhs._len) {
        return false;
    }
    
    return (strcmpv(lhs._str, rhs._str) == 0);
}

bool operator!=(const String &lhs, const String &rhs) {
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
