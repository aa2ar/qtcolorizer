/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   palettemodel.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "palettemodel.h"
#include "paletteprovider.h"
#include <QPainter>

struct PaletteModelItem
{
    QString titleText;
    QColor  titleBg;
    QPalette::ColorRole role;
};

QColor getBorderColor(const QColor & c)
{
    int sum = c.red() + c.green() + c.blue();
    return (sum < 380) ? Qt::white : Qt::black;
}

PaletteModel::PaletteModel(PaletteProvider * pp, const QPalette & pal, QObject * parent)
    : QAbstractTableModel(parent)
    , mPaletteProvider(pp)
{
    QColor c = pal.color(QPalette::Button);
    mHeaderColor1 = c.darker(110);
    mHeaderColor2 = c.lighter(110);

    mItems.append(new PaletteModelItem{"Window background", mHeaderColor2, QPalette::Window});
    mItems.append(new PaletteModelItem{"Window text",       mHeaderColor2, QPalette::WindowText});

    mItems.append(new PaletteModelItem{"Button background", mHeaderColor1, QPalette::Button});
    mItems.append(new PaletteModelItem{"Button text",       mHeaderColor1, QPalette::ButtonText});

    mItems.append(new PaletteModelItem{"Base",              mHeaderColor2, QPalette::Base});
    mItems.append(new PaletteModelItem{"Alternate base",    mHeaderColor2, QPalette::AlternateBase});

    mItems.append(new PaletteModelItem{"Text",              mHeaderColor1, QPalette::Text});
    mItems.append(new PaletteModelItem{"BrightText",        mHeaderColor1, QPalette::BrightText});

    mItems.append(new PaletteModelItem{"Highlight",         mHeaderColor2, QPalette::Highlight});
    mItems.append(new PaletteModelItem{"Highlighted text",  mHeaderColor2, QPalette::HighlightedText});

    mItems.append(new PaletteModelItem{"Tooltip background",mHeaderColor1, QPalette::ToolTipBase});
    mItems.append(new PaletteModelItem{"Tooltip text",      mHeaderColor1, QPalette::ToolTipText});

    mItems.append(new PaletteModelItem{"Link",              mHeaderColor2, QPalette::Link});
    mItems.append(new PaletteModelItem{"Link visited",      mHeaderColor2, QPalette::LinkVisited});

    // no effect on Kubuntu
//    mItems.append(new PaletteModelItem{"Light",             mHeaderColor1, QPalette::Light});
//    mItems.append(new PaletteModelItem{"Midlight",          mHeaderColor1, QPalette::Midlight});
//    mItems.append(new PaletteModelItem{"Mid",               mHeaderColor1, QPalette::Mid});
//    mItems.append(new PaletteModelItem{"Dark",              mHeaderColor1, QPalette::Dark});
//    mItems.append(new PaletteModelItem{"Shadow",            mHeaderColor1, QPalette::Shadow});
}

PaletteModel::~PaletteModel()
{
    for (auto i: mItems) delete i;
}

void PaletteModel::setShowDetails(bool value)
{
    mShowDetails = value;
    emit layoutChanged();
}

void PaletteModel::setColor(int grpIdx, int roleIdx, const QColor & c)
{
    if (roleIdx < 0 || roleIdx >= mItems.count()) return;

    QPalette::ColorGroup grp = getGroup(grpIdx);
    QPalette::ColorRole role = mItems[roleIdx]->role;

    mPaletteProvider->setColor(grp, role, c);

    emit dataChanged(index(roleIdx, 0), index(roleIdx, 2));
}

void PaletteModel::resetColor(int grpIdx, int roleIdx)
{
    if (roleIdx < 0 || roleIdx >= mItems.count()) return;

    QPalette::ColorGroup grp = getGroup(grpIdx);
    QPalette::ColorRole role = mItems[roleIdx]->role;

    mPaletteProvider->resetColor(grp, role);

    emit dataChanged(index(roleIdx, 0), index(roleIdx, 2));
}

