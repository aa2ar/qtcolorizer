/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   widgetsetmodeldelegate.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __WIDGETSETMODELDELEGATE_H__INCLUDED__
#define __WIDGETSETMODELDELEGATE_H__INCLUDED__

#include <QStyledItemDelegate>
#include <functional>
#include "colorizermodel.h"
#include "paletteprovider.h"

class QComboBox;
class ColorizerModel;
class WidgetSetModelDelegate : public QStyledItemDelegate, public PaletteProvider
{
    Q_OBJECT

public:
    WidgetSetModelDelegate(ColorizerModel *, QComboBox *, std::function<bool(ColorizerModel::AddItem &)>, QObject * parent = 0);

    void setHeight(int);
    int height() const { return mHeight; }

    void editCurrentWidgetSet();
    void removeCurrentWidgetSet();

    // QStyledItemDelegate
    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const override;
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override;




    // PaletteProvider
    QPalette getPalette() const override;
    void     resetPalette() override;
    void     revertPalette() override;
    QColor   getColor(QPalette::ColorGroup, QPalette::ColorRole) const override;
    void     setColor(QPalette::ColorGroup, QPalette::ColorRole, const QColor &) override;
    void     resetColor(QPalette::ColorGroup, QPalette::ColorRole) override;
    void     revertColor(QPalette::ColorGroup, QPalette::ColorRole) override;
    void     resetRole(QPalette::ColorRole) override;
    void     revertRole(QPalette::ColorRole) override;
    void     resetWidgetSet() override;
    void     revertWidgetSet() override;
    void     resetAll() override;
    void     revertAll() override;
    QFont    getFont() const override;
    void     resetFont(const QFont &) override;
    void     revertFont() override;
    bool     isValid() const override;

signals:
    void widgetSetChanged(int);
    void paletteChanged();
    void fontChanged();

private:
    ColorizerModel * mModel = nullptr;
    QComboBox * mComboBox = nullptr;
    int mCurIdx = -1;
    int mHeight = 40;
    std::function<bool(ColorizerModel::AddItem &)> mFn;
};

#endif // __WIDGETSETMODELDELEGATE_H__INCLUDED__
