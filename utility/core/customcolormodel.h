/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   customcolormodel.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __CUSTOMCOLORMODEL_H__INCLUDED__
#define __CUSTOMCOLORMODEL_H__INCLUDED__

#include <QAbstractListModel>
#include <QStyledItemDelegate>
#include "types.h"

class CustomColorModel : public QAbstractListModel
{
public:
    explicit CustomColorModel(QObject *parent = 0);

    void addColor(const QColor &);
    QColor getColor(int);
    void modifyColor(int, const QColor &);
    void removeColor(int);

    void load(const QString &);
    void save();

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

private:
    QVector<QColor> mList;
    QString mFilePath;
};

class CustomColorModelDelegate : public QStyledItemDelegate
{
public:
    CustomColorModelDelegate(QWidget *, const QColor &, const QFont &, const QFont &, int h = 20);

    void setHeight(int);
    int height() const;

    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const;
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

private:
    int mHeight = 20;
    QColor mBgColor;
    QFont  mColorFont;
    QFont  mAddNewFont;
    QWidget * mWidget = nullptr;
};

#endif // __CUSTOMCOLORMODEL_H__INCLUDED__
