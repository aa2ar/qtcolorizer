/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   testee.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __TESTEE_H__INCLUDED__
#define __TESTEE_H__INCLUDED__

#include <QWidget>
#include <QCloseEvent>
#include <QPalette>

namespace Ui {
class Testee;
}

class QColor;
class Testee : public QWidget
{
    Q_OBJECT

public:
    explicit Testee(QWidget *parent = 0);
    ~Testee();

    void setColorPalette(const QPalette &);

protected:
    void closeEvent(QCloseEvent * event) Q_DECL_OVERRIDE;

private:
    Ui::Testee *ui;
};

#endif // __TESTEE_H__INCLUDED__
