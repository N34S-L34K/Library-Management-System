#ifndef LIBRARYMANAGEMENT_H
#define LIBRARYMANAGEMENT_H

#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QQueue>
#include <QStack>
#include <QString>
#include <QWidget>

class LibraryManagement : public QWidget
{
    Q_OBJECT

public:
    LibraryManagement(QWidget *parent = nullptr);

private slots:
    void checkAvailableBooks();
    void borrowBook();
    void returnBook();
    void displayBorrowedBooks();
    void displayReturnedBooks();
    void addReservation();
    void serveNextReservation();
    void claimReservation();
    void viewReservations();

private:
    QStack<QString> availableBooks;
    QStack<QString> borrowedBooks;
    QStack<QString> returnedBooks;
    QQueue<QString> reservationQueue;
    QQueue<QString> reservedBooks;

    QLineEdit *nameInput;
    QLineEdit *bookInput;
    QListWidget *availableBooksList;
    QListWidget *borrowedBooksList;
    QListWidget *returnedBooksList;
    QListWidget *reservationList;

    QPushButton *borrowBookButton;
    QPushButton *returnBookButton;
    QPushButton *addReservationButton;
    QPushButton *serveNextReservationButton;
    QPushButton *claimReservationButton;
};

#endif // LIBRARYMANAGEMENT_H
