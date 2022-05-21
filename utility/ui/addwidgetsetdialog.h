/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   addwidgetsetdialog.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __ADDWIDGETSETDIALOG_H__INCLUDED__
#define __ADDWIDGETSETDIALOG_H__INCLUDED__

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class AddWidgetSetDialog;
}

class QCheckBox;
class AddWidgetSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWidgetSetDialog(QWidget *parent = 0);
    ~AddWidgetSetDialog();

    void setTypes(const QString &);
    void setNames(const QString &);

    QString getTypes();
    QString getNames();

private:
    Ui::AddWidgetSetDialog *ui;

    QButtonGroup mButtons;

    void disableButtons(bool);
    void buttonClicked(int);

};

#endif // __ADDWIDGETSETDIALOG_H__INCLUDED__
