/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   aboutdialog.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __ABOUTDIALOG_H__INCLUDED__
#define __ABOUTDIALOG_H__INCLUDED__

#include <QDialog>
#include "ui_aboutdialog.h"

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog, public Ui::AboutDialog
{
public:
    explicit AboutDialog(QWidget *parent = 0);
};

#endif // __ABOUTDIALOG_H__INCLUDED__
