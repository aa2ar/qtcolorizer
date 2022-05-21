/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   testee.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "testee.h"
#include "ui_testee.h"
#include <QColor>

Testee::Testee(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Testee)
{
    ui->setupUi(this);
    ui->horizontalSlider->setValue(50);
    ui->horizontalSlider_2->setValue(50);
}

Testee::~Testee()
{
    delete ui;
}

void Testee::setColorPalette(const QPalette & pal)
{
    setPalette(pal);
}

void Testee::closeEvent(QCloseEvent * event)
{
     event->ignore();
}
