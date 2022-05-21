/**
* \file   qtcolorizer.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "qtcolorizer.h"
#include "qtcolorizer_p.h"
#include <QApplication>
#include <QFile>
#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtWidgets>
#include <QDebug>

enum {
    WindowColorRoleFlag         = (1 << 1),
    WindowTextColorRoleFlag     = (1 << 2),
    ButtonColorRoleFlag         = (1 << 3),
    ButtonTextColorRoleFlag     = (1 << 4),
    BaseColorRoleFlag           = (1 << 5),
    AlternateBaseColorRoleFlag  = (1 << 6),
    TextColorRoleFlag           = (1 << 7),
    BrightTextColorRoleFlag     = (1 << 8),
    HighlightColorRoleFlag      = (1 << 9),
    HighlightedTextColorRoleFlag= (1 << 10),
    ToolTipColorRoleFlag        = (1 << 11),
    ToolTipTextColorRoleFlag    = (1 << 12),
    LinkColorRoleFlag           = (1 << 13),
    LinkVisitedColorRoleFlag    = (1 << 14),
};

uint32_t role2flag(QPalette::ColorRole r)
{
    switch (r) {
    case QPalette::Window:     return WindowColorRoleFlag;
    case QPalette::WindowText: return WindowTextColorRoleFlag;
    case QPalette::Button:     return ButtonColorRoleFlag;
    case QPalette::ButtonText: return ButtonTextColorRoleFlag;
    case QPalette::Base:       return BaseColorRoleFlag;
    case QPalette::AlternateBase: return AlternateBaseColorRoleFlag;
    case QPalette::Text:       return TextColorRoleFlag;
    case QPalette::BrightText: return BrightTextColorRoleFlag;
    case QPalette::Highlight:  return HighlightColorRoleFlag;
    case QPalette::HighlightedText: return HighlightedTextColorRoleFlag;
    case QPalette::ToolTipBase:return ToolTipColorRoleFlag;
    case QPalette::ToolTipText:return ToolTipTextColorRoleFlag;
    case QPalette::Link:       return LinkColorRoleFlag;
    case QPalette::LinkVisited:return LinkVisitedColorRoleFlag;
    default:
        return 0;
    }
}

uint32_t role2flag(QPalette::ColorGroup g, QPalette::ColorRole r)
{
    if (g == QPalette::Disabled) {
        switch (r) {
        case QPalette::Text:
        case QPalette::ButtonText:
        case QPalette::Highlight:
            return 0;
        default:;
        }
    }
    return role2flag(r);
}

int group2index(QPalette::ColorGroup g)
{
    if (g == QPalette::Active)   return 0;
    if (g == QPalette::Inactive) return 1;
    if (g == QPalette::Disabled) return 2;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
QtColorizerItem::QtColorizerItem(QtColorizerItemPrivate * i)
    : p(i)
{
    qDebug() << "ColorizerItem(ColorizerItemP *)";
}

QString QtColorizerItem::types() const
{
    return p->classes;
}

QString QtColorizerItem::names() const
{
    return p->names;
}

QPalette QtColorizerItem::palette() const
{
    return p->palette;
}

QFont QtColorizerItem::font() const
{
    if (p->base.data() == nullptr || p->fontModified)
        return p->font;

    return p->base.data()->font;
}

void QtColorizerItem::setTypes(const QString & types)
{
    p->classes = types;
}

void QtColorizerItem::setNames(const QString & names)
{
    p->names = names;
}

bool QtColorizerItem::isPaletteModified() const
{
    return p->isPaletteModified();
}

bool QtColorizerItem::isFontModified() const
{
    return p->fontModified;
}

bool QtColorizerItem::isModified() const
{
    return (p->isPaletteModified() || p->fontModified);
}

inline bool QtColorizerItem::isColorModified(int idx, uint32_t flag) const
{
    return p->isColorModified(idx, flag);
}

void QtColorizerItem::resetPalette(const QPalette & pal)
{
    p->palette = pal;
    p->flags[0] = p->flags[1] = p->flags[2] = 0xFFFF;
}

void QtColorizerItem::setColor(QPalette::ColorGroup g, QPalette::ColorRole r, const QColor & c)
{
    p->setColor(g, r, c);
}

void QtColorizerItem::revertColor(QPalette::ColorGroup g, QPalette::ColorRole r)
{
    p->revertColor(g, r);
}

void QtColorizerItem::revertPalette()
{
    if (p->base.data()) {
        p->palette = p->base->palette;
        p->flags[0] = p->flags[1] = p->flags[2] = 0;
    }
}

QPalette QtColorizerItem::getPalette() const
{
    if (p->base.data() == nullptr)
        return p->palette;

    static QPalette::ColorRole roles[14] = {
        QPalette::Window,
        QPalette::WindowText,
        QPalette::Button,
        QPalette::ButtonText,
        QPalette::Base,
        QPalette::AlternateBase,
        QPalette::Text,
        QPalette::BrightText,
        QPalette::Highlight,
        QPalette::HighlightedText,
        QPalette::ToolTipBase,
        QPalette::ToolTipText,
        QPalette::Link,
        QPalette::LinkVisited
    };
    static QPalette::ColorGroup groups[3] = { QPalette::Active, QPalette::Inactive, QPalette::Disabled };


    QPalette pal = p->base.data()->palette;

    uint32_t flag = 0;
    int idx = 0;
    for (int gi = 0; gi < 3; ++gi) {
        QPalette::ColorGroup g = groups[gi];
        idx = group2index(g);
        if (p->flags[idx] != 0)
            for (int ri = 0; ri < 14; ++ri) {
                QPalette::ColorRole r = roles[ri];
                flag = role2flag(r);
                if (p->flags[idx] & flag)
                    pal.setColor(g, r, p->palette.color(g, r));
            }
    }

    return pal;
}

QColor QtColorizerItem::getColor(QPalette::ColorGroup g, QPalette::ColorRole r) const
{
    return p->getColor(g, r);
}

void QtColorizerItem::setFont(const QFont & f)
{
    p->font = f;
    p->fontModified = true;
}

void QtColorizerItem::revertFont()
{
    p->font = p->defaultFont;
    p->fontModified = false;
}

void QtColorizerItem::reset()
{
    p->paletteModified = false;
    p->fontModified = false;
}

bool QtColorizerItem::isNull() const
{
    return p.isNull();
}

QtColorizerItem::QtColorizerItem()
{
    qDebug() << "ColorizerItem()";
}

QtColorizerItem & QtColorizerItem::operator =(const QtColorizerItem & other)
{
    qDebug() << "ColorizerItem::operator =(...)";
    this->p = other.p;
    return *this;
}

QtColorizerItemPrivate::~QtColorizerItemPrivate()
{
    qDebug() << "~ColorizerItemP()";
}

QColor QtColorizerItemPrivate::getColor(QPalette::ColorGroup g, QPalette::ColorRole r) const
{
    int idx = group2index(g);
    uint32_t flag = role2flag(r);
    if ((base.data() == nullptr) || ((flags[idx] & flag) == flag))
        return (g == QPalette::All) ? palette.color(r) : palette.color(g, r);

    return base.data()->getColor(g, r);
}

void QtColorizerItemPrivate::setColor(QPalette::ColorGroup g, QPalette::ColorRole r, const QColor & c)
{
    int idx = group2index(g);
    uint32_t flag = role2flag(r);

    if (base.data()) {
        QColor baseColor = base.data()->getColor(g, r);
        if (baseColor != c) {
            flags[idx] |= flag;
            palette.setColor(g, r, c);
        }
    }
    else {
        flags[idx] |= flag;
        palette.setColor(g, r, c);
    }
}

void QtColorizerItemPrivate::revertColor(QPalette::ColorGroup g, QPalette::ColorRole r)
{
    int idx = group2index(g);
    uint32_t flag = role2flag(r);
    flags[idx] &= ~flag;

    QColor c;
    if (base.data())
        c = base->palette.color(g, r);
    else
        c = defaultPalette.color(g, r);

    palette.setColor(g, r, c);
}

void QtColorizerItemPrivate::resetPalette(const QPalette & pal)
{
    palette = pal;
    flags[0] = flags[1] = flags[2] = ~(uint32_t)0; // all colors are modified
}

void QtColorizerItemPrivate::revertPalette()
{
    if (base.data()) {
        palette = base->palette;
        flags[0] = flags[1] = flags[2] = 0; // all colors are unmodified
    }
}

inline bool QtColorizerItemPrivate::isColorModified(int idx, uint32_t flag) const
{
    return (flags[idx] & flag) == flag;
}

bool QtColorizerItemPrivate::isPaletteModified() const
{
    return flags[0] || flags[1] || flags[2]; // if any color is modified then palette is modified
}

////////////////////////////////////////////////////////////////////////////////
QtColorizer::QtColorizer(const QPalette & defaultPalette, const QFont & defaultFont)
    : mDefaultPalette(defaultPalette)
    , mDefaultFont(defaultFont)
{
}

QtColorizer::~QtColorizer()
{
    qDebug() << "~Colorizer()";
}

bool QtColorizer::load(const QString & filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {

        QByteArray data = file.readAll();

        QJsonParseError err;
        QJsonDocument doc(QJsonDocument::fromJson(data, &err));

        if (err.error == QJsonParseError::NoError) {

            int version = 1;

            read(doc.object(), version);

            return true;
        }
    }
    return false;
}

bool QtColorizer::save(const QString & filePath, int version)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {

        QJsonObject root;
        write(root, version);

        QJsonDocument doc(root);
        QByteArray data = doc.toJson(QJsonDocument::Indented);
        return file.write(data) > 0;

    }
    return false;
}

void QtColorizer::apply(QApplication * a)
{
    if (!mItems.isEmpty()) {
        a->setPalette(mItems[0].palette());
        a->setFont(mItems[0].font());
    }
}

void QtColorizer::apply(QWidget * w)
{
    if (mItems.size() > 1) {
        for (int i = 1; i < mItems.size(); ++i) {
            const QtColorizerItem & item = mItems[i];
            QStringList types = item.types().split(',');
            QStringList names = item.names().split(',');
            foreach (const QString & type, types)
                applyByTypeName(w, type.trimmed(), names, item.getPalette(), item.font());
        }
    }
}

QVector<QtColorizerItem> QtColorizer::moveItems()
{
    QVector<QtColorizerItem> v = mItems;
    mItems.clear();
    return v;
}

void QtColorizer::setItems(const QVector<QtColorizerItem> & items)
{
    mItems = items;
}

void QtColorizer::apply(const QString & filePath, QApplication & a)
{
    QtColorizer colorizer(a.palette(), a.font());
    if (colorizer.load(filePath))
        colorizer.apply(&a);
}

QtColorizerItem QtColorizer::createBaseItem(const QString & classes, const QString & names,
                                        const QPalette & pal, const QFont & font)
{
    uint32_t flags[3] = {~(uint32_t)0, ~(uint32_t)0, ~(uint32_t)0};
    return createItem(QtColorizerItem(), classes, names, pal, font, flags, false);
}

QtColorizerItem QtColorizer::createItem(QtColorizerItem base,
                                    const QString & classes, const QString & names,
                                    const QPalette & pal, const QFont & font,
                                    uint32_t * flags, bool fontModified)
{
    QtColorizerItemPrivate * ip = new QtColorizerItemPrivate {classes, names, pal, pal, font, font, false, fontModified, {flags[0], flags[1], flags[2]}, base.p};
    return QtColorizerItem(ip);
}

void QtColorizer::write(QJsonObject & root, int version)
{
    root["version"] = QJsonValue(version);

    if (mItems.isEmpty())
        return;

    QJsonObject joColorizer;
    QJsonObject joBase;
    QtColorizerItem & baseItem = mItems[0];
    writeItem(joBase, baseItem, baseItem.font());
    joColorizer["base"] = joBase;

    if (mItems.size() > 1) {
        QJsonArray jaSets;
        for (int i = 1; i < mItems.size(); ++i) {
            QJsonObject joSet;
            QtColorizerItem & item = mItems[i];
            writeItem(joSet, item, baseItem.font(), &baseItem);
            if (!joSet.isEmpty()) jaSets.append(joSet);
        }
        if (!jaSets.isEmpty()) joColorizer["sets"] = jaSets;
    }

    root["colorizer"] = joColorizer;
}

void QtColorizer::read(const QJsonObject & root, int &)
{
    if (root.contains(QStringLiteral("colorizer")) && root[QStringLiteral("colorizer")].isObject()) {

        QtColorizerItem baseItem;

        QPalette basePalette;
        QFont    baseFont;

        QJsonObject joColorizer = root[QStringLiteral("colorizer")].toObject();
        if (joColorizer.contains(QStringLiteral("base")) && joColorizer[QStringLiteral("base")].isObject()) {
            QJsonObject joBase = joColorizer[QStringLiteral("base")].toObject();

            uint32_t flags[3] = {0, 0, 0};

            readPalette(joBase, basePalette, flags);
            readFont(joBase, baseFont);

            baseItem = createItem(QtColorizerItem(), "QApplication", "All widgets", basePalette, baseFont, flags, false);
            mItems.append(baseItem);
        }
        if (joColorizer.contains(QStringLiteral("sets")) && joColorizer[QStringLiteral("sets")].isArray()) {
            QJsonArray jaSets = joColorizer[QStringLiteral("sets")].toArray();
            for (int i = 0; i < jaSets.size(); ++i) {
                QJsonValue jv = jaSets.at(i);
                if (jv.isObject()) {
                    QJsonObject joSet = jv.toObject();

                    QtColorizerItem item = readItem(joSet, baseItem, false);

                    if (!item.isNull())
                        mItems.append(item);
                }
            }
        }
    }
}

void QtColorizer::readPalette(const QJsonObject & jObj, QPalette & pal, uint32_t * flags)
{
    if (jObj.contains(QStringLiteral("colors")) && jObj[QStringLiteral("colors")].isObject()) {
        QJsonObject joColors = jObj[QStringLiteral("colors")].toObject();
        readPalette(joColors, "active", pal, QPalette::Active, flags);
        readPalette(joColors, "inactive", pal, QPalette::Inactive, flags);
        readPalette(joColors, "disabled", pal, QPalette::Disabled, flags);
    }
}

void QtColorizer::readPalette(const QJsonObject & jObj, const QString & name, QPalette & pal, QPalette::ColorGroup grp, uint32_t * flags)
{
    if (jObj.contains(name) && jObj[name].isObject()) {

        QJsonObject joGroup = jObj[name].toObject();
        QColor c;

        int idx = group2index(grp);

        c = QColor(joGroup[QStringLiteral("window")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::Window, c);
                           flags[idx] |= role2flag(QPalette::Window); }

        c = QColor(joGroup[QStringLiteral("window-text")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::WindowText, c);
                           flags[idx] |= role2flag(QPalette::WindowText); }

        c = QColor(joGroup[QStringLiteral("button")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::Button, c);
                           flags[idx] |= role2flag(QPalette::Button); }

        c = QColor(joGroup[QStringLiteral("button-text")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::ButtonText, c);
                           flags[idx] |= role2flag(QPalette::ButtonText); }

        c = QColor(joGroup[QStringLiteral("base")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::Base, c);
                           flags[idx] |= role2flag(QPalette::Base); }

        c = QColor(joGroup[QStringLiteral("alt-base")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::AlternateBase, c);
                           flags[idx] |= role2flag(QPalette::AlternateBase); }

        c = QColor(joGroup[QStringLiteral("text")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::Text, c);
                           flags[idx] |= role2flag(QPalette::Text); }

        c = QColor(joGroup[QStringLiteral("bright-text")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::BrightText, c);
                           flags[idx] |= role2flag(QPalette::BrightText); }

        c = QColor(joGroup[QStringLiteral("highlight")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::Highlight, c);
                           flags[idx] |= role2flag(QPalette::Highlight); }

        c = QColor(joGroup[QStringLiteral("highlight-text")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::HighlightedText, c);
                           flags[idx] |= role2flag(QPalette::HighlightedText); }

        c = QColor(joGroup[QStringLiteral("tooltip-base")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::ToolTipBase, c);
                           flags[idx] |= role2flag(QPalette::ToolTipBase); }

        c = QColor(joGroup[QStringLiteral("tooltip-text")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::ToolTipText, c);
                           flags[idx] |= role2flag(QPalette::ToolTipText); }

        c = QColor(joGroup[QStringLiteral("link")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::Link, c);
                           flags[idx] |= role2flag(QPalette::Link); }

        c = QColor(joGroup[QStringLiteral("link-visited")].toString());
        if (c.isValid()) { pal.setColor(grp, QPalette::LinkVisited, c);
                           flags[idx] |= role2flag(QPalette::LinkVisited); }
    }
}

//void QtColorizer::writeFont(QJsonObject & jObj, const QFont & font)
//{
//    QJsonObject joFont;
//    joFont["family"] = font.family();
//    joFont["bold"] = font.bold();
//    joFont["italic"] = font.italic();

//    if (font.pointSize() != -1)
//        joFont["point-size"] = font.pointSize();
//    else if (font.pixelSize() != -1)
//        joFont["pixel-size"] = font.pixelSize();

//    jObj["font"] = joFont;
//}

bool QtColorizer::readFont(const QJsonObject & jObj, QFont & font)
{
    if (jObj.contains(QStringLiteral("font")) && jObj[QStringLiteral("font")].isObject()) {

        QJsonObject joFont = jObj[QStringLiteral("font")].toObject();
        font.setFamily(joFont.value(QStringLiteral("family")).toString(mDefaultFont.family()));
        font.setBold(joFont.value(QStringLiteral("bold")).toBool(mDefaultFont.bold()));
        font.setItalic(joFont.value(QStringLiteral("italic")).toBool(mDefaultFont.italic()));

        if (joFont.contains(QStringLiteral("point-size")))
            font.setPointSize(joFont.value(QStringLiteral("point-size")).toInt(mDefaultFont.pointSize()));
        else if (joFont.contains(QStringLiteral("pixel-size")))
            font.setPixelSize(joFont.value(QStringLiteral("pixel-size")).toInt(mDefaultFont.pixelSize()));

        return true;
    }

    return false;
}

void QtColorizer::writeItem(QJsonObject & jObj, const QtColorizerItem & item,
                          const QFont & baseFont,
                          const QtColorizerItem * baseItem)
{
    QJsonObject joColors;
    writeItemPalette(joColors, item, "active",   QPalette::Active, baseItem);
    writeItemPalette(joColors, item, "inactive", QPalette::Inactive, baseItem);
    writeItemPalette(joColors, item, "disabled", QPalette::Disabled, baseItem);

    if (baseItem) {
        jObj["types"] = item.types();
        jObj["names"] = item.names();
    }

    if (!joColors.isEmpty())
        jObj["colors"] = joColors;

    writeItemFont(jObj, item, baseFont, baseItem);

}

QColor getColor(const QtColorizerItem & item,
                QPalette::ColorGroup g, QPalette::ColorRole r,
                const QtColorizerItem * baseItem = nullptr)
{
    if (baseItem) {
        QColor baseColor = baseItem->getColor(g, r);
        QColor itemColor = item.getColor(g, r);
        return (baseColor == itemColor) ? QColor() : itemColor;
    }
    return item.getColor(g, r);


//    if (base)
//        return item.palette().color(g, r);

//    int idx = group2index(g);
//    uint32_t flag = role2flag(r);
//    return (item.isColorModified(idx, flag)) ? item.getColor(g, r) : QColor();
}

void QtColorizer::writeItemPalette(QJsonObject & jObj, const QtColorizerItem & item,
                                 const QString & name,
                                 QPalette::ColorGroup grp, const QtColorizerItem * baseItem)
{
    QJsonObject joGroup;
    QColor c;

    c = getColor(item, grp, QPalette::Window, baseItem);
    if (c.isValid()) joGroup["window"] = c.name();

    c = getColor(item, grp, QPalette::WindowText, baseItem);
    if (c.isValid()) joGroup["window-text"] = c.name();

    c = getColor(item, grp, QPalette::Button, baseItem);
    if (c.isValid()) joGroup["button"] = c.name();

    c = getColor(item, grp, QPalette::ButtonText, baseItem);
    if (c.isValid()) joGroup["button-text"] = c.name();

    c = getColor(item, grp, QPalette::Base, baseItem);
    if (c.isValid()) joGroup["base"] = c.name();

    c = getColor(item, grp, QPalette::AlternateBase, baseItem);
    if (c.isValid()) joGroup["alt-base"] = c.name();

    c = getColor(item, grp, QPalette::Text, baseItem);
    if (c.isValid()) joGroup["text"] = c.name();

    c = getColor(item, grp, QPalette::BrightText, baseItem);
    if (c.isValid()) joGroup["bright-text"] = c.name();

    c = getColor(item, grp, QPalette::Highlight, baseItem);
    if (c.isValid()) joGroup["highlight"] = c.name();

    c = getColor(item, grp, QPalette::HighlightedText, baseItem);
    if (c.isValid()) joGroup["highlight-text"] = c.name();

    c = getColor(item, grp, QPalette::ToolTipBase, baseItem);
    if (c.isValid()) joGroup["tooltip-base"] = c.name();

    c = getColor(item, grp, QPalette::ToolTipText, baseItem);
    if (c.isValid()) joGroup["tooltip-text"] = c.name();


    c = getColor(item, grp, QPalette::Link, baseItem);
    if (c.isValid()) joGroup["link"] = c.name();

    c = getColor(item, grp, QPalette::LinkVisited, baseItem);
    if (c.isValid()) joGroup["link-visited"] = c.name();

    if (!joGroup.isEmpty()) jObj[name] = joGroup;
}

void QtColorizer::writeItemFont(QJsonObject & jObj, const QtColorizerItem & item, const QFont & dfltFont, const QtColorizerItem * baseItem)
{
    if (baseItem == nullptr)
        writeItemFont(jObj, item.font(), dfltFont);
    else {
        QFont font = item.font();
        if (font != baseItem->font())
            writeItemFont(jObj, font, dfltFont);
    }
}

void QtColorizer::writeItemFont(QJsonObject & jObj, const QFont & font, const QFont & dfltFont)
{
    QJsonObject joFont;
    joFont["family"] = font.family();
    joFont["bold"] = font.bold();
    joFont["italic"] = font.italic();

    if (font.pointSize() != -1)
        joFont["point-size"] = font.pointSize();
    else if (font.pixelSize() != -1)
        joFont["pixel-size"] = font.pixelSize();
    else
        joFont["point-size"] = dfltFont.pointSize();

    jObj["font"] = joFont;
}

QtColorizerItem QtColorizer::readItem(const QJsonObject & jObj, QtColorizerItem baseItem, bool isBaseItem)
{
    QString types = isBaseItem ? QStringLiteral("QApplication") : QStringLiteral("*");
    if (jObj.contains(QStringLiteral("types")) && jObj[QStringLiteral("types")].isString())
        types = jObj[QStringLiteral("types")].toString();

    QString names = isBaseItem ? QStringLiteral("All widgets") : QStringLiteral("*");
    if (jObj.contains(QStringLiteral("names")) && jObj[QStringLiteral("names")].isString())
        names = jObj[QStringLiteral("names")].toString();

    QPalette palette  = baseItem.palette();
    QFont    font     = baseItem.font();

    uint32_t flags[3] = {0, 0, 0};
    if (jObj.contains(QStringLiteral("colors")) && jObj[QStringLiteral("colors")].isObject()) {
        QJsonObject joColors = jObj[QStringLiteral("colors")].toObject();
        readPalette(joColors, "active", palette, QPalette::Active, flags);
        readPalette(joColors, "inactive", palette, QPalette::Inactive, flags);
        readPalette(joColors, "disabled", palette, QPalette::Disabled, flags);
    }

    bool fontModified = readFont(jObj, font);

    return createItem(baseItem, types, names, palette, font, flags, fontModified);
}

template <typename T>
void applyByName(QObject * obj, const QString & name, const QPalette & pal, const QFont & font) {
    QList<T*> children;
    if (name == QStringLiteral("*"))
        children = obj->findChildren<T*>(QRegExp("."));
    else
        children = obj->findChildren<T*>(name);

    foreach (T * child, children) {
        child->setPalette(pal);
        child->setFont(font);
    }
}

void QtColorizer::applyByTypeName(QObject * obj, const QString & type, const QStringList & names, const QPalette & pal, const QFont & font)
{


#define APPLY_BY_NAME(QWidgetType) \
if (type == QStringLiteral(#QWidgetType)) \
    { foreach (const QString & name, names) applyByName<QWidgetType>(obj, name.trimmed(), pal, font); }


    APPLY_BY_NAME(QWidget)
    else APPLY_BY_NAME(QPushButton)
    else APPLY_BY_NAME(QToolButton)
    else APPLY_BY_NAME(QDial)
    else APPLY_BY_NAME(QDialogButtonBox)
    else APPLY_BY_NAME(QToolBox)
    else APPLY_BY_NAME(QCommandLinkButton)
    else APPLY_BY_NAME(QFrame)
    else APPLY_BY_NAME(QCheckBox)
    else APPLY_BY_NAME(QGroupBox)
    else APPLY_BY_NAME(QTabWidget)
    else APPLY_BY_NAME(QComboBox)
    else APPLY_BY_NAME(QLineEdit)
    else APPLY_BY_NAME(QTextEdit)
    else APPLY_BY_NAME(QSpinBox)
    else APPLY_BY_NAME(QLabel)
    else APPLY_BY_NAME(QScrollBar)
    else APPLY_BY_NAME(QSlider)

#undef APPLY_BY_NAME
}
