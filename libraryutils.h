#ifndef LIBRARYUTILS_H
#define LIBRARYUTILS_H

#include <QString>
#include <QWidget>
#include <QVector>
#include <QListWidget>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QVariant>
#include "libraryitem.h"
#include "book.h"
#include "magazine.h"

class LibraryUtils
{
public:
    LibraryUtils() = delete;
    //ID Storage
    static void storeLastID(const QString &fileName, const QString &id);
    static QString loadLastID(const QString &fileName);

    //File Storage
    static void loadFile(const QString &fileName, QVector<LibraryItem*> &libraryData);
    static QString storeFile(const QString &fileName, const QVector<LibraryItem*> &libraryData);

    //Unique ID creator
    static QString incrementID(const QString &id);

    //Add Library Items to list
    static void addLibraryItemToList(QListWidget *widget, LibraryItem* item);

    //Search
    static QList<LibraryItem*> performSearch(const QVector<LibraryItem*>& library,
                                             const QString& searchType,
                                             const QString& searchValue);
    static void updateSearchList(QListWidget *targetWidget, const QList<LibraryItem*>& items);
};

#endif // LIBRARYUTILS_H
