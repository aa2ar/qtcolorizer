/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colorgroupmodel.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "colorgroupmodel.h"
#include <QColor>
#include <QPainter>
#include <QDebug>

ColorGroupModel::ColorGroupModel(ColorSet * set, QObject * parent)
    : QAbstractListModel(parent)
    , mSet(set)
{}

const ColorTones * ColorGroupModel::getTones(int idx)
{
    if (mSet == nullptr || idx < 0 || idx >= (int)mSet->size())
        return nullptr;

    return std::get<2>(((*mSet)[idx]));
}

QVariant ColorGroupModel::data(const QModelIndex & index, int role) const
{
    if (mSet == nullptr || index.row() < 0 || index.row() >= (int)mSet->size())
        return QVariant();

    const ColorSetItem & item = mSet->at(index.row());

    switch (role) {
    case Qt::BackgroundRole:
        return QColor(std::get<1>(item).c_str());
    case Qt::ForegroundRole:
        return QColor(Qt::black);
    case Qt::DisplayRole:
        return QString(std::get<0>(item).c_str());
    }

    return QVariant();
}

int ColorGroupModel::rowCount(const QModelIndex & /*parent*/) const
{
    return mSet ? mSet->size() : 0;
}


ColorGroupModelDelegate::ColorGroupModelDelegate(int h)
    : QStyledItemDelegate()
    , mHeight(h)
{}

void ColorGroupModelDelegate::setHeight(int h)
{
    mHeight = h;
}

void ColorGroupModelDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QVariant varBg = index.model()->data(index, Qt::BackgroundRole);
    QRect r = option.rect.adjusted(0, 0, 0, -1);

    QColor bg = varBg.value<QColor>();
    painter->fillRect(option.rect, bg);

    painter->setRenderHint(QPainter::Antialiasing, true);

    int sum = bg.red() + bg.green() + bg.blue();
    QColor fg = sum < 380 ? Qt::white : Qt::black;

    painter->setBrush(fg);
    painter->setPen(fg);

    if ((option.state & QStyle::State_Selected) /*|| (option.state & QStyle::State_MouseOver)*/) {
        float sz = r.height() * 0.3f;
        float d = (r.height() - sz) / 2.0f;
        QRectF rEllipse(r.left() + d, r.top() + d, sz, sz);
        painter->drawEllipse(rEllipse);
    }

    QRect rText = r;
    rText.setLeft(r.left() + r.height());

    painter->drawText(rText, Qt::AlignLeft|Qt::AlignVCenter, index.model()->data(index, Qt::DisplayRole).toString());

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(Qt::gray);
    painter->drawLine(option.rect.left(), option.rect.bottom(), option.rect.right(), option.rect.bottom());
}

QSize ColorGroupModelDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex &) const
{
    return { option.rect.width(), mHeight };
}
