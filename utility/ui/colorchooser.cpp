/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   colorchooser.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "colorchooser.h"
#include "ui_colorchooser.h"
#include "core/colordb.h"
#include "core/palettemodel.h"
#include "core/customcolormodel.h"
#include "core/colorizermodel.h"
#include "core/widgetsetmodeldelegate.h"
#include "core/appsettings.h"
#include "core/paletteprovider.h"
#include "../qtcolorizer/qtcolorizer.h"
#include "aboutdialog.h"
#include "addwidgetsetdialog.h"
#include <QApplication>
#include <QLineEdit>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QDebug>

class CustomColorValidator : public QValidator
{
public:
    CustomColorValidator(QLineEdit * ed, QObject * parent = nullptr)
        : QValidator(parent)
        , mEdit(ed)
        , re1("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$")
        , re2("^([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3})$")
    {}

    State validate(QString &input, int &pos) const;

    QColor getColor(const QString &);

private:
    QLineEdit * mEdit = nullptr;
    mutable bool mValid = false;
    QRegExp re1, re2;
};

QValidator::State CustomColorValidator::validate(QString & input, int &) const
{
    bool valid = false;

    if (input.startsWith("#"))
        valid = re1.exactMatch(input);
    else
    if (re2.exactMatch(input)) {
        QStringList parts = re2.capturedTexts();
        if (parts.size() == 4) {
            int r = parts[1].toUInt();
            int g = parts[2].toUInt();
            int b = parts[3].toUInt();
            valid = (r < 256 && g < 256 && b < 256);
        }
    }

    //if (valid != mValid)
    {
        mValid = valid;

        QColor c = mValid ? Qt::darkGreen : Qt::darkGray;
        QPalette pal = mEdit->palette();
        pal.setColor(QPalette::Text, c);
        mEdit->setPalette(pal);
    }

    return valid ? Acceptable : Intermediate;
}

QColor CustomColorValidator::getColor(const QString & text)
{
    if (re1.exactMatch(text))
        return QColor(text);

    if (re2.exactMatch(text)) { // DRY!
        QStringList parts = re2.capturedTexts();
        if (parts.size() == 4) {
            int r = parts[1].toUInt();
            int g = parts[2].toUInt();
            int b = parts[3].toUInt();
            if (r < 256 && g < 256 && b < 256)
                return QColor((int)r, (int)g, (int)b);
        }
    }

    return QColor();
}


static
bool chooseColor(QColor & c, QWidget * parent)
{
    QColorDialog dlg(c, parent);
    if (dlg.exec() == QDialog::Accepted) {
        c = dlg.selectedColor();
        return true;
    }
    return false;
}

