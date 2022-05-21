/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   mainwidnow.h
* \author Alex Arefyev
* \date   14.06.2018
*
*
*/

#ifndef __MAINWINDOW_H__INCLUDED__
#define __MAINWINDOW_H__INCLUDED__

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QMdiSubWindow;
class QCloseEvent;
class QtColorizer;
class ColorDB;
class Testee;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;

    ColorDB * mDb = nullptr;

    QtColorizer * mColorizer = nullptr;

    Testee * mTestee = nullptr;
    QMdiSubWindow * mSub0 = nullptr;
    QMdiSubWindow * mSub1 = nullptr;

    void setState(int);
    void updateWindowTitle();
};

#endif // __MAINWINDOW_H__INCLUDED__
