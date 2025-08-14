#include "mainwindow.h"
#include "libraryutils.h"
#include "libraryitem.h"
#include "book.h"
#include "magazine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    qRegisterMetaType<LibraryItem*>("LibraryItem*");

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    //Use a guaranteed writable per-user app data dir (Qt 5.3: DataLocation) and ensure it exists
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if (dataDir.isEmpty()) {
        dataDir = QDir::homePath() + "/.local/share/Library_Management_System02";
    }
    QDir().mkpath(dataDir);

    //Build file paths in that directory
    dataFilePath = QDir(dataDir).filePath("librarydata.csv");
    idFilePath = QDir(dataDir).filePath("lastid.txt");

    //Create files on first run to avoid access warnings
    if (!QFileInfo::exists(dataFilePath)) {
        QFile f(dataFilePath);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) { f.close(); }
    }
    if (!QFileInfo::exists(idFilePath)) {
        LibraryUtils::storeLastID(idFilePath, "0000000000");
    }
    LibraryUtils::loadFile(dataFilePath, libraryData);

    //Initialize the library model
    libraryModel = new QStandardItemModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(libraryModel);

    //Welcome Page
    welcomePage = new QWidget(this);
    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomePage);

    QLabel *welcomeLabel = new QLabel("Welcome to \n The Library Management System");
    displayMenuButton = new QPushButton("Menu", welcomePage);

    welcomeLayout->addWidget(welcomeLabel);
    welcomeLayout->addWidget(displayMenuButton);

    connect(displayMenuButton, &QPushButton::clicked, this, &MainWindow::on_displayMenuButton_clicked);

    welcomePage->setLayout(welcomeLayout);

    //Main menu Page
    mainMenuPage = new QWidget(this);
    QVBoxLayout *mainMenuLayout = new QVBoxLayout(mainMenuPage);

    QLabel *mainMenuLabel = new QLabel("Please select an option");

    addButton = new QPushButton("Add", mainMenuPage);
    browseButton = new QPushButton("Browse Library", mainMenuPage);

    mainMenuLayout->addWidget(mainMenuLabel);
    mainMenuLayout->addWidget(addButton);
    mainMenuLayout->addWidget(browseButton);

    connect(addButton, &QPushButton::clicked, this, [this]{
        stackedWidget->setCurrentIndex(2);
    });

    connect(browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButton_clicked);

    mainMenuPage->setLayout(mainMenuLayout);

    //Add Page
    addPage = new QWidget(this);
    QVBoxLayout *addLayout = new QVBoxLayout(addPage);

    QLabel *addLabel = new QLabel("Add Item", addPage);
    itemTypeCombo = new QComboBox(addPage);
    itemTypeCombo->addItem("Book");
    itemTypeCombo->addItem("Magazine");

    //Form rows
    QHBoxLayout *titleRow = new QHBoxLayout();
    titleEdit = new QLineEdit(addPage);
    titleRow->addWidget(new QLabel("Title:", addPage));
    titleRow->addWidget(titleEdit);

    QHBoxLayout *authorRow = new QHBoxLayout();
    authorEdit = new QLineEdit(addPage);
    authorRow->addWidget(new QLabel("Author:", addPage));
    authorRow->addWidget(authorEdit);

    QHBoxLayout *genreRow = new QHBoxLayout();
    genreEdit = new QLineEdit(addPage);
    genreRow->addWidget(new QLabel("Genre (Books):", addPage));
    genreRow->addWidget(genreEdit);

    QHBoxLayout *issueRow = new QHBoxLayout();
    issueNumberedit = new QLineEdit(addPage);
    issueRow->addWidget(new QLabel("Issue Number (Magazines):", addPage));
    issueRow->addWidget(issueNumberedit);

    confirmAddButton = new QPushButton("Add", addPage);

    addLayout->addWidget(addLabel);
    addLayout->addWidget(itemTypeCombo);
    addLayout->addLayout(titleRow);
    addLayout->addLayout(authorRow);
    addLayout->addLayout(genreRow);
    addLayout->addLayout(issueRow);
    addLayout->addWidget(confirmAddButton);

    //Back button
    QVBoxLayout *backSearch = new QVBoxLayout();
    backSearch->addStretch();

    backAddButton = new QPushButton("back");

    backSearch->addWidget(backAddButton);
    addLayout->addLayout(backSearch);

    //Toggle relevant field enablement
    auto updateAddFieldEnablement = [this]{
        const bool isBook = itemTypeCombo->currentText() == "Book";
        genreEdit->setEnabled(isBook);
        issueNumberedit->setEnabled(!isBook);
    };
    updateAddFieldEnablement();
    connect(itemTypeCombo, &QComboBox::currentTextChanged, this, [this, updateAddFieldEnablement]{ updateAddFieldEnablement(); });

    //Confirm add handler
    connect(confirmAddButton, &QPushButton::clicked, this, [this]{
        const QString itemType = itemTypeCombo->currentText();
        const QString title = titleEdit->text().trimmed();
        const QString author = authorEdit->text().trimmed();
        const QString genre = genreEdit->text().trimmed();
        const QString issueStr = issueNumberedit->text().trimmed();

        if (title.isEmpty() || author.isEmpty()) {
            QMessageBox::warning(this, "Missing Data", "Title and Author are required.");
            return;
        }

        QString lastId = LibraryUtils::loadLastID(idFilePath);
        QString nextId = LibraryUtils::incrementID(lastId);

        if (itemType == "Book") {
            if (genre.isEmpty()) {
                QMessageBox::warning(this, "Missing Data", "Genre is required for Books.");
                return;
            }
            libraryData.append(new Book(title, author, nextId, false, genre));
        } else {
            bool ok = false;
            int issue = issueStr.toInt(&ok);
            if (!ok) {
                QMessageBox::warning(this, "Invalid Data", "Issue Number must be an integer for Magazines.");
                return;
            }
            libraryData.append(new Magazine(title, author, nextId, false, issue));
        }

        //Persist new item and last id
        LibraryUtils::storeFile(dataFilePath, libraryData);
        LibraryUtils::storeLastID(idFilePath, nextId);

        //Update the browse model if it exists
        if (libraryModel) {
            updateLibraryModel();
        }

        //Clear inputs
        titleEdit->clear();
        authorEdit->clear();
        genreEdit->clear();
        issueNumberedit->clear();

        QMessageBox::information(this, "Added", "Item added successfully.");
    });

    connect(backAddButton, &QPushButton::clicked, this, [this]{
        stackedWidget->setCurrentIndex(1);
    });

    addPage->setLayout(addLayout);

    //Setup browse page
    setupBrowsePage();

    //Stacked Widget Page Indexing
    stackedWidget->addWidget(welcomePage);
    stackedWidget->addWidget(mainMenuPage);
    stackedWidget->addWidget(addPage);
    stackedWidget->addWidget(browsePage);
    stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_displayMenuButton_clicked()
{
    if (stackedWidget)
    {
        stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_addButton_clicked()
{
    if (stackedWidget)
    {
        stackedWidget->setCurrentIndex(3);
    }
}

void MainWindow::on_browseButton_clicked()
{
    if (stackedWidget)
    {
        updateLibraryModel();
        stackedWidget->setCurrentIndex(3);
    }
}

void MainWindow::on_filterTextChanged(const QString &text)
{
    QString filterTypeText = filterType->currentText();

    if (filterTypeText == "All") {
        //Use case-insensitive regex for global filtering
        QRegExp regex(text, Qt::CaseInsensitive);
        proxyModel->setFilterRegExp(regex);
    } else {
        //Set filter to specific column based on filter type
        int column = 0;
        if (filterTypeText == "Title") column = 0;
        else if (filterTypeText == "Author") column = 1;
        else if (filterTypeText == "Genre") column = 3;
        else if (filterTypeText == "Issue Number") column = 3;
        else if (filterTypeText == "Status") column = 4;

        proxyModel->setFilterKeyColumn(column);
        //Use case-insensitive regex for column-specific filtering
        QRegExp regex(text, Qt::CaseInsensitive);
        proxyModel->setFilterRegExp(regex);
    }
}

void MainWindow::on_browseItemClicked(const QModelIndex &index)
{
    QModelIndex sourceIndex = proxyModel->mapToSource(index);
    if (sourceIndex.isValid()) {
        selectedBrowseItem = sourceIndex.data(Qt::UserRole).value<LibraryItem*>();
        if (selectedBrowseItem) {
            QMessageBox::information(this, "Item Details", selectedBrowseItem->displayInfo());
        }
    }
}

void MainWindow::on_browseBorrowClicked()
{
    if (!selectedBrowseItem) return;
    if (selectedBrowseItem->get_isBorrowed()) return;

    selectedBrowseItem->set_isBorrowed(true);
    LibraryUtils::storeFile(dataFilePath, libraryData);
    updateLibraryModel();
    QMessageBox::information(this, "Success", "Item borrowed successfully!");
}

void MainWindow::on_browseReturnClicked()
{
    if (!selectedBrowseItem) return;
    if (!selectedBrowseItem->get_isBorrowed()) return;

    selectedBrowseItem->set_isBorrowed(false);
    LibraryUtils::storeFile(dataFilePath, libraryData);
    updateLibraryModel();
    QMessageBox::information(this, "Success", "Item returned successfully!");
}

void MainWindow::setupBrowsePage()
{
    browsePage = new QWidget(this);
    QVBoxLayout *browseLayout = new QVBoxLayout(browsePage);

    //Header
    QLabel *browseLabel = new QLabel("Browse Entire Library", browsePage);
    browseLabel->setAlignment(Qt::AlignCenter);
    browseLayout->addWidget(browseLabel);

    //Filter controls
    QHBoxLayout *filterLayout = new QHBoxLayout();
    QLabel *filterLabel = new QLabel("Filter by:", browsePage);
    filterType = new QComboBox(browsePage);
    filterType->addItem("All");
    filterType->addItem("Title");
    filterType->addItem("Author");
    filterType->addItem("Genre");
    filterType->addItem("Issue Number");
    filterType->addItem("Status");

    filterEdit = new QLineEdit(browsePage);
    filterEdit->setPlaceholderText("Enter filter text...");

    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterType);
    filterLayout->addWidget(filterEdit);
    browseLayout->addLayout(filterLayout);

    //Library list view
    libraryListView = new QListView(browsePage);
    libraryListView->setModel(proxyModel);
    libraryListView->setSelectionMode(QAbstractItemView::SingleSelection);
    browseLayout->addWidget(libraryListView);

    //Action buttons
    QHBoxLayout *actionLayout = new QHBoxLayout();
    QPushButton *browseBorrowButton = new QPushButton("Borrow", browsePage);
    QPushButton *browseReturnButton = new QPushButton("Return", browsePage);
    backBrowseButton = new QPushButton("Back", browsePage);

    actionLayout->addWidget(browseBorrowButton);
    actionLayout->addWidget(browseReturnButton);
    actionLayout->addStretch();
    actionLayout->addWidget(backBrowseButton);
    browseLayout->addLayout(actionLayout);

    //Connect signals
    connect(filterEdit, &QLineEdit::textChanged, this, &MainWindow::on_filterTextChanged);
    connect(filterType, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        Q_UNUSED(text);
        on_filterTextChanged(filterEdit->text());
    });
    connect(libraryListView, &QListView::clicked, this, &MainWindow::on_browseItemClicked);
    connect(browseBorrowButton, &QPushButton::clicked, this, &MainWindow::on_browseBorrowClicked);
    connect(browseReturnButton, &QPushButton::clicked, this, &MainWindow::on_browseReturnClicked);
    connect(backBrowseButton, &QPushButton::clicked, this, [this]{
        stackedWidget->setCurrentIndex(1);
    });

    browsePage->setLayout(browseLayout);
}

void MainWindow::populateLibraryModel()
{
    libraryModel->clear();
    libraryModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Author" << "Type" << "Details" << "Status");

    for (LibraryItem* item : libraryData) {
        QList<QStandardItem*> row;

        QString title = item->getTitle();
        QString author = item->getAuthor();
        QString type = (dynamic_cast<Book*>(item)) ? "Book" : "Magazine";
        QString details;
        QString status = item->get_isBorrowed() ? "Borrowed" : "Available";

        // Get specific details based on item type
        if (Book* book = dynamic_cast<Book*>(item)) {
            details = "Genre: " + book->getGenre();
        } else if (Magazine* magazine = dynamic_cast<Magazine*>(item)) {
            details = "Issue: " + QString::number(magazine->getIssueNumber());
        }

        row << new QStandardItem(title)
            << new QStandardItem(author)
            << new QStandardItem(type)
            << new QStandardItem(details)
            << new QStandardItem(status);

        // Store the LibraryItem pointer in the first item's UserRole
        row.first()->setData(QVariant::fromValue(item), Qt::UserRole);

        libraryModel->appendRow(row);
    }
}

void MainWindow::updateLibraryModel()
{
    populateLibraryModel();
}
