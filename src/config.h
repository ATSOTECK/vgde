#ifndef __VGDE_CONFIG_H__
#define __VGDE_CONFIG_H__

#include <stdint.h>

#define null nullptr

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32   bool32;

typedef uint8_t			   uint8;
typedef unsigned char	   uchar;
typedef unsigned short	   ushort;
typedef uint16_t		   uint16;
typedef uint32_t		   uint32;
typedef unsigned int	   uint;
typedef unsigned long	   ulong;
typedef unsigned long long ullong;
typedef uint64_t		   uint64;

typedef int16  char16;
typedef int32  char32;
typedef int64  char64;
typedef uint16 uchar16;
typedef uint32 uchar32;
typedef uint64 uchar64;

typedef size_t sizeT;

#define staticCast(x, y)	  static_cast<x>(y)
#define staticCasti(x)		  static_cast<int>(x)
#define staticCasti8(x)		  static_cast<int8>(x)
#define staticCasti16(x)	  static_cast<int16>(x)
#define staticCasti32(x)	  static_cast<int32>(x)
#define staticCasti64(x)	  static_cast<int64>(x)
#define staticCastu(x)		  static_cast<uint>(x)
#define staticCastu8(x)		  static_cast<uint8>(x)
#define staticCastu16(x)	  static_cast<uint16>(x)
#define staticCastu32(x)	  static_cast<uint32>(x)
#define staticCastu64(x)	  static_cast<uint64>(x)
#define staticCastf(x)		  static_cast<float>(x)
#define staticCast_t(x)		  static_cast<size_t>(x)
#define staticCastl(x)        static_cast<long>(x)
#define staticCastul(x)		  static_cast<ulong>(x)
#define staticCastc(x)		  static_cast<char>(x)
#define dynamicCast(x, y)	  dynamic_cast<x>(y)
#define reinterpretCast(x, y) reinterpret_cast<x>(y)
#define constCast(x, y)		  const_cast<x>(y)

#define here std::cerr << "here line # " << __LINE__ << " " __FILE__ << "\n";
#define vgderr(x) std::cerr << "error " << x << " on line # " << __LINE__ << " in " __FILE__ << "\n";
#define db(x) std::cerr << x << "\n";
#define dbln() std::cerr << "\n";
#define trace(x) std::cerr << __FILE__ << ", " << __LINE__ << ": " << x << "\n";
#define tracemsg() std::cerr << __FILE__ << ", " << __LINE__ << ": "

#endif
