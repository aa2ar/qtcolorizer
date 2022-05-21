/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colorizermodel.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "colorizermodel.h"
#include <QAbstractListModel>
#include <QComboBox>
#include <QPainter>
#include <QDebug>


class ColorizerItemListModel : public QAbstractListModel
{
public:
    explicit ColorizerItemListModel(ColorizerModel * m, QObject * parent = 0)
        : QAbstractListModel(parent)
        , mModel(m)
    {}

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

private:
    ColorizerModel * mModel = nullptr;
};

int ColorizerItemListModel::rowCount(const QModelIndex & /*parent*/) const
{
    return mModel->mItems.size();
}

QVariant ColorizerItemListModel::data(const QModelIndex & index, int role) const
{
    return (role == Qt::DisplayRole) ? mModel->itemText(index) : QVariant();
}





////////////////////////////////////////////////////////////////////////////////////////////////////
ColorizerModel::ColorizerModel(const QPalette & pal, const QFont & font, const AddItem & ai, QObject *parent)
    : QObject(parent)
    , mDefaultPalette(pal)
    , mDefaultFont(font)
    , mDefaultTypes(ai.types)
    , mDefaultNames(ai.names)
{
    mItems.append({
                      ai.types+QStringLiteral("\n")+ai.names,
                      QtColorizer::createBaseItem(ai.types, ai.names, ai.palette, ai.font)
                  });
    mItems.append({tr("Add widget set..."), QtColorizerItem()});

    mItemListModel = new ColorizerItemListModel(this, this);
}

void ColorizerModel::setComputeDetails(bool value)
{
    mComputeDetails = value;
}

bool ColorizerModel::computeDetails() const
{
    return mComputeDetails && !mQuick;
}

void ColorizerModel::setQuick(bool value)
{
    mQuick = value;
}

bool ColorizerModel::quick() const
{
    return mQuick;
}

int ColorizerModel::addItem(const QString & types, const QString & names,
                            const QPalette & pal, const QFont & font)
{
    int idx = mItems.size()-1;
    uint32_t flags[3] = {0, 0, 0};
    mItems.insert(idx, {types+QStringLiteral("\n")+names,
                        QtColorizer::createItem(mItems[0].item,
                                              types, names, pal, font, flags, false)});
//    emit dataChanged(index(idx, 0), index(idx, 0));
    emit mItemListModel->layoutChanged();
    setModified(true);
    return idx;
}

QPair<QString, QString> ColorizerModel::getItem(int idx) const
{
    if (!isIndexValid(idx)) return QPair<QString, QString>();
    QStringList parts = mItems[idx].text.split(QStringLiteral("\n"));
    if (parts.size() != 2) return QPair<QString, QString>();
    return QPair<QString, QString>(parts[0], parts[1]);
}

bool ColorizerModel::changeItem(int idx, const QString & types, const QString & names)
{
    if (!isIndexValid(idx) || idx == 0) return false;
    mItems[idx].text = types+QStringLiteral("\n")+names;
    QtColorizerItem & item = mItems[idx].item;
    item.setTypes(types);
    item.setNames(names);
    setModified(true);
    return true;
}

bool ColorizerModel::removeItem(int idx)
{
    if (!isIndexValid(idx) || idx == 0) return false;
    mItems.remove(idx);
    setModified(true);
//    emit dataChanged(index(idx, 0), index(idx, 0));
    emit mItemListModel->layoutChanged();
    return true;
}

/////////////////////////////////////////////////////////////////////////
const QPalette & ColorizerModel::defaultPalette() const
{
    return mDefaultPalette;
}

const QFont & ColorizerModel::defaultFont() const
{
    return mDefaultFont;
}

/////////////////////////////////////////////////////////////////////////
QPalette ColorizerModel::getPalette(int idx)
{
    if (!isIndexValid(idx)) return QPalette();
    return mItems[idx].item.getPalette();
}

