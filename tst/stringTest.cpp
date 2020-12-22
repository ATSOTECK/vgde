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

#include "stringTest.h"

void runStringTests() {
    String s = "123";
    db(s.toInt());
    s = "0xF"; //15
    db(s.toInt(String::Hex));
    s = "10110011"; //179
    db(s.toInt(String::Bin));
    
    String str = " hello个world ";
    dbq(str);
    std::string sss = "sss个";
    std::cout << sss << std::endl;
    
    db(str.endsWith("world"));
    db(str.endsWith("world "));
    db(str.endsWith("world", true));
    dbln
    db(str.startsWith("hello"));
    db(str.startsWith(" hello"));
    db(str.startsWith("hello", true));
    dbq(str);
    
    dbln
    
    dbq(str.stringAfterLast('o'));
    dbq(str.stringAfterLast('o', 'l'));
    dbq(str.stringAfterLast(' '));
    dbq(str.stringAfterLast(' ', ' '));
    dbq(str.stringAfterLast(0x4E2A)); //codepoint for 个.
    dbq(str.stringAfterLast(String::codepointFor("个")));
    
    str = " hello个world 2";
    
    dbq(str.stringAfterFirst(' '));
    dbq(str.stringAfterFirst('o'));
    dbq(str.stringAfterFirst('o', 'l'));
    dbq(str.stringAfterFirst('2'));
    dbq(str.stringAfterFirst('2', '2'));
    
    dbln
    dbq(str.stringBeforeFirst(0x4E2A));
    dbq(str.stringBeforeLast(0x4E2A));
    dbq(str.stringBeforeFirst('o'));
    dbq(str.stringBeforeLast('o'));
    dbq(str.stringBeforeFirst('o', 'l'));
    dbq(str.stringBeforeLast('o', 'l'));
}
