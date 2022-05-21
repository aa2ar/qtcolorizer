/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colordb.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "colordb.h"
#include "types.h"
#include <QAbstractItemModel>

ColorTones redTones = {
    {"DarkSalmon", "#E9967A"},
    {"LightSalmon", "#FFA07A"},
};

ColorTones pinkTones = {
    {"Pink", "#FFC0CB"},
    {"LightPink", "#FFB6C1"},
    {"HotPink", "#FF69B4"},
    {"PaleVioletRed", "#DB7093"},
};

ColorTones orangeTones = {
    {"LightSalmon", "#FFA07A"},
};

ColorTones yellowTones = {
    {"LightYellow", "#FFFFE0"},
    {"LemonChiffon", "#FFFACD"},
    {"PapayaWhip", "#FFEFD5"},
    {"Moccasin", "#FFE4B5"},
    {"PeachPuff", "#FFDAB9"},
    {"PaleGoldenrod", "#EEE8AA"},
    {"Khaki", "#F0E68C"},
    {"DarkKhaki", "#BDB76B"},
};

ColorTones violetTones = {
    {"Lavender", "#E6E6FA"},
    {"Thistle", "#D8BFD8"},
    {"Plum", "#DDA0DD"},
    {"Violet", "#EE82EE"},
    {"Orchid", "#DA70D6"},
    {"MediumOrchid", "#BA55D3"},
    {"MediumPurple", "#9370DB"},
    {"SlateBlue", "#6A5ACD"},
    {"DarkSlateBlue", "#483D8B"},
};

ColorTones brownTones = {
    {"Cornsilk", "#FFF8DC"},
    {"BlanchedAlmond", "#FFEBCD"},
    {"Bisque", "#FFE4C4"},
    {"NavajoWhite", "#FFDEAD"},
    {"Wheat", "#F5DEB3"},
    {"BurlyWood", "#DEB887"},
    {"Tan", "#D2B48C"},
    {"RosyBrown", "#BC8F8F"},
    {"Peru", "#CD853F"},
    {"Sienna", "#A0522D"},
};

ColorTones greenTones = {
    {"PaleGreen",  "#98FB98"},
    {"LightGreen", "#90EE90"},
    {"MediumSeaGreen", "#3CB371"},
    {"OliveDrab", "#6B8E23"},
    {"DarkOliveGreen", "#556B2F"},
    {"DarkSeaGreen", "#8FBC8F"},
};

ColorTones blueTones = {
    {"LightCyan", "#E0FFFF"},
    {"PaleTurquoise", "#AFEEEE"},
    {"CadetBlue", "#5F9EA0"},
    {"SteelBlue", "#4682B4"},
    {"LightSteelBlue", "#B0C4DE"},
    {"PowderBlue", "#B0E0E6"},
    {"LightBlue", "#ADD8E6"},
    {"SkyBlue", "#87CEEB"},
    {"LightSkyBlue", "#87CEFA"},
    {"CornflowerBlue", "#6495ED"},
    {"RoyalBlue", "#4169E1"},
    {"MidnightBlue", "#191970"},
};

ColorTones whiteTones = {
    {"White", "#FFFFFF"},
    {"Snow", "#FFFAFA"},
    {"Honeydew", "#F0FFF0"},
    {"MintCream", "#F5FFFA"},
    {"Azure", "#F0FFFF"},
    {"AliceBlue", "#F0F8FF"},
    {"GhostWhite", "#F8F8FF"},
    {"WhiteSmoke", "#F5F5F5"},
    {"Seashell", "#FFF5EE"},
    {"Beige", "#F5F5DC"},
    {"OldLace", "#FDF5E6"},
    {"FloralWhite", "#FFFAF0"},
    {"Ivory", "#FFFFF0"},
    {"AntiqueWhite", "#FAEBD7"},
    {"Linen", "#FAF0E6"},
    {"LavenderBlush", "#FFF0F5"},
    {"MistyRose", "#FFE4E1"},
};

ColorTones grayTones = {
    {"Gainsboro", "#DCDCDC"},
    {"LightGrey", "#D3D3D3"},
    {"Silver", "#C0C0C0"},
    {"DarkGray", "#A9A9A9"},
    {"Gray", "#808080"},
    {"DimGray", "#696969"},
    {"LightSlateGray", "#778899"},
    {"SlateGray", "#708090"},
    {"DarkSlateGray", "#2F4F4F"},
    {"Black", "#000000"},
};

ColorSet colorSet = {
    std::make_tuple("Red", "#CD5C5C", &redTones),
    std::make_tuple("Pink", "#FFC0CB", &pinkTones),
    std::make_tuple("Orange", "#FFA07A", &orangeTones),
    std::make_tuple("Yellow", "#F0E68C", &yellowTones),
    std::make_tuple("Violet", "#EE82EE", &violetTones),
    std::make_tuple("Brown", "#CD853F", &brownTones),
    std::make_tuple("Green", "#90EE90", &greenTones),
    std::make_tuple("Blue", "#4682B4", &blueTones),
    std::make_tuple("White", "#FFFFFF", &whiteTones),
    std::make_tuple("Gray", "#808080", &grayTones),
};


ColorDB::ColorDB()
{
    mColorModel = new ColorGroupModel(&colorSet);
    mToneModel  = new ColorToneModel();
}

QAbstractListModel * ColorDB::colorModel() const
{
    return mColorModel;
}

QAbstractItemModel * ColorDB::tonesModel() const
{
    return mToneModel;
}

void ColorDB::setCurrentTone(int idx)
{
    const ColorTones * ct = mColorModel->getTones(idx);
    if (ct)
        mToneModel->setSource(ct);
}

QColor ColorDB::getColor(int idx)
{
    return mToneModel->getColor(idx);
}
