/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   file.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "colortonemodel.h"
#include <QPainter>

ColorToneModel::ColorToneModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void ColorToneModel::setSource(const ColorTones * src)
{
    emit beginResetModel();
    mTones = src;
    emit endResetModel();
}

QColor ColorToneModel::getColor(int idx)
{
    if (mTones == nullptr || idx < 0 || idx >= (int)mTones->size())
        return QColor();

    const ColorTonesItem & item = mTones->operator [](idx);
    return QColor(std::get<1>(item).c_str());
}

QVariant ColorToneModel::data(const QModelIndex & index, int role) const
{
    if (mTones == nullptr || index.row() < 0 || index.row() >= (int)mTones->size())
        return QVariant();

    const ColorTonesItem & item = mTones->at(index.row());

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

int ColorToneModel::rowCount(const QModelIndex & /*parent*/) const
{
    return mTones ? mTones->size() : 0;
}


ColorToneModelDelegate::ColorToneModelDelegate(QWidget * w, const QColor & c, QObject * parent, int h)
    : QStyledItemDelegate(parent)
    , mHeight(h)
    , mWidget(w)
    , mBgColor(c)
{
    Q_ASSERT(mWidget);
}

void ColorToneModelDelegate::setHeight(int h)
{
    mHeight = h;
}

int ColorToneModelDelegate::height() const
{
    return mHeight;
}

void ColorToneModelDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QVariant varBg = index.model()->data(index, Qt::BackgroundRole);
    QRect r = option.rect.adjusted(0, 0, 0, -1);

    QColor bg = varBg.value<QColor>();
    painter->fillRect(option.rect, bg);

    if (option.state & QStyle::State_MouseOver) {
        QRect rt(r.right() - r.height()+1, r.top(), r.height(), r.height());
        QRect rImg(0, 0, 16, 16);
        int d = (rt.height() - 16) / 2;

        painter->setPen(Qt::gray);

        painter->fillRect(rt, mBgColor);
        painter->drawLine(rt.topLeft(), rt.bottomLeft());
        rImg.moveTo(rt.left()+d+1, rt.top()+d);
        painter->drawPixmap(rImg, QPixmap(":/rc/img/copy2-16x16.png"));

        r.adjust(0, 0, -r.height(), 0);
    }


    painter->setRenderHint(QPainter::Antialiasing, true);

    int sum = bg.red() + bg.green() + bg.blue();
    QColor fg = sum < 380 ? Qt::white : Qt::black;

    painter->setBrush(fg);
    painter->setPen(fg);

    if ((option.state & (QStyle::State_Selected) && (mWidget && mWidget->hasFocus())) /*|| (option.state & QStyle::State_MouseOver)*/) {
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

QSize ColorToneModelDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex &) const
{
    return { option.rect.width(), 20 };
}
