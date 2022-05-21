/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colorchooser.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __COLORCHOOSER_H__INCLUDED__
#define __COLORCHOOSER_H__INCLUDED__

#include <QWidget>
#include <QPair>

namespace Ui {
class ColorChooser;
}

class QButtonGroup;
class ColorDB;
class CustomColorValidator;
class CustomColorModel;
class CustomColorModelDelegate;
class PaletteModel;
class ColorToneModelDelegate;
class ColorizerModel;
class WidgetSetModelDelegate;
class ColorChooser : public QWidget
{
    Q_OBJECT

public:
    explicit ColorChooser(QWidget *parent = 0);
    ~ColorChooser();

    QString getFileName();

    void setColorDB(ColorDB *);
    QPalette getPalette() const;
    QFont    getFont() const;

signals:
    void paletteChanged();
    void fontChanged();
    void stateChanged(int);
    void modified(bool);

public slots:
    bool save();

private slots:
    void on_btnResetAll_clicked();
    void on_btnChangeFont_clicked();
    void on_btnRevertPalette_clicked();
    void on_btnAbout_clicked();
    void on_cbxQuick_clicked(bool checked);
    void on_cbxComputeDetails_clicked(bool checked);

    void newColorizer();
    void load();
    bool saveAs();

    void resetColor();
    void revertColor();

    void resetRole();
    void revertRole();

    void resetWidgetSet();
    void revertWidgetSet();

    void resetAll();
    void revertAll();

    void resetPalette();
    void revertPalette();

    void revertFont();
    void resetFont();

    void on_btnEditStyle_clicked();

    void on_btnRemoveStyle_clicked();

    void updateResetActionUi();
    void updateRevertActionUi();

private:
    Ui::ColorChooser *ui;

    QString mFilePath;
    QString mFileName = tr("noname");

    ColorDB * mDb = nullptr;

    PaletteModel * mPaletteModel = nullptr;

    ColorToneModelDelegate * mToneModelDelegate = nullptr;

    CustomColorModel * mCustomColorModel = nullptr;
    CustomColorModelDelegate * mCustomColorModelDelegate = nullptr;

    ColorizerModel * mWidgetSetModel = nullptr;
    WidgetSetModelDelegate * mWidgetSetModelDelegate = nullptr;

    QAction * mActionResetColor = nullptr;
    QAction * mActionResetRole = nullptr;
    QAction * mActionResetWidgetSet = nullptr;
    QAction * mActionResetAll = nullptr;

    QAction * mActionRevertColor = nullptr;
    QAction * mActionRevertRole = nullptr;
    QAction * mActionRevertWidgetSet = nullptr;
    QAction * mActionRevertAll = nullptr;


    void setFilePath(const QString &);

    void setColor(const QColor &);
    void setFont(const QFont &);

    int getGroupIndex(int);
    QPair<int, int> getPaletteCurrentIndeces();

    void setModified(bool);
    void setShowDetails(bool);

    void setupActions();
    void updateWidgetSetButtons();
    void updateUi();
};

#endif // __COLORCHOOSER_H__INCLUDED__
