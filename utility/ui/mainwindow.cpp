/* Copyright © 2018 Alexander Arefyev */
/* License: MIT                       */
/* Contacts: alex.arefyev@gmail.com   */

/**
* \file   mainwidnow.cpp
* \author Alexander Arefyev
* \date   14.06.2018
*
*
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testee.h"
#include "core/colordb.h"
#include <QPalette>
#include <QFontDialog>
#include <QMdiSubWindow>
#include <QScrollBar>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

static
void printColors(QWidget * w) {

    QPalette pal = w->palette();

//    qDebug() << "Window:        " << pal.color(QPalette::Window).name();
//    qDebug() << "WindowText:    " << pal.color(QPalette::WindowText).name();
//    qDebug() << "Base:          " << pal.color(QPalette::Base).name();
//    qDebug() << "AlternateBase: " << pal.color(QPalette::AlternateBase).name();
//    qDebug() << "Text:          " << pal.color(QPalette::Text).name();
//    qDebug() << "ButtonText:    " << pal.color(QPalette::ButtonText).name();
    qDebug() << "Light:         " << pal.color(QPalette::Inactive, QPalette::Light).name();
    qDebug() << "Midlight:      " << pal.color(QPalette::Inactive, QPalette::Midlight).name();
    qDebug() << "Button:        " << pal.color(QPalette::Inactive, QPalette::Button).name();
    qDebug() << "Mid:           " << pal.color(QPalette::Inactive, QPalette::Mid).name();
    qDebug() << "Dark:          " << pal.color(QPalette::Inactive, QPalette::Dark).name();
//    qDebug() << "BrightText:    " << pal.color(QPalette::BrightText).name();

    qDebug("\n---------\n");

    QColor c = pal.color(QPalette::Inactive, QPalette::Button);
//    for (int i = 100; i < 250; i += 10) {
//        QColor cc = c.darker(i);
//        qDebug() << i << ": " << cc.name();
//    }
//    qDebug("\n---------\n");
    for (int i = 100; i < 120; i += 1) {
        QColor cc = c.lighter(i);
        qDebug() << i << ": " << cc.name();
    }
}

class FakeWidget : public QWidget
{
public:
    explicit FakeWidget(QWidget * parent = nullptr) : QWidget(parent) {}

protected:
    void closeEvent(QCloseEvent * event) override {
        event->ignore();
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);

    mTestee = new Testee();
    mSub0 = ui->mdiArea->addSubWindow(new FakeWidget());
    mSub0->setSystemMenu(nullptr);
    mSub0->setWindowFlags(Qt::CustomizeWindowHint);
    mSub0->resize(1, 1);

    mSub1 = ui->mdiArea->addSubWindow(mTestee);
    connect(mSub1, &QMdiSubWindow::aboutToActivate, this, [this]{
//        ui->colorChooser->setState(0);
        qDebug() << "!!! QMdiSubWindow::aboutToActivate !!!";
    });

    mSub1->setSystemMenu(nullptr);
    mSub1->setWindowFlags(Qt::CustomizeWindowHint);
    mSub1->move(32, 32);
    mSub1->resize(400, 400);

    mDb = new ColorDB();

    ui->colorChooser->setColorDB(mDb);

    setWindowTitle(ui->colorChooser->getFileName());

    connect(ui->colorChooser, &ColorChooser::paletteChanged, this, [this](){
        ui->mdiArea->setPalette(ui->colorChooser->getPalette());
        ui->mdiArea->verticalScrollBar()->setPalette(palette());
        ui->mdiArea->horizontalScrollBar()->setPalette(palette());
    });
    connect(ui->colorChooser, &ColorChooser::fontChanged, this, [this](){
        mTestee->setFont(ui->colorChooser->getFont());
    });
    connect(ui->colorChooser, &ColorChooser::stateChanged, this, [this](int state){
        setState(state);
    });
    connect(ui->colorChooser, &ColorChooser::modified, this, [this](bool value){
        updateWindowTitle();
        setWindowModified(value);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (ui->colorChooser->isWindowModified()) {
        QMessageBox::StandardButton btn =
                QMessageBox::question(
                    this, QStringLiteral(""),
                    tr("Modified. Save?"),
                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (btn == QMessageBox::Cancel)
            event->ignore();
        else
        if (btn == QMessageBox::Yes) {
            if (!ui->colorChooser->save())
                event->ignore();
        }
    }
}

void MainWindow::setState(int id)
{
    switch (id) {
    case -1:
    case  0:
        ui->mdiArea->setActiveSubWindow(mSub1);
//        mTestee->setEnabled(true);
        break;
    case 1:
        ui->mdiArea->setActiveSubWindow(mSub0);
//        mTestee->setEnabled(true);
        break;
    case 2:
        ui->mdiArea->setActiveSubWindow(mSub1);
//        mTestee->setEnabled(false);
        break;
    }
}

void MainWindow::updateWindowTitle()
{
    setWindowTitle(QStringLiteral("[*]") + ui->colorChooser->getFileName());
}

