/**
* \file   qtcolorizer.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __QTCOLORIZER_H__INCLUDED__
#define __QTCOLORIZER_H__INCLUDED__

#include <QPalette>
#include <QFont>
#include <QSharedPointer>

struct QtColorizerItemPrivate;
class QtColorizerItem
{
    friend class QtColorizer;
    QtColorizerItem(QtColorizerItemPrivate *);
    QSharedPointer<QtColorizerItemPrivate> p;

public:
    QString types()    const;
    QString names()    const;
    QPalette palette() const;
    QFont    font()    const;

    void setTypes(const QString &);
    void setNames(const QString &);

    bool isPaletteModified() const;
    bool isFontModified()    const;
    bool isModified()        const;

    inline bool isColorModified(int, uint32_t) const;

    void resetPalette(const QPalette &);
    void revertPalette();

    QPalette getPalette() const;

    QColor getColor(QPalette::ColorGroup, QPalette::ColorRole) const;
    void setColor(QPalette::ColorGroup, QPalette::ColorRole, const QColor &);
    void revertColor(QPalette::ColorGroup, QPalette::ColorRole);

    void setFont(const QFont &);
    void revertFont();

    void reset();

    bool isNull() const;

public:
    QtColorizerItem();
    QtColorizerItem & operator =(const QtColorizerItem &);
};

class QtColorizer
{
public:
    explicit QtColorizer(const QPalette &, const QFont &);
    ~QtColorizer();

    bool load(const QString &);
    bool save(const QString &, int version = 1);

    void apply(QApplication *);
    void apply(QWidget *);

    QVector<QtColorizerItem> moveItems();
    void setItems(const QVector<QtColorizerItem> &);

    static void apply(const QString &, QApplication &);

    static QtColorizerItem createBaseItem(const QString &, const QString &, const QPalette &, const QFont &);

    static QtColorizerItem createItem(QtColorizerItem,
                                    const QString &, const QString &,
                                    const QPalette &, const QFont &,
                                    uint32_t *, bool);

private:
    int     mVersion = 1;
    QPalette mDefaultPalette;
    QFont    mDefaultFont;
    QVector<QtColorizerItem> mItems;

    void write(QJsonObject &, int);

    void read(const QJsonObject &, int &);
    void readPalette(const QJsonObject &, QPalette &, uint32_t *flags);
    void readPalette(const QJsonObject &, const QString &, QPalette &, QPalette::ColorGroup, uint32_t *flags);
    bool readFont(const QJsonObject &, QFont &);

    void writeItem(QJsonObject &, const QtColorizerItem &, const QFont &, const QtColorizerItem * baseItem = nullptr);
    void writeItemPalette(QJsonObject &, const QtColorizerItem &, const QString &, QPalette::ColorGroup, const QtColorizerItem * baseItem = nullptr);
    void writeItemFont(QJsonObject &, const QtColorizerItem &, const QFont &, const QtColorizerItem * baseItem = nullptr);
    void writeItemFont(QJsonObject &, const QFont &, const QFont &);

    QtColorizerItem readItem(const QJsonObject &, QtColorizerItem, bool baseItem = true);

    void applyByTypeName(QObject * obj, const QString & type, const QStringList &names, const QPalette &, const QFont &);
};

#endif // __QTCOLORIZER_H__INCLUDED__
