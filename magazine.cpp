#include "magazine.h"
#include "libraryitem.h"
#include "libraryutils.h"

QTextStream outie(stdout);

 //Default constructor
Magazine::Magazine()
    : LibraryItem(), IssueNumber(0) {}

//Overload constructor
Magazine::Magazine(QString title, QString author, QString id, bool status, int issueNumber)
    : LibraryItem(title, author, id, status), IssueNumber(issueNumber) {}

//Virtual destructor
Magazine::~Magazine() {}

//Override pure virtual functions
QString Magazine::displayInfo() const
{
    return "\nTitle: " + Magazine::getTitle() +
           "\nAuthor: " + Magazine::getAuthor() +
           "\nID: " + Magazine::getID() +
           "\nIssue Number: " + QString::number(Magazine::getIssueNumber()) +
           "\nStatus: " + (Magazine::get_isBorrowed() ? "Borrowed" : "Available");
}

//Setters
void Magazine::setIssueNumber(int issueNumber)
{
    this -> IssueNumber = issueNumber;
}

//Getters
int Magazine::getIssueNumber() const
{
    return IssueNumber;
}
