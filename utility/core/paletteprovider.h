/**
* \file   paletteprovider.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __PALETTEPROVIDER_H__INCLUDED__
#define __PALETTEPROVIDER_H__INCLUDED__

#include <QPalette>

struct PaletteProviderCallback
{
    virtual ~PaletteProviderCallback() = default;
    virtual void colorChanged(QPalette::ColorGroup, QPalette::ColorRole) = 0;
    virtual void paletteChanged(const QPalette &) = 0;
};

struct PaletteDataGetter
{
    virtual ~PaletteDataGetter() = default;

    virtual QPalette getPalette() const = 0;
    virtual QColor   getColor(QPalette::ColorGroup, QPalette::ColorRole) const = 0;
    virtual QFont    getFont() const = 0;
};

class QFont;
struct PaletteProvider
{
    virtual ~PaletteProvider() = default;

    virtual QPalette getPalette() const = 0;
    virtual void     resetPalette() = 0;
    virtual void     revertPalette() = 0;

    virtual QColor   getColor(QPalette::ColorGroup, QPalette::ColorRole) const = 0;
    virtual void     setColor(QPalette::ColorGroup, QPalette::ColorRole, const QColor &) = 0;
    virtual void     resetColor(QPalette::ColorGroup, QPalette::ColorRole) = 0;
    virtual void     revertColor(QPalette::ColorGroup, QPalette::ColorRole) = 0;
    virtual void     resetRole(QPalette::ColorRole) = 0;
    virtual void     revertRole(QPalette::ColorRole) = 0;
    virtual void     resetWidgetSet() = 0;
    virtual void     revertWidgetSet() = 0;
    virtual void     resetAll() = 0;
    virtual void     revertAll() = 0;

    virtual QFont    getFont() const = 0;
    virtual void     resetFont(const QFont &) = 0;
    virtual void     revertFont() = 0;

    virtual bool     isValid() const = 0;

};

#endif // __PALETTEPROVIDER_H__INCLUDED__

