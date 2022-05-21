#include "ui/mainwindow.h"
#include "../qtcolorizer/qtcolorizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtColorizer c(a.palette(), a.font());
    c.load(QStringLiteral(":/colorizer.json"));
    c.apply(&a);

    MainWindow w;

    c.apply(&w);
    w.show();

    return a.exec();
}
