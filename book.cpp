#include "book.h"
#include "libraryitem.h"
#include "libraryutils.h"

QTextStream out(stdout);

//Default constructor
Book::Book()
    : LibraryItem::LibraryItem(), genre(" ") {}

//Overload constructor
Book::Book(QString title, QString author, QString id, bool status, QString genre)
    : LibraryItem::LibraryItem(title, author, id, status), genre(genre) {}

//Virtual destructor
Book::~Book() {}

//Override pure virtual functions
QString Book::displayInfo() const
{
    return  "\nTitle: " + Book::getTitle() +
            "\nAuthor: " + Book::getAuthor() +
            "\nID: " + Book::getID() +
            "\nGenre: " + Book::getGenre() +
            "\nStatus: " + (Book::get_isBorrowed() ? "Borrowed" : "Available");
}

//Setters
void Book::setGenre(QString genre)
{
    this -> genre = genre;
}

//Getters
QString Book::getGenre() const
{
    return genre;
}
