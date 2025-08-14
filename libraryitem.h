#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include <QString>
#include <QMetaType>

//LibraryItem class
//Parent class for derived classes
class LibraryItem
{
public:
    //Default constructor
    LibraryItem();
    //Overload constructor
    LibraryItem(QString title, QString author, QString id, bool status);
    //Virtual destructor
    //To accomodate for any pointers used
    virtual ~LibraryItem();

    //Setters
    void setTitle(QString title);
    void setAuthor(QString author);
    void setID(QString id);
    void set_isBorrowed(bool status);

    //Getters
    QString getTitle() const;
    QString getAuthor() const;
    QString getID() const;
    bool get_isBorrowed() const;

    //Pure virtual function for displaying info
    //Will be used by derived classes
    virtual QString displayInfo() const = 0;

protected:
    //Member variables
    QString title;
    QString author;
    QString id;
    bool isBorrowed;
};

Q_DECLARE_METATYPE(LibraryItem*)

#endif // LIBRARYITEM_H
