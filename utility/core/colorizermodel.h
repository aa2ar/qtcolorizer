/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colorizermodel.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __WIDGETSETMODEL_H__INCLUDED__
#define __WIDGETSETMODEL_H__INCLUDED__

#include <QPalette>
#include <QFont>
#include "../qtcolorizer/qtcolorizer.h"

class QAbstractListModel;
class ColorizerItemListModel;
class ColorizerModel : public QObject
{
    Q_OBJECT

    friend class ColorizerItemListModel;

public:
    struct AddItem {
        QString types;
        QString names;
        QPalette palette;
        QFont    font;
    };

public:
    explicit ColorizerModel(const QPalette &, const QFont &, const AddItem &, QObject * parent = 0);

    void setComputeDetails(bool);
    bool computeDetails() const;

    void setQuick(bool);
    bool quick() const;

    int addItem(const QString &, const QString &, const QPalette &, const QFont &);
    QPair<QString, QString> getItem(int) const;
    bool changeItem(int, const QString &, const QString &);
    bool removeItem(int);

    const QPalette & defaultPalette() const;
    const QFont    & defaultFont() const;

    QPalette getPalette(int idx);
    void     resetPalette(int idx);
    void     revertPalette(int idx);
    QColor   getColor(int idx, QPalette::ColorGroup, QPalette::ColorRole) const;
    void     setColor(int idx, QPalette::ColorGroup, QPalette::ColorRole, const QColor &);
    void     resetColor(int idx, QPalette::ColorGroup, QPalette::ColorRole);
    void     revertColor(int idx, QPalette::ColorGroup, QPalette::ColorRole);
    void     resetRole(int idx, QPalette::ColorRole);
    void     revertRole(int idx, QPalette::ColorRole);
    void     resetWidgetSet(int idx);
    void     revertWidgetSet(int idx);
    void     resetAll();
    void     revertAll();

    QFont    getFont(int idx);
    void     resetFont(int idx, const QFont &);
    void     revertFont(int idx);

    bool isIndexValid(int) const;

    bool isModified() const;
    void clearModified();

    void setItems(const QVector<QtColorizerItem> &);
    QVector<QtColorizerItem> getItems() const;
    bool newItems();

    int itemCount() const;


//    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
//    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    QAbstractListModel * itemListModel() const;

    QPalette getBasePalette() const;
    QFont    getBaseFont() const;


signals:
    void modified(bool);
    void paletteChanged(int);
    void fontChanged(int);

private:
    struct Item {
        QString text;
        QtColorizerItem item;
    };

    QVector<Item> mItems;
    QPalette mDefaultPalette;
    QFont    mDefaultFont;
    QString  mDefaultTypes;
    QString  mDefaultNames;
    bool mModified = false;
    bool mComputeDetails = true;
    bool mQuick = false;

    void setModified(bool);

    void     computeColorDetails(QPalette::ColorRole, const QColor &, QColor *);
    QPalette makeQuickPalette(const QColor &);

    ///////////////////////////////////////////////////
    ColorizerItemListModel * mItemListModel = nullptr;
    int mCurrentIndex = -1;

    QVariant itemText(const QModelIndex & index) const;

};

#endif // __WIDGETSETMODEL_H__INCLUDED__
