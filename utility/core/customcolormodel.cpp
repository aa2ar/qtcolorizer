/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   customcolormodel.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "customcolormodel.h"
#include <QColor>
#include <QPainter>
#include <QFile>
#include <QDebug>

CustomColorModel::CustomColorModel(QObject * parent)
    : QAbstractListModel(parent)
{
    mList.append(QColor());
}

void CustomColorModel::addColor(const QColor & c)
{
    mList.last() = c;
    mList.append(QColor());
    emit dataChanged(index(mList.size()-2, 0), index(mList.size()-1, 0));
}

QColor CustomColorModel::getColor(int idx)
{
    return (idx < 0 || idx >= mList.size()-1) ? QColor() : mList[idx];
}

void CustomColorModel::modifyColor(int idx, const QColor & c)
{
    if (idx < 0 || idx >= mList.size() - 1)
        return;

    mList[idx] = c;
    emit dataChanged(index(idx, 0), index(idx, 0));
}

void CustomColorModel::removeColor(int idx)
{
    if (idx < 0 || idx >= mList.size() - 1)
        return;

    mList.remove(idx);
    emit dataChanged(index(idx, 0), index(idx, 0));
}

void CustomColorModel::load(const QString & filePath)
{
    mFilePath = filePath;
    QFile file(mFilePath);
    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        mList.clear();

        QTextStream ts(&file);
        QString line;
        while (!(line = ts.readLine()).isNull()) {
            QColor c(line);
            if (c.isValid())
                mList.append(c);
        }

        mList.append(QColor());
    }
}

void CustomColorModel::save()
{
    QFile file(mFilePath);
    if (file.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text)) {
        QTextStream ts(&file);
        for (const QColor & c: mList)
            if (c.isValid())
                ts << c.name() + "\n";
    }
}

QVariant CustomColorModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() >= mList.size())
        return QVariant();

    const QColor & c = mList[index.row()];

    switch (role) {
    case Qt::BackgroundRole:
        return c;
    case Qt::ForegroundRole:
        return QColor(Qt::black);
    case Qt::DisplayRole:
        return c.name();
    }

    return QVariant();
}

int CustomColorModel::rowCount(const QModelIndex & /*parent*/) const
{
    return mList.size();
}


CustomColorModelDelegate::CustomColorModelDelegate(QWidget * w, const QColor & c, const QFont & f1, const QFont & f2, int h)
    : QStyledItemDelegate()
    , mHeight(h)
    , mBgColor(c)
    , mColorFont(f1)
    , mAddNewFont(f2)
    , mWidget(w)
{
    Q_ASSERT(mWidget);
}

void CustomColorModelDelegate::setHeight(int h)
{
    mHeight = h;
}

int CustomColorModelDelegate::height() const
{
    return mHeight;
}

void CustomColorModelDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QVariant varBg = index.model()->data(index, Qt::BackgroundRole);
    QRect r = option.rect.adjusted(0, 0, 0, -1);

    QColor bg = varBg.value<QColor>();

    if (bg.isValid()) {

        painter->fillRect(option.rect, bg);

        if (option.state & QStyle::State_MouseOver) {
            QRect rt(r.right() - r.height()+1, r.top(), r.height(), r.height());
            QRect rImg(0, 0, 16, 16);
            int d = (rt.height() - 16) / 2;

            painter->setPen(Qt::gray);

            painter->fillRect(rt, mBgColor);
            painter->drawLine(rt.topLeft(), rt.bottomLeft());
            rImg.moveTo(rt.left()+d+1, rt.top()+d);
            painter->drawPixmap(rImg, QPixmap(":/rc/img/close16x16.png"));

            r.adjust(0, 0, -r.height(), 0);



            rt.translate(-rt.height(), 0);

            painter->fillRect(rt, mBgColor);
            painter->drawLine(rt.topLeft(), rt.bottomLeft());
            rImg.moveTo(rt.left()+d+1, rt.top()+d);
            painter->drawPixmap(rImg, QPixmap(":/rc/img/wrench16x16.png"));

            r.adjust(0, 0, -r.height(), 0);



            rt.translate(-rt.height(), 0);

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

        if (option.state & QStyle::State_Selected && mWidget->hasFocus()) {
            qreal sz = r.height() * 0.3;
            qreal d = (r.height() - sz) / 2.0;
            QRectF rEllipse(r.left() + d, r.top() + d, sz, sz);
            painter->drawEllipse(rEllipse);
        }

        QRect rText = r;
        rText.setLeft(r.left() + r.height());

        painter->setFont(mColorFont);
        painter->drawText(rText, Qt::AlignLeft|Qt::AlignVCenter, index.model()->data(index, Qt::DisplayRole).toString());

        painter->setRenderHint(QPainter::Antialiasing, false);
    }
    else {
        QRect rText = r;
        rText.setLeft(r.left() + 2);

        painter->setFont(mAddNewFont);
        painter->setRenderHint(QPainter::TextAntialiasing, true);
        painter->setPen(Qt::black);
        painter->drawText(rText, Qt::AlignLeft|Qt::AlignVCenter, tr("Click to add color..."));
    }

    painter->setPen(Qt::gray);
    painter->drawLine(option.rect.left(), option.rect.bottom(), option.rect.right(), option.rect.bottom());
}

QSize CustomColorModelDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex &) const
{
    return { option.rect.width(), mHeight };
}
