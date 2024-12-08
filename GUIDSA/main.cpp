#include <QApplication>
#include "LibraryManagement.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LibraryManagement window;
    window.setWindowTitle("Library Management System");
    window.resize(400, 600);
    window.show();

    return app.exec();
}
