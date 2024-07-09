#include <QApplication>
#include "login_dialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LoginDialog loginDialog;



    if (loginDialog.exec() == QDialog::Accepted) {
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    }




    return 0;




}