void PaletteModel::revertColor(int grpIdx, int roleIdx)
{
    if (roleIdx < 0 || roleIdx >= mItems.count()) return;

    QPalette::ColorGroup grp = getGroup(grpIdx);
    QPalette::ColorRole role = mItems[roleIdx]->role;

    mPaletteProvider->revertColor(grp, role);

    emit dataChanged(index(roleIdx, 0), index(roleIdx, 2));
}

void PaletteModel::resetRole(int roleIdx)
{
    if (roleIdx < 0 || roleIdx >= mItems.count()) return;

    QPalette::ColorRole role = mItems[roleIdx]->role;

    mPaletteProvider->resetRole(role);

    emit dataChanged(index(roleIdx, 0), index(roleIdx, 2));
}

void PaletteModel::revertRole(int roleIdx)
{
    if (roleIdx < 0 || roleIdx >= mItems.count()) return;

    QPalette::ColorRole role = mItems[roleIdx]->role;

    mPaletteProvider->revertRole(role);

    emit dataChanged(index(roleIdx, 0), index(roleIdx, 2));
}

void PaletteModel::resetWidgetSet()
{
    mPaletteProvider->resetWidgetSet();
}

void PaletteModel::revertWidgetSet()
{
    mPaletteProvider->revertWidgetSet();
}

void PaletteModel::resetAll()
{
    mPaletteProvider->resetAll();
}

void PaletteModel::revertAll()
{
    mPaletteProvider->revertAll();
}

void PaletteModel::paletteChanged()
{
    emit dataChanged(index(0, 0), index(mItems.count()-1, 2));
}

void PaletteModel::resetPalette()
{
    mPaletteProvider->resetPalette();
}

void PaletteModel::revertPalette()
{
    mPaletteProvider->revertPalette();
}

int PaletteModel::rowCount(const QModelIndex &) const
{
    return mItems.count();
}

int PaletteModel::columnCount(const QModelIndex &) const
{
    return mShowDetails ? 1 : 3;
}

QVariant PaletteModel::data(const QModelIndex & index, int role) const
{
    if (role != Qt::BackgroundRole)
        return QVariant();
    if (index.row() < 0 || index.row() >= mItems.count())
        return QVariant();

    PaletteModelItem * item = mItems[index.row()];
    return getColor(index.column(), item->role);
}

QVariant PaletteModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole)
            switch (section) {
            case 0: return mShowDetails ? tr("All") : tr("Active");
            case 1: return tr("Inactive");
            case 2: return tr("Disabled");
            }
    }
    else {
        if (section >= 0 && section < mItems.count()) {
            if (role == Qt::DisplayRole)
                return mItems[section]->titleText;
            if (role == Qt::BackgroundRole)
                return mItems[section]->titleBg;
        }
    }
    return QVariant();
}

QPalette::ColorGroup PaletteModel::getGroup(int grpIdx) const
{
    QPalette::ColorGroup grp = QPalette::All;

    switch (grpIdx) {
    case 0: grp = QPalette::Active; break;
    case 1: grp = QPalette::Inactive; break;
    case 2: grp = QPalette::Disabled; break;
    }
    return grp;
}

QColor PaletteModel::getColor(int grpIdx, QPalette::ColorRole role) const
{
    QPalette::ColorGroup grp = getGroup(grpIdx);
    return mPaletteProvider->getColor(grp, role);
}

void PaletteModel::setColor(QPalette::ColorGroup acg, QPalette::ColorRole acr, const QColor & acolor)
{
    mPaletteProvider->setColor(acg, acr, acolor);
}

//void PaletteModel::setPalette(const QPalette & pal)
//{
//    mPaletteProvider->resetPalette(pal);
//    emit dataChanged(index(0, 0), index(mItems.count()-1, 2));
//}


// PaletteModelDelegate
void ColorizerModelDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    QVariant varBg = index.model()->data(index, Qt::BackgroundRole);
    QColor bg = varBg.value<QColor>();
    QRect r = option.rect;
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, getBorderColor(bg));
        r.adjust(3, 3, -3, -3);
    }
    painter->fillRect(r, bg);
}

QSize ColorizerModelDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex &) const
{
    return { option.rect.width(), RowHeight };
}
