#ifndef BOOK_H
#define BOOK_H

#include <QTextStream>
#include <QString>
#include "libraryitem.h"

//Book class inheriting from LibraryItem
class Book : public LibraryItem
{
public:
    //Default constructor
    Book();

    //Overload constructor
    Book(QString title, QString author, QString id, bool status, QString genre);

    //Virtual destructor
    virtual ~Book();

    //Override pure virtual functions
    QString displayInfo() const override;

    //Setters
    void setGenre(QString genre);

    //Getters
    QString getGenre() const;

private:
    //Member variable
    QString genre;
};

#endif // BOOK_H
