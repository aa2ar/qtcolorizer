/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   widgetsetmodeldelegate.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "widgetsetmodeldelegate.h"
#include "../qtcolorizer/qtcolorizer.h"
#include <QComboBox>
#include <QPainter>
#include <QFontMetricsF>
#include <QDebug>

WidgetSetModelDelegate::WidgetSetModelDelegate(ColorizerModel * m, QComboBox * cbx, std::function<bool(ColorizerModel::AddItem &)> f, QObject * parent)
    : QStyledItemDelegate(parent)
    , mModel(m)
    , mComboBox(cbx)
    , mFn(std::move(f))
{
    mHeight = int(2 * 1.15f * QFontMetricsF(mComboBox->font()).height() + 0.5f);

    connect(mComboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this, [this](int idx){
        qDebug() << "currentIndexChanged: " << idx;

        if (mModel->itemListModel()->rowCount() < 2)
            return;

        if (idx < mModel->itemListModel()->rowCount()-1) {
            mCurIdx = idx;
            emit widgetSetChanged(mCurIdx);
        }
        else {
            ColorizerModel::AddItem ai{"", "", mModel->getBasePalette(), mModel->getBaseFont()};
            bool add = mFn(ai);
            qDebug() << ai.types << ai.names;

            if (!add) {
                mComboBox->blockSignals(true);
                mComboBox->setCurrentIndex(mCurIdx);
                mComboBox->blockSignals(false);
            }
            else {
                int idx = mModel->addItem(ai.types, ai.names,
                                          ai.palette, ai.font);
                mCurIdx = idx;
                emit widgetSetChanged(mCurIdx);
            }
        }
    });
    connect(mComboBox, (void (QComboBox::*)(int))&QComboBox::activated, this, [this](int idx){
        qDebug() << "activated: " << idx;
    });
    connect(mModel, &ColorizerModel::paletteChanged, this, [this](int idx){
        if (idx == mCurIdx)
            emit paletteChanged();
    });
    connect(mModel, &ColorizerModel::fontChanged, this, [this](int idx){
        if (idx == mCurIdx)
            emit fontChanged();
    });
}

void WidgetSetModelDelegate::setHeight(int h)
{
    mHeight = h;
}

void WidgetSetModelDelegate::editCurrentWidgetSet()
{
    QPair<QString, QString> data = mModel->getItem(mCurIdx);
    ColorizerModel::AddItem ai{data.first, data.second, mModel->defaultPalette(), mModel->defaultFont()};
    bool ok = mFn(ai);
    if (ok)
        mModel->changeItem(mCurIdx, ai.types, ai.names);
}

void WidgetSetModelDelegate::removeCurrentWidgetSet()
{
    if (mModel->removeItem(mCurIdx)) {
        --mCurIdx;
        mComboBox->setCurrentIndex(mCurIdx);
        emit widgetSetChanged(mCurIdx);
    }
}

void WidgetSetModelDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    unsigned idx = (unsigned)index.row();

    if (index.row() < mModel->itemListModel()->rowCount()-1) {

        QColor c = Qt::white;
        if (option.state & QStyle::State_Selected) {
            c = mComboBox->palette().color(QPalette::Highlight);
        }
        else {
            if (idx % 2) c = c.darker(110);
        }

        painter->fillRect(option.rect, c);

        QRect r = option.rect;
        r.adjust(3, 0, -3, 0);

        QString s = mModel->itemListModel()->data(index).toString();
        painter->drawText(r, Qt::AlignVCenter|Qt::AlignLeft, s);
    }
    else {
        painter->fillRect(option.rect, Qt::black);

        QRect r = option.rect;
        r.adjust(3, 0, -3, 0);

        QString s = mModel->itemListModel()->data(index).toString();

        painter->setPen(Qt::white);
        painter->drawText(r, Qt::AlignVCenter|Qt::AlignLeft, s);
    }
}

QSize WidgetSetModelDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if (index.row() < mModel->itemListModel()->rowCount()-1)
        return {option.rect.width(), mHeight};

    return {option.rect.width(), mHeight/2};
}

// PaletteProvider
QPalette WidgetSetModelDelegate::getPalette() const
{
    return mModel->getPalette(mCurIdx);
}

void WidgetSetModelDelegate::resetPalette()
{
    mModel->resetPalette(mCurIdx);
}

void WidgetSetModelDelegate::revertPalette()
{
    mModel->revertPalette(mCurIdx);
}

QColor WidgetSetModelDelegate::getColor(QPalette::ColorGroup g, QPalette::ColorRole r) const
{
    return mModel->getColor(mCurIdx, g, r);
}

void WidgetSetModelDelegate::setColor(QPalette::ColorGroup g, QPalette::ColorRole r, const QColor & c)
{
    mModel->setColor(mCurIdx, g, r, c);
}

void WidgetSetModelDelegate::resetColor(QPalette::ColorGroup g, QPalette::ColorRole r)
{
    mModel->resetColor(mCurIdx, g, r);
}

void WidgetSetModelDelegate::revertColor(QPalette::ColorGroup g, QPalette::ColorRole r)
{
    mModel->revertColor(mCurIdx, g, r);
}

void WidgetSetModelDelegate::resetRole(QPalette::ColorRole r)
{
    mModel->resetRole(mCurIdx, r);
}

void WidgetSetModelDelegate::revertRole(QPalette::ColorRole r)
{
    mModel->revertRole(mCurIdx, r);
}

void WidgetSetModelDelegate::resetWidgetSet()
{
    mModel->resetWidgetSet(mCurIdx);
}

void WidgetSetModelDelegate::revertWidgetSet()
{
    mModel->revertWidgetSet(mCurIdx);
}

void WidgetSetModelDelegate::resetAll()
{
    mModel->resetAll();
}

void WidgetSetModelDelegate::revertAll()
{
    mModel->revertAll();
}

QFont WidgetSetModelDelegate::getFont() const
{
    return mModel->getFont(mCurIdx);
}

void WidgetSetModelDelegate::resetFont(const QFont & font)
{
    mModel->resetFont(mCurIdx, font);
}

void WidgetSetModelDelegate::revertFont()
{
    mModel->revertFont(mCurIdx);
}

bool WidgetSetModelDelegate::isValid() const
{
    return mModel->isIndexValid(mCurIdx);
}
