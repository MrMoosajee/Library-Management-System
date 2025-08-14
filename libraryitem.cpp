#include "libraryitem.h"
#include "libraryutils.h"

//Default constructor
LibraryItem::LibraryItem()
    : title (" "), author(" "), id("0000000000"), isBorrowed(false) {}

//Overload constructor
LibraryItem::LibraryItem(QString title, QString author, QString id, bool status)
    :title(title), author(author), id(id), isBorrowed(status) {}

//Virtual destructor
//To accomodate for any pointers used
LibraryItem::~LibraryItem() {}

//Setters
void LibraryItem::setTitle(QString title)
{
    this -> title = title;
}

void LibraryItem::setAuthor(QString author)
{
    this -> author = author;
}

void LibraryItem::setID(QString id)
{
    this -> id = id;
}

void LibraryItem::set_isBorrowed(bool status)
{
    isBorrowed = status;
}

//Getters
QString LibraryItem::getTitle() const
{
    return title;
}

QString LibraryItem::getAuthor() const
{
    return author;
}

QString LibraryItem::getID() const
{
    return id;
}

bool LibraryItem::get_isBorrowed() const
{
    return isBorrowed;
}
