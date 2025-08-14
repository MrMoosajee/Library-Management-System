#ifndef MAGAZINE_H
#define MAGAZINE_H

#include <QTextStream>
#include <QString>
#include "libraryitem.h"

//Magazine class
//Inherits from LibraryItem
class Magazine: public LibraryItem
{
public:
    //Default constructor
    Magazine();

    //Overload constructor
    Magazine(QString title, QString author, QString id, bool status, int issueNumber);

    //Virtual destructor
    virtual ~Magazine();

    //Override pure virtual functions
    QString displayInfo() const override;

    //Setters
    void setIssueNumber(int IssueNumber);

    //Getters
    int getIssueNumber() const;

private:
    //Member Variable
    int IssueNumber;
};

#endif // MAGAZINE_H
