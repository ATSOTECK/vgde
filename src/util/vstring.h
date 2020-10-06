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

    [[nodiscard]] size_t length() const;
    [[nodiscard]] size_t size() const;
    
    void clear();
    [[nodiscard]] bool isEmpty() const;
    
    [[nodiscard]] size_t indexOf(uchar32 cp) const;
    [[nodiscard]] size_t indexOfNext(uchar32 cp, size_t startFrom) const;
    [[nodiscard]] size_t indexOfLast(uchar32 cp) const;
    [[nodiscard]] bool contains(uchar32 cp) const;
    
    static bool isNumber(uchar32 cp);
    static bool isAlpha(uchar32 cp);
    static bool isAlphaNumeric(uchar32 cp);
    static bool isWhitespace(uchar32 cp);
    
    bool startsWith(const String &str, bool ignoreWhitespace = false) const;
    bool endsWith(const String &str, bool ignoreWhitespace = false) const;
    
    void erase(size_t index, size_t count = 1);
    
    void trimLeadingWhitespace();
    void trimTrailingWhitespace();
    void trimWhitespace();
    
    [[nodiscard]] char *c_str() const;
    [[nodiscard]] std::string stdString() const;
    
    //void reserve(size_t size);
    
    [[nodiscard]] size_t offsetForCharIndex(size_t index) const;
    [[nodiscard]] uint32 codepoint(size_t index) const;

    operator std::string() const;
    
    String &operator =(const String &other);
    
    uchar32 operator[](size_t index) const;
    uchar32 operator[](size_t index);

private:
    void init(const char *str);
    void reset();
    
    friend bool operator==(const String &lhs, const String &rhs);
    
    friend std::ostream &operator <<(std::ostream &os, String &str);
    friend std::ostream &operator <<(std::ostream &os, const String &str);

    char *_str;
    size_t _size;
    size_t _len;
};

bool operator==(const String &lhs, const String &rhs);
bool operator!=(const String &lhs, const String &rhs);

std::ostream &operator <<(std::ostream &os, String &str);
std::ostream &operator <<(std::ostream &os, const String &str);

#endif //__VGDE_VSTRING_H__