// ColorChooser
ColorChooser::ColorChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorChooser)
{
    ui->setupUi(this);

    ui->paletteTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setupActions();


    connect(ui->btnNewColorizer, SIGNAL(clicked()), this, SLOT(newColorizer()));
    connect(ui->btnLoadColorizer, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui->btnSavePalette, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->btnSavePaletteAs, SIGNAL(clicked()), this, SLOT(saveAs()));

    connect(ui->btnRevertFont, SIGNAL(clicked()), this, SLOT(revertFont()));
    connect(ui->btnResetFont, SIGNAL(clicked()), this, SLOT(resetFont()));

    QFont f = font();
    f.setFamily("Monospace");

    mCustomColorModel = new CustomColorModel(this);
    mCustomColorModel->load(qApp->property("custom-colors-file").toString());
    ui->customColorListView->setModel(mCustomColorModel);
    mCustomColorModelDelegate = new CustomColorModelDelegate(ui->customColorListView,
                                                             palette().color(QPalette::Window),
                                                             f, font());
    ui->customColorListView->setItemDelegate(mCustomColorModelDelegate);
    ui->customColorListView->setMouseTracking(true);

    mWidgetSetModel = new ColorizerModel(palette(), font(), {"QApplication", "All widgets", palette(), font()}, this);
    connect(mWidgetSetModel, &ColorizerModel::modified, this, [this](bool value){
        setModified(value);
    });
    mWidgetSetModelDelegate = new WidgetSetModelDelegate(mWidgetSetModel, ui->cbxWidgetSet,
                                                            [this](ColorizerModel::AddItem & ai) -> bool {
                                                                AddWidgetSetDialog dlg(this);
                                                                dlg.setTypes(ai.types);
                                                                dlg.setNames(ai.names);
                                                                if (dlg.exec() != QDialog::Accepted)
                                                                    return false;
                                                                ai.types = dlg.getTypes();
                                                                ai.names = dlg.getNames();
                                                                return true;
                                                            }, this);
    ui->cbxWidgetSet->setItemDelegate(mWidgetSetModelDelegate);
    ui->cbxWidgetSet->setModel(mWidgetSetModel->itemListModel());
    ui->cbxWidgetSet->setFixedHeight(mWidgetSetModelDelegate->height());

    connect(mWidgetSetModelDelegate, &WidgetSetModelDelegate::paletteChanged,
            this, &ColorChooser::paletteChanged);
    connect(mWidgetSetModelDelegate, &WidgetSetModelDelegate::fontChanged,
            this, &ColorChooser::fontChanged);

    ////////////////////////////////////////////////////////////////
    mPaletteModel = new PaletteModel(mWidgetSetModelDelegate, palette(), this);
    ui->paletteTable->setModel(mPaletteModel);
    ui->paletteTable->setItemDelegate(new ColorizerModelDelegate());

    connect(mWidgetSetModelDelegate, &WidgetSetModelDelegate::widgetSetChanged, this, [this](int){
        if (mWidgetSetModelDelegate->isValid()) {
            mPaletteModel->paletteChanged();
            updateWidgetSetButtons();
            emit paletteChanged();
            emit fontChanged();
        }
    });

    ////////////////////////////////////////////////////////////////
    QHeaderView * hv = ui->paletteTable->horizontalHeader();
    hv->setSectionResizeMode(0, QHeaderView::Stretch);

    hv = ui->paletteTable->verticalHeader();
    hv->setSectionResizeMode(QHeaderView::Fixed);

    ui->paletteTable->resizeRowsToContents();
    ui->paletteTable->resizeColumnsToContents();

    connect(ui->customColorListView, &QListView::clicked, this, [this](const QModelIndex & index){
        if (index.row() == mCustomColorModel->rowCount()-1) {
            QColor c(Qt::gray);
            if (chooseColor(c, this)) {
                mCustomColorModel->addColor(c);
                ui->customColorListView->selectionModel()->clear();
            }
        }
        else {
            QPoint pt = ui->customColorListView->mapFromGlobal(QCursor::pos());
            QRect ri = ui->customColorListView->visualRect(index);
            int h = mCustomColorModelDelegate->height();
            int right = ri.right() - h;
            if (pt.x() > right)
                mCustomColorModel->removeColor(index.row());
            else {
                right -= h;
                if (pt.x() > right) {
                    QColor c = mCustomColorModel->getColor(index.row());
                    if (c.isValid() && chooseColor(c, this))
                        mCustomColorModel->modifyColor(index.row(), c);
                }
                else {
                    right -= h;
                    if (pt.x() > right) {
                        QColor c = mCustomColorModel->getColor(index.row());
                        if (c.isValid() && chooseColor(c, this)) {
                            mCustomColorModel->addColor(c);
                            ui->customColorListView->selectionModel()->clear();
                        }
                    }
                    else
                        setColor(mCustomColorModel->getColor(index.row()));
                }
            }
        }
    });

    ui->paletteTable->setCurrentIndex(mPaletteModel->index(0, 0));

    //on_cbxQuick_clicked(ui->cbxQuick->isChecked());
    updateUi();
}

ColorChooser::~ColorChooser()
{
    mCustomColorModel->save();
    delete ui;
}

QString ColorChooser::getFileName()
{
    return mFileName;
}

void ColorChooser::setColorDB(ColorDB * db)
{
    if (mDb != nullptr)
        return;

    mDb = db;

    ui->colorListView->setModel(mDb->colorModel());
    ui->toneListView->setModel(mDb->tonesModel());

    ui->colorListView->setItemDelegate(new ColorGroupModelDelegate());

    mToneModelDelegate = new ColorToneModelDelegate(ui->toneListView, palette().color(QPalette::Window));
    ui->toneListView->setItemDelegate(mToneModelDelegate);

    connect(ui->colorListView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this](const QItemSelection &, const QItemSelection &){
        int idx = ui->colorListView->selectionModel()->currentIndex().row();
        mDb->setCurrentTone(idx);
    });

    connect(ui->toneListView, &QListView::clicked,
            this, [this](const QModelIndex & index){

        int row = ui->toneListView->selectionModel()->currentIndex().row();
        QColor c = mDb->getColor(row);

        QPoint pt = ui->toneListView->mapFromGlobal(QCursor::pos());
        QRect ri = ui->toneListView->visualRect(index);
        int h = mToneModelDelegate->height();
        int right = ri.right() - h;
        if (pt.x() > right) {
            if (c.isValid() && chooseColor(c, this)) {
                mCustomColorModel->addColor(c);
                ui->customColorListView->selectionModel()->clear();
            }
        }
        else
            setColor(c);
    });
}

QPalette ColorChooser::getPalette() const
{
    return mWidgetSetModelDelegate->getPalette();
}

