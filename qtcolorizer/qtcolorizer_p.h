/**
* \file   colorizeritem.h
* \author Alex Arefyev
* \date   29.03.2018
* 
* 
*/

#ifndef __COLORIZER_ITEM_P__INCLUDED__
#define __COLORIZER_ITEM_P__INCLUDED__

#include <QString>
#include <QPalette>
#include <QFont>
#include <QSharedPointer>

struct QtColorizerItemPrivate
{
    QString classes;
    QString names;

    QPalette palette;
    QPalette defaultPalette;

    QFont    font;
    QFont    defaultFont;

    bool paletteModified;
    bool fontModified;

    uint32_t flags[3]; // Active, inactive, Disabled

    QSharedPointer<QtColorizerItemPrivate> base;

    ~QtColorizerItemPrivate();

    QColor getColor(QPalette::ColorGroup, QPalette::ColorRole) const;
    void   setColor(QPalette::ColorGroup, QPalette::ColorRole, const QColor &);
    void   revertColor(QPalette::ColorGroup, QPalette::ColorRole);
    void   resetPalette(const QPalette &);
    void   revertPalette();
    inline bool isColorModified(int, uint32_t) const;
    bool isPaletteModified() const;
};

#endif /* __COLORIZER_ITEM_P__INCLUDED__ */
