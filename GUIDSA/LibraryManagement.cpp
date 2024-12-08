#include "LibraryManagement.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

LibraryManagement::LibraryManagement(QWidget *parent)
    : QWidget(parent)
{
    nameInput = new QLineEdit(this);
    bookInput = new QLineEdit(this);

    QLabel *checkAvailableBooksLabel = new QLabel("Available Books", this);
    QLabel *displayBorrowedBooksLabel = new QLabel("Display Borrowed Books", this);
    QLabel *displayReturnedBooksLabel = new QLabel("Recently Returned Books", this);
    QLabel *viewReservationsLabel = new QLabel("View Reservations", this);

    borrowBookButton = new QPushButton("Borrow Book", this);
    returnBookButton = new QPushButton("Return Book", this);
    addReservationButton = new QPushButton("Reserve Book", this);
    serveNextReservationButton = new QPushButton("Serve Next Reservation", this);
    claimReservationButton = new QPushButton("Claim Reservation", this);

    availableBooksList = new QListWidget(this);
    borrowedBooksList = new QListWidget(this);
    returnedBooksList = new QListWidget(this);
    reservationList = new QListWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *bookLayout = new QHBoxLayout;
    QHBoxLayout *reservationLayout = new QHBoxLayout;

    bookLayout->addWidget(bookInput);
    bookLayout->addWidget(borrowBookButton);
    bookLayout->addWidget(returnBookButton);
    bookLayout->setSpacing(10);

    reservationLayout->addWidget(nameInput);
    reservationLayout->addWidget(bookInput);
    reservationLayout->addWidget(addReservationButton);
    reservationLayout->setSpacing(10);

    mainLayout->addLayout(bookLayout);
    mainLayout->addWidget(checkAvailableBooksLabel);
    mainLayout->addWidget(availableBooksList);

    mainLayout->addWidget(displayBorrowedBooksLabel);
    mainLayout->addWidget(borrowedBooksList);

    mainLayout->addWidget(displayReturnedBooksLabel);
    mainLayout->addWidget(returnedBooksList);

    mainLayout->addWidget(viewReservationsLabel);
    mainLayout->addWidget(reservationList);

    mainLayout->addLayout(reservationLayout);
    mainLayout->addWidget(serveNextReservationButton);
    mainLayout->addWidget(claimReservationButton);

    setLayout(mainLayout);

    connect(borrowBookButton, &QPushButton::clicked, this, &LibraryManagement::borrowBook);
    connect(returnBookButton, &QPushButton::clicked, this, &LibraryManagement::returnBook);
    connect(addReservationButton, &QPushButton::clicked, this, &LibraryManagement::addReservation);
    connect(serveNextReservationButton,
            &QPushButton::clicked,
            this,
            &LibraryManagement::serveNextReservation);
    connect(claimReservationButton,
            &QPushButton::clicked,
            this,
            &LibraryManagement::claimReservation);

    availableBooks.push("Harry Potter");
    availableBooks.push("The Hobbit");
    availableBooks.push("Lord of the Rings");
    availableBooks.push("Deviant");

    checkAvailableBooks();
    displayBorrowedBooks();
    displayReturnedBooks();
    viewReservations();
}

void LibraryManagement::checkAvailableBooks()
{
    availableBooksList->clear();
    if (availableBooks.isEmpty()) {
        availableBooksList->addItem("No Available Books.");
    } else {
        QStack<QString> temp = availableBooks;
        while (!temp.isEmpty()) {
            availableBooksList->addItem(temp.pop());
        }
    }
}

void LibraryManagement::borrowBook()
{
    QString bookName = bookInput->text();
    QStack<QString> tempStack;
    bool found = false;

    while (!availableBooks.isEmpty()) {
        if (availableBooks.top() == bookName) {
            found = true;
            availableBooks.pop();
            borrowedBooks.push(bookName);
            break;
        } else {
            tempStack.push(availableBooks.top());
            availableBooks.pop();
        }
    }

    while (!tempStack.isEmpty()) {
        availableBooks.push(tempStack.pop());
    }

    if (found) {
        QMessageBox::information(this, "Success", "Book borrowed: " + bookName);
    } else {
        QMessageBox::warning(this, "Error", "Book not available.");
    }

    checkAvailableBooks();
    displayBorrowedBooks();
    displayReturnedBooks();
}