QFont ColorChooser::getFont() const
{
    return mWidgetSetModelDelegate->getFont();
}

void ColorChooser::resetPalette()
{
    mPaletteModel->resetPalette();
}

void ColorChooser::revertPalette()
{
    mPaletteModel->revertPalette();
}

void ColorChooser::setFont(const QFont & font)
{
    mWidgetSetModelDelegate->resetFont(font);
}

int ColorChooser::getGroupIndex(int idx)
{
    if (ui->cbxQuick->isChecked() || ui->cbxComputeDetails->isChecked())
        return -1;
    return idx;
}

QPair<int, int> ColorChooser::getPaletteCurrentIndeces()
{
    QModelIndex index = ui->paletteTable->selectionModel()->currentIndex();
    return {getGroupIndex(index.column()), index.row()};
}

void ColorChooser::setModified(bool value)
{
    setWindowModified(value);
    emit modified(value);
}

void ColorChooser::setShowDetails(bool value)
{
    mPaletteModel->setShowDetails(value);
    ui->paletteTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ColorChooser::setupActions()
{
    QMenu * m = new QMenu(this);
    connect(m, SIGNAL(aboutToShow()), this, SLOT(updateResetActionUi()));

    mActionResetColor = m->addAction(tr("Reset color"));
    connect(mActionResetColor, SIGNAL(triggered(bool)), this, SLOT(resetColor()));
    m->addSeparator();

    mActionResetRole = m->addAction(tr("Reset role"));
    connect(mActionResetRole, SIGNAL(triggered(bool)), this, SLOT(resetRole()));
    m->addSeparator();

    mActionResetWidgetSet = m->addAction(tr("Reset widget set"));
    connect(mActionResetWidgetSet, SIGNAL(triggered(bool)), this, SLOT(resetWidgetSet()));
    m->addSeparator();

    mActionResetAll = m->addAction(tr("Reset all"));
    connect(mActionResetAll, SIGNAL(triggered(bool)), this, SLOT(resetAll()));

    ui->btnResetColor->setMenu(m);


    m = new QMenu(this);
    connect(m, SIGNAL(aboutToShow()), this, SLOT(updateRevertActionUi()));

    mActionRevertColor = m->addAction(tr("Revert color"));
    connect(mActionRevertColor, SIGNAL(triggered(bool)), this, SLOT(revertColor()));
    m->addSeparator();

    mActionRevertRole = m->addAction(tr("Revert role"));
    connect(mActionRevertRole, SIGNAL(triggered(bool)), this, SLOT(revertRole()));
    m->addSeparator();

    mActionRevertWidgetSet = m->addAction(tr("Revert widget set"));
    connect(mActionRevertWidgetSet, SIGNAL(triggered(bool)), this, SLOT(revertWidgetSet()));
    m->addSeparator();

    mActionRevertAll = m->addAction(tr("Revert all"));
    connect(mActionRevertAll, SIGNAL(triggered(bool)), this, SLOT(revertAll()));
    ui->btnRevertColor->setMenu(m);

}

void ColorChooser::updateWidgetSetButtons()
{
    int idx = ui->cbxWidgetSet->currentIndex();
    bool enabled = idx > 0;
    ui->btnEditStyle->setEnabled(enabled);
    ui->btnRemoveStyle->setEnabled(enabled);
}

void ColorChooser::updateUi()
{
    ui->cbxComputeDetails->setEnabled(!mWidgetSetModel->quick());
    ui->cbxComputeDetails->setChecked(mWidgetSetModel->computeDetails());
}

void ColorChooser::on_btnResetAll_clicked()
{
    resetPalette();
    resetFont();
}

void ColorChooser::setColor(const QColor & c)
{
    QPair<int, int> indeces = getPaletteCurrentIndeces();
    mPaletteModel->setColor(indeces.first, indeces.second, c);
}

void ColorChooser::resetColor()
{
    QPair<int, int> indeces = getPaletteCurrentIndeces();
    mPaletteModel->resetColor(indeces.first, indeces.second);
}

void ColorChooser::revertColor()
{
    QPair<int, int> indeces = getPaletteCurrentIndeces();
    mPaletteModel->revertColor(indeces.first, indeces.second);
}

void ColorChooser::resetRole()
{
    QPair<int, int> indeces = getPaletteCurrentIndeces();
    mPaletteModel->resetRole(indeces.second);
}

void ColorChooser::revertRole()
{
    QPair<int, int> indeces = getPaletteCurrentIndeces();
    mPaletteModel->revertRole(indeces.second);
}

void ColorChooser::resetWidgetSet()
{
    mPaletteModel->resetWidgetSet();
}

void ColorChooser::revertWidgetSet()
{
    mPaletteModel->revertWidgetSet();
}

void ColorChooser::resetAll()
{
    mPaletteModel->resetAll();
}

void ColorChooser::revertAll()
{
    mPaletteModel->revertAll();
}

void ColorChooser::on_btnChangeFont_clicked()
{
    QFontDialog dlg(getFont(), this);
    if (QDialog::Accepted == dlg.exec())
        setFont(dlg.selectedFont());
}

void ColorChooser::on_btnRevertPalette_clicked()
{
    revertPalette();
}

void ColorChooser::newColorizer()
{
    bool ok = true;
    if (mWidgetSetModel->isModified()) {
        QMessageBox::StandardButton btn =
                QMessageBox::question(
                    this, QStringLiteral(""),
                    tr("Modified. Save?"),
                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (btn == QMessageBox::Yes)
            ok = save();
        else
        if (btn == QMessageBox::No) {
            mWidgetSetModel->clearModified();
            ok = true;
        }
        else
            ok = false;
    }

    if (ok)
        if (mWidgetSetModel->newItems()) {
            ui->cbxWidgetSet->setCurrentIndex(0);
            mPaletteModel->paletteChanged();
            emit paletteChanged();
        }
}

void ColorChooser::load()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open file"), AppSettings::lastTargetFile(),
                                                    tr("JSON (*.json);;All Files (*)"));
    if (!fileName.isEmpty()) {
        AppSettings::setLastTargetFile(fileName);
        setFilePath(fileName);
        QtColorizer loader(palette(), font());
        if (loader.load(fileName)) {
            mWidgetSetModel->setItems(loader.moveItems());
            ui->cbxWidgetSet->setCurrentIndex(0);
            emit paletteChanged();
            emit fontChanged();
        }
    }
}

