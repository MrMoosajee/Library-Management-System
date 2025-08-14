#include "libraryutils.h"
#include <QVariant>

void LibraryUtils::storeLastID(const QString &fileName, const QString &id)
{
    QFile lastID(fileName);

    if (!lastID.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "File not accessable");
    }

    QTextStream idOut(&lastID);
    idOut << id;

    lastID.close();
}

QString LibraryUtils::loadLastID(const QString &fileName)
{
    QFile lastID(fileName);

    if (!lastID.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "0000000000";
    }

    QTextStream idIn(&lastID);
    QString line = idIn.readLine().trimmed();

    lastID.close();

    return line;
}

QString LibraryUtils::incrementID(const QString &id)
{
    int size = id.size();
    QString newID = id;

    for (int i = size - 1; i >= 0; i--)
    {
        if (newID[i] < '9')
        {
            newID[i] = newID[i].unicode() + 1;
            break;
        }
        else
        {
            newID[i] = '0';
        }
    }

    return newID;
}

void LibraryUtils::addLibraryItemToList(QListWidget *widget, LibraryItem* item)
{
    QListWidgetItem* libraryItems = new QListWidgetItem (item->getTitle() + " by " + item->getAuthor());

    libraryItems->setData(Qt::UserRole, QVariant::fromValue(item));
    widget->addItem(libraryItems);
}

void LibraryUtils::loadFile(const QString &fileName, QVector<LibraryItem*> &libraryData)
{
    libraryData.clear();

    QFile loaded(fileName);

    if (!loaded.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Library data not accessible", "The intended library data has not loaded");
        return;
    }
    if (loaded.size() == 0)
    {
        QMessageBox::information(nullptr, "Empty", "This Library System is empty");
    }

    QTextStream libraryIn(&loaded);
    while (!libraryIn.atEnd())
    {
        QString line = libraryIn.readLine();
        QStringList parts = line.split(",");

        if (parts.size() != 5) continue;
        bool ok;
        int checkIfInt = parts[4].trimmed().toInt(&ok);
        QString borrowedStr = parts[3].trimmed().toLower();
        bool b = (borrowedStr == "true" || borrowedStr == "1");

        if (ok) //If last part corresponds to Issue Number
        {
            libraryData.append(new Magazine(parts[0].trimmed(), parts[1].trimmed(), parts[2].trimmed(), b, checkIfInt));
        }
        else //If last part is a String
        {
            libraryData.append(new Book(parts[0].trimmed(), parts[1].trimmed(), parts[2].trimmed(), b, parts[4].trimmed()));
        }
    }

    loaded.close();
}

QString LibraryUtils::storeFile(const QString &fileName, const QVector<LibraryItem*> &libraryData)
{
    QFile loaded(fileName);

    if(!loaded.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error:", "File could not be saved, Library Data not updated");
        return QString("Error: could not open file for writing");
    }

    QTextStream libraryOut(&loaded);

    for (LibraryItem* item : libraryData)
    {
        const Book* book = dynamic_cast<const Book*>(item);
        if (book)
        {
            libraryOut << book->getTitle() << ","
                       << book->getAuthor() << ","
                       << book->getID() << ","
                       << book->get_isBorrowed() << ","
                       << book->getGenre()
                       << "\n";
            continue;
        }

        const Magazine* magazine = dynamic_cast<const Magazine*>(item);
        if (magazine)
        {
            libraryOut << magazine->getTitle() << ","
                       << magazine->getAuthor() << ","
                       << magazine->getID() << ","
                       << magazine->get_isBorrowed() << ","
                       << magazine->getIssueNumber()
                       << "\n";
            continue;
        }
    }

    loaded.close();

    return "Saving Successful";
}

QList<LibraryItem*> LibraryUtils::performSearch(const QVector<LibraryItem*>& library, const QString& searchType, const QString& searchValue)
{
    QList<LibraryItem*> results;

    for (LibraryItem* item : library)
    {
        if (searchType == "Title" && item->getTitle().contains(searchValue, Qt::CaseInsensitive))
        {
            results.append(item);
        }
        else if (searchType == "Author" && item->getAuthor().contains(searchValue, Qt::CaseInsensitive))
        {
            results.append(item);
        }
        else if (searchType == "Genre")
        {
            const Book* book = dynamic_cast<const Book*>(item);
            if (book && book->getGenre().contains(searchValue))
            {
                results.append(item);
            }
        }
        else if (searchType == "Issue Number")
        {
            const Magazine* magazine = dynamic_cast<const Magazine*>(item);
            if (magazine && QString::number(magazine->getIssueNumber()) == searchValue)
            {
                results.append(item);
            }
        }
    }

    return results;
}

void LibraryUtils::updateSearchList(QListWidget *targetWidget, const QList<LibraryItem*>& items)
{
    if (targetWidget == nullptr)
    {
        return;
    }
    targetWidget->clear();
    for (LibraryItem* item : items)
    {
        addLibraryItemToList(targetWidget, item);
    }
}
