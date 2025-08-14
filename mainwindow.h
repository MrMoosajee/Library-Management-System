#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QStackedWidget>
#include <QLabel>
#include <QMetaType>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>
#include <QString>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "libraryutils.h"
#include "libraryitem.h"
#include "book.h"
#include "magazine.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //Data handle
    QString dataFilePath;

    //Stacked Widget for multiple pages
    QStackedWidget *stackedWidget;

    //Widget Pages
    QWidget *welcomePage;
    QWidget *mainMenuPage;
    QWidget *addPage;
    QWidget *browsePage; // Browse entire library

    //Buttons
    QPushButton *displayMenuButton;
    QPushButton *addButton;
    QPushButton *browseButton; // Browse button
    QPushButton *confirmAddButton;
    QPushButton *backAddButton;
    QPushButton *backBrowseButton; // Back button for browse page

    //Button line edits
    QLineEdit *titleEdit;
    QLineEdit *authorEdit;
    QLineEdit *genreEdit;
    QLineEdit *issueNumberedit;
    QLineEdit *filterEdit;

    //Combo boxes
    QComboBox *itemTypeCombo;
    QComboBox *filterType;
    //Data Storage
    QVector<LibraryItem*> libraryData;
    QString idFilePath;

    //QListView and models for browsing
    QListView *libraryListView;
    QStandardItemModel *libraryModel;
    QSortFilterProxyModel *proxyModel;

    //Currently selected browse item
    LibraryItem *selectedBrowseItem = nullptr;

private slots:
    void on_displayMenuButton_clicked();
    void on_addButton_clicked();
    void on_browseButton_clicked();
    void on_filterTextChanged(const QString &text);
    void on_browseItemClicked(const QModelIndex &index);
    void on_browseBorrowClicked();
    void on_browseReturnClicked();

private:
    //Helper method
    void setupBrowsePage();
    void populateLibraryModel();
    void updateLibraryModel();
};

#endif // MAINWINDOW_H