void LibraryManagement::returnBook()
{
    QString bookName = bookInput->text();
    QStack<QString> tempStack;
    bool found = false;

    while (!borrowedBooks.isEmpty()) {
        if (borrowedBooks.top() == bookName) {
            found = true;
            borrowedBooks.pop();
            returnedBooks.push(bookName);
            availableBooks.push(bookName);
            break;
        } else {
            tempStack.push(borrowedBooks.top());
            borrowedBooks.pop();
        }
    }

    while (!tempStack.isEmpty()) {
        borrowedBooks.push(tempStack.pop());
    }

    if (found) {
        QMessageBox::information(this, "Success", "Book returned: " + bookName);
    } else {
        QMessageBox::warning(this, "Error", "Book not found in borrowed books.");
    }

    checkAvailableBooks();
    displayBorrowedBooks();
    displayReturnedBooks();
}

void LibraryManagement::displayBorrowedBooks()
{
    borrowedBooksList->clear();
    if (borrowedBooks.isEmpty()) {
        borrowedBooksList->addItem("No borrowed books.");
    } else {
        QStack<QString> temp = borrowedBooks;
        while (!temp.isEmpty()) {
            borrowedBooksList->addItem(temp.pop());
        }
    }
}

void LibraryManagement::displayReturnedBooks()
{
    returnedBooksList->clear();
    if (returnedBooks.isEmpty()) {
        returnedBooksList->addItem("No recently returned books.");
    } else {
        QStack<QString> temp = returnedBooks;
        while (!temp.isEmpty()) {
            returnedBooksList->addItem(temp.pop());
        }
    }
}

void LibraryManagement::addReservation()
{
    QString name = nameInput->text();
    QString bookName = bookInput->text();

    if (name.isEmpty() || bookName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both your name and the book name.");
        return;
    }

    if (!availableBooks.contains(bookName)) {
        QMessageBox::warning(this, "Error", "Book is not available.");
        return;
    }

    reservationQueue.enqueue(name);
    reservedBooks.enqueue(bookName);
    availableBooks.removeOne(bookName);

    QMessageBox::information(this, "Reservation", name + " reserved: " + bookName);

    viewReservations();
    checkAvailableBooks();
}

void LibraryManagement::serveNextReservation()
{
    if (!reservationQueue.isEmpty()) {
        QString nextUser = reservationQueue.dequeue();
        QString book = reservedBooks.dequeue();
        borrowedBooks.push(book);
        QMessageBox::information(this,
                                 "Serving Reservation",
                                 "Serving reservation for: " + nextUser + " with book: " + book);
    } else {
        QMessageBox::information(this, "No Reservations", "No pending reservations.");
    }

    viewReservations();
}

void LibraryManagement::claimReservation()
{
    QString name = nameInput->text().trimmed();

    if (reservationQueue.isEmpty()) {
        QMessageBox::warning(this, "Error", "No reservations available.");
        return;
    }

    if (reservationQueue.head() == name) {
        qDebug() << "Claiming reservation for:" << name;
        qDebug() << "Front of reservation queue:" << reservationQueue.head();

        reservationQueue.dequeue();
        QString book = reservedBooks.dequeue();

        borrowedBooks.push(book);
        borrowedBooksList->addItem(book);

        QMessageBox::information(this,
                                 "Reservation Claimed",
                                 name + " claimed their reserved book: " + book);
    } else {
        qDebug() << "Mismatch! Input name:" << name << "Front of queue:" << reservationQueue.head();

        QMessageBox::warning(this,
                             "Error",
                             "It is not your turn to claim a reservation. "
                             "Only "
                                 + reservationQueue.head() + " can claim their book now.");
    }

    viewReservations();
    displayBorrowedBooks();
}

void LibraryManagement::viewReservations()
{
    reservationList->clear();
    if (reservationQueue.isEmpty()) {
        reservationList->addItem("No reservations.");
    } else {
        QQueue<QString> tempQueue = reservationQueue;
        QQueue<QString> tempBooks = reservedBooks;
        while (!tempQueue.isEmpty()) {
            reservationList->addItem(tempQueue.dequeue() + " - " + tempBooks.dequeue());
        }
    }
}
