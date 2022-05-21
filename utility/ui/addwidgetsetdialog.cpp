/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   addwidgetsetdialog.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "addwidgetsetdialog.h"
#include "ui_addwidgetsetdialog.h"
#include <QDebug>

AddWidgetSetDialog::AddWidgetSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWidgetSetDialog)
{
    ui->setupUi(this);

    mButtons.setExclusive(false);
    mButtons.addButton(ui->cbQWidget, 0);
    mButtons.addButton(ui->cbQToolButton);
    mButtons.addButton(ui->cbQDialogButtonBox);
    mButtons.addButton(ui->cbQToolBox);
    mButtons.addButton(ui->cbQPushButton);
    mButtons.addButton(ui->cbQCommandLinkButton);
    mButtons.addButton(ui->cbQFrame);
    mButtons.addButton(ui->cbQCheckBox);
    mButtons.addButton(ui->cbQGroupBox);
    mButtons.addButton(ui->cbQTabWidget);
    mButtons.addButton(ui->cbQComboBox);
    mButtons.addButton(ui->cbQLineEdit);
    mButtons.addButton(ui->cbQTextEdit);
    mButtons.addButton(ui->cbQSpinBox);
    mButtons.addButton(ui->cbQLabel);
    mButtons.addButton(ui->cbQScrollBar);
    mButtons.addButton(ui->cbQSlider);
    mButtons.addButton(ui->cbQDial);

    connect(&mButtons, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &AddWidgetSetDialog::buttonClicked);
}

AddWidgetSetDialog::~AddWidgetSetDialog()
{
    delete ui;
}

void AddWidgetSetDialog::setTypes(const QString & types)
{
    QStringList parts = types.split(',');
    QList<QAbstractButton *> btns = mButtons.buttons();
    for (QString type: parts)
        for (QAbstractButton * btn: btns)
            btn->setChecked(btn->text() == type);
}

void AddWidgetSetDialog::setNames(const QString & names)
{
    ui->edtNames->setText(names);
}

QString AddWidgetSetDialog::getTypes()
{
    QStringList names;
    if (ui->cbQWidget->isChecked())
        return ui->cbQWidget->text();

    QList<QAbstractButton *> btns = mButtons.buttons();
    for (QAbstractButton * btn: btns) {
        if (btn != ui->cbQWidget && btn->isChecked())
            names.append(btn->text());
    }

    QString rv = QStringLiteral("*");
    if (!names.isEmpty())
        rv = names.join(',');

    return rv;
}

QString AddWidgetSetDialog::getNames()
{
    QString rv = ui->edtNames->text();
    if (rv.isEmpty())
        rv = QStringLiteral("*");

    return rv;
}

void AddWidgetSetDialog::disableButtons(bool value)
{
    QList<QAbstractButton *> btns = mButtons.buttons();
    for (QAbstractButton * btn: btns)
        if (btn != ui->cbQWidget)
            btn->setEnabled(!value);
}

void AddWidgetSetDialog::buttonClicked(int btnId)
{
    if (btnId == 0)
        disableButtons(ui->cbQWidget->isChecked());
}