void ColorizerModel::resetPalette(int idx)
{
    if (!isIndexValid(idx)) return;
    mItems[idx].item.resetPalette(mDefaultPalette);
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::revertPalette(int idx)
{
    if (!isIndexValid(idx)) return;
    mItems[idx].item.resetPalette(mItems[0].item.palette());
    setModified(true);
    emit paletteChanged(idx);
}

QColor ColorizerModel::getColor(int idx, QPalette::ColorGroup g, QPalette::ColorRole r) const
{
    if (!isIndexValid(idx)) return QColor();
    return mItems[idx].item.getColor(g, r);
}

void ColorizerModel::setColor(int idx, QPalette::ColorGroup g, QPalette::ColorRole r, const QColor & c)
{
    if (!isIndexValid(idx)) return;
    if (mQuick)
        mItems[idx].item.resetPalette(makeQuickPalette(c));
    else {
        if (mComputeDetails) {
            QtColorizerItem & item = mItems[idx].item;
            QPalette pal = item.getPalette();
            QColor colors[3]{
                pal.color(QPalette::Active, r),
                pal.color(QPalette::Inactive, r),
                pal.color(QPalette::Disabled, r),
            };
            computeColorDetails(r, c, colors);
            item.setColor(QPalette::Active,   r, colors[0]);
            item.setColor(QPalette::Inactive, r, colors[1]);
            item.setColor(QPalette::Disabled, r, colors[2]);
        }
        else
            mItems[idx].item.setColor(g, r, c);
    }
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::resetColor(int idx, QPalette::ColorGroup g, QPalette::ColorRole r)
{
    QColor c = (g == QPalette::All) ? mDefaultPalette.color(r) : mDefaultPalette.color(g, r);
    mItems[idx].item.setColor(g, r, c);
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::revertColor(int idx, QPalette::ColorGroup g, QPalette::ColorRole r)
{
    if (!isIndexValid(idx)) return;
    mItems[idx].item.revertColor(g, r);
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::resetRole(int idx, QPalette::ColorRole r)
{
    if (!isIndexValid(idx)) return;
    QtColorizerItem & item = mItems[idx].item;
    QPalette pal = mDefaultPalette;
    item.setColor(QPalette::Active, r, pal.color(QPalette::Active, r));
    item.setColor(QPalette::Inactive, r, pal.color(QPalette::Inactive, r));
    item.setColor(QPalette::Disabled, r, pal.color(QPalette::Disabled, r));//FIXME:???
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::revertRole(int idx, QPalette::ColorRole r)
{
    if (!isIndexValid(idx)) return;
    QtColorizerItem & item = mItems[idx].item;
    QPalette pal = mItems[0].item.palette();
    item.setColor(QPalette::Active, r, pal.color(QPalette::Active, r));
    item.setColor(QPalette::Inactive, r, pal.color(QPalette::Inactive, r));
    item.setColor(QPalette::Disabled, r, pal.color(QPalette::Disabled, r));//FIXME:???
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::resetWidgetSet(int idx)
{
    if (!isIndexValid(idx)) return;
    mItems[idx].item.resetPalette(mDefaultPalette);
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::revertWidgetSet(int idx)
{
    if (!isIndexValid(idx)) return;
    mItems[idx].item.revertPalette();
    setModified(true);
    emit paletteChanged(idx);
}

void ColorizerModel::resetAll()
{
    mItems[0].item.resetPalette(mDefaultPalette);
    for (int idx = 1; idx < mItems.size()-1; ++idx) {
        mItems[idx].item.revertPalette();
        emit paletteChanged(idx);
    }
    setModified(true);
}

void ColorizerModel::revertAll()
{
    for (int idx = 1; idx < mItems.size()-1; ++idx) {
        mItems[idx].item.revertPalette();
        emit paletteChanged(idx);
    }
    setModified(true);
}

QFont ColorizerModel::getFont(int idx)
{
    if (!isIndexValid(idx)) return QFont();
    return mItems[idx].item.font();
}

void ColorizerModel::resetFont(int idx, const QFont & font)
{
    if (!isIndexValid(idx)) return;
    mItems[idx].item.setFont(font);
    setModified(true);
    emit fontChanged(idx);
}

void ColorizerModel::revertFont(int idx)
{
    if (!isIndexValid(idx)) return;
    mItems[idx].item.setFont(mDefaultFont);
    setModified(true);
    emit fontChanged(idx);
}

bool ColorizerModel::isIndexValid(int idx) const
{
    return (idx >= 0 && idx < mItems.size()-1);
}

bool ColorizerModel::isModified() const
{
    return mModified;
}

void ColorizerModel::clearModified()
{
    mModified = false;
    for (int i = 0; i < mItems.size()-1; ++i) {
        Item & item = mItems[i];
        item.item.reset();
    }
    emit modified(mModified);
}

void ColorizerModel::setItems(const QVector<QtColorizerItem> & items)
{
    mItems.clear();

    for (int i = 0; i < items.size(); ++i) {
        const QtColorizerItem & item = items[i];
        mItems.append({item.types()+QStringLiteral("\n")+item.names(), item});
    }
    mItems.append({tr("Add widget set..."), QtColorizerItem()});

//    emit layoutChanged();
    emit mItemListModel->layoutChanged();

    setModified(false);
}

QVector<QtColorizerItem> ColorizerModel::getItems() const
{
    QVector<QtColorizerItem> v;
    v.reserve(mItems.size()-1);
    for (int i = 0; i < mItems.size()-1; ++i)
        v.append(mItems[i].item);

    return v;
}

bool ColorizerModel::newItems()
{
    if (mModified) return false;

    mItems.clear();
    mItems.append({mDefaultTypes+QStringLiteral("\n")+mDefaultNames,
                        QtColorizer::createBaseItem(mDefaultTypes, mDefaultNames, mDefaultPalette, mDefaultFont)
                  });
    mItems.append({tr("Add widget set..."), QtColorizerItem()});

//    emit layoutChanged();
    emit mItemListModel->layoutChanged();

    setModified(false);

    return true;
}

int ColorizerModel::itemCount() const
{
    return mItems.count();
}

/////////////////////////////////////////////////////////////////////////
//int WidgetSetModel::rowCount(const QModelIndex &) const
//{
//    return mItems.size();
//}

//QVariant WidgetSetModel::data(const QModelIndex & index, int role) const
//{
//    if (index.row() < 0 || index.row() >= mItems.size())
//        return QVariant();

//    if (role == Qt::DisplayRole)
//        return mItems[index.row()].text;

//    return QVariant();
//}

QVariant ColorizerModel::itemText(const QModelIndex &index) const
{
    if (index.row() < 0 || index.row() >= mItems.size())
        return QVariant();
    return mItems[index.row()].text;
}

QAbstractListModel * ColorizerModel::itemListModel() const
{
    return mItemListModel;
}

QPalette ColorizerModel::getBasePalette() const
{
    return mItems[0].item.palette();
}

QFont ColorizerModel::getBaseFont() const
{
    return mItems[0].item.font();
}

void ColorizerModel::setModified(bool value)
{
    mModified = value;
    emit modified(mModified);
}

void ColorizerModel::computeColorDetails(QPalette::ColorRole r, const QColor & srcColor, QColor * targetColors)
{
//    QPalette::Window
//    QPalette::WindowText

//    QPalette::Button
//    QPalette::ButtonText

//    QPalette::Base
//    QPalette::AlternateBase

//    QPalette::Text
//    QPalette::BrightText

//    QPalette::Highlight
//    QPalette::HighlightedText

//    //QPalette::ToolTipBase
//    //QPalette::ToolTipText

//    //QPalette::Link
//    //QPalette::LinkVisited

    QColor cActive = targetColors[0],
           cInactive = targetColors[1],
           cDisabled = targetColors[2];

    switch (r) {
    case QPalette::WindowText:
    case QPalette::ButtonText:
    case QPalette::Text:
    case QPalette::Highlight:
        cActive = cInactive = srcColor;
        break;
    case QPalette::Window:
    case QPalette::Button:
    default:
        cActive = cInactive = cDisabled = srcColor;
    }

    targetColors[0] = cActive;
    targetColors[1] = cInactive;
    targetColors[2] = cDisabled;
}

QPalette ColorizerModel::makeQuickPalette(const QColor & c)
{
    return QPalette(c);
}

