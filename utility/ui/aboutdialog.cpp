/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   aboutdialog.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    setupUi(this);
    setWindowTitle("");
}

