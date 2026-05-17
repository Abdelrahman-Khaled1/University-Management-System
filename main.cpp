#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("School Management System");
    app.setOrganizationName("College");

    MainWindow win;
    win.show();

    return app.exec();
}
