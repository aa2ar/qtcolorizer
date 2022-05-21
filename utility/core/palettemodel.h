/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   palettemodel.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __COLORIZERMODEL_H__INCLUDED__
#define __COLORIZERMODEL_H__INCLUDED__

#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <tuple>

struct PaletteModelItem;
struct PaletteProvider;
class PaletteModel : public QAbstractTableModel
{
public:
    PaletteModel(PaletteProvider *, const QPalette &, QObject *parent = 0);
    ~PaletteModel();

    void setShowDetails(bool);

    void setColor(int, int, const QColor &);
    void resetColor(int, int);
    void revertColor(int, int);

    void resetRole(int);
    void revertRole(int);

    void resetWidgetSet();
    void revertWidgetSet();

    void resetAll();
    void revertAll();

    void resetPalette();
    void revertPalette();

//    std::tuple<QPalette::ColorGroup, QPalette::ColorRole> getGroupAndRole(int, int);//FIXME:unused

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    int columnCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void paletteChanged();

private:
    QVector<PaletteModelItem *> mItems;
    bool mShowDetails = true;
    QPalette mInitialPalette;

    QColor mHeaderColor1;
    QColor mHeaderColor2;

    // !!!
    PaletteProvider * mPaletteProvider = nullptr;

    QPalette::ColorGroup getGroup(int) const;
    QColor getColor(int, QPalette::ColorRole) const;
//    QColor getColor(int, int) const;//FIXME:unused

    // !!!
    void setColor(QPalette::ColorGroup acg, QPalette::ColorRole acr, const QColor &acolor);
//    void setPalette(const QPalette &); // unused ???
};

class ColorizerModelDelegate : public QStyledItemDelegate
{
    enum { RowHeight = 20 };
public:
    void paint(QPainter *, const QStyleOptionViewItem &, const QModelIndex &) const;
    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;
};

#endif // __COLORIZERMODEL_H__INCLUDED__
