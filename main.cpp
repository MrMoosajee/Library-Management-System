#include "mainwindow.h"
#include "libraryitem.h"
#include <QApplication>
#include <QVariant>
#include <QMetaType>

int main(int argc, char *argv[])
{
    qRegisterMetaType<LibraryItem*>("LibraryItem*");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