bool ColorChooser::save()
{
    if (!isWindowModified())
        return true;

    if (QFile(mFilePath).exists()) {
        QtColorizer loader(palette(), font());

        loader.setItems(mWidgetSetModel->getItems());
        if (loader.save(mFilePath)) {
            mWidgetSetModel->clearModified();
            return true;
        }

        QMessageBox::critical(this,
                              QStringLiteral(""),
                              "Error saving file");
        return false;
    }

    return saveAs();
}

bool ColorChooser::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save file as"), AppSettings::lastTargetFile(),
                                                    tr("JSON (*.json);;All Files (*)"));
    if (fileName.isEmpty())
        return false;

    AppSettings::setLastTargetFile(fileName);
    QtColorizer loader(palette(), font());
    loader.setItems(mWidgetSetModel->getItems());
    if (loader.save(fileName)) {
        setFilePath(fileName);
        mWidgetSetModel->clearModified();
        return true;
    }

    QMessageBox::critical(this,
                          QStringLiteral(""),
                          "Error saving file");

    return false;
}

void ColorChooser::revertFont()
{
    mWidgetSetModelDelegate->revertFont();
}

void ColorChooser::resetFont()
{
    mWidgetSetModelDelegate->resetFont(font());
}

void ColorChooser::setFilePath(const QString & filePath)
{
    mFilePath = filePath;
    mFileName = QFileInfo(filePath).fileName();
    ui->lblFilePath->setText(filePath);
}

void ColorChooser::on_btnAbout_clicked()
{
    AboutDialog().exec();
}


void ColorChooser::on_cbxQuick_clicked(bool checked)
{
    mWidgetSetModel->setQuick(checked);
    setShowDetails(mWidgetSetModel->computeDetails() || mWidgetSetModel->quick());
    updateUi();
}

void ColorChooser::on_cbxComputeDetails_clicked(bool checked)
{
    mWidgetSetModel->setComputeDetails(checked);
    setShowDetails(mWidgetSetModel->computeDetails());
    updateUi();
}

void ColorChooser::on_btnEditStyle_clicked()
{
    mWidgetSetModelDelegate->editCurrentWidgetSet();
}

void ColorChooser::on_btnRemoveStyle_clicked()
{
    if (QMessageBox::question(this,
                              qApp->applicationDisplayName(),
                              tr("Remove current widget set?"),
                              QMessageBox::Yes,
                              QMessageBox::No) == QMessageBox::Yes)
        mWidgetSetModelDelegate->removeCurrentWidgetSet();
}

void ColorChooser::updateResetActionUi()
{

}

void ColorChooser::updateRevertActionUi()
{
    int idx = ui->cbxWidgetSet->currentIndex();
    bool enable = idx != 0;
    mActionRevertColor->setEnabled(enable);
    mActionRevertRole->setEnabled(enable);
    mActionRevertWidgetSet->setEnabled(enable);
}
