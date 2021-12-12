#ifndef __ANOLE_VERSION_HPP__
#define __ANOLE_VERSION_HPP__

#include "base.hpp"

namespace anole
{
/**
 * for a released version
 *  Version::literal will be like '0.0.16 2020/07/28'
 *
 * for a non-released version
 *  Version::literal will contain 'HEAD' before version
 *  like 'HEAD 0.0.17 2020/08/07'
 *  and '0.0.17' is the next released version
 *
 * YYYY/MM/DD is the date when the last commit is committed
*/
struct Version
{
    constexpr static auto literal = "0.0.24 2021/12/12";

    constexpr static Size major    = 0;
    constexpr static Size minor    = 0;
    constexpr static Size revision = 24;
};

/**
 * theMagic stands for version of IR
 *
 * each version of IR has an unique magic number,
 *  which is same as the time date IR is released
 *
 * YYYY'MM'DD may be followed by 'X like '1
 *  for the temporary change after the last release
*/
using Magic = Size;
inline constexpr Magic theMagic = 2021'02'13;
}

#endif
