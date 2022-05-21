/**
* \file   types.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __TYPES_H__INCLUDED__
#define __TYPES_H__INCLUDED__

#include <vector>
#include <tuple>
#include <string>

typedef std::pair<std::string, std::string> ColorTonesItem;
typedef std::vector<ColorTonesItem> ColorTones;

typedef std::tuple<std::string, std::string, ColorTones *> ColorSetItem;
typedef std::vector<ColorSetItem> ColorSet;

#endif // __TYPES_H__INCLUDED__

