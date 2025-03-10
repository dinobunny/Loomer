#include <QApplication>
#include "mainwindow.h"
#include "RegWindow.h"
#include "UserData.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Инициализируем синглтон
    UserData& userdata = UserData::getInstance();  // Получаем экземпляр синглтона

    RegWindow regwindow;
    regwindow.show();

    int regwindresult = app.exec(); // Обтимизировать

    if (regwindresult == 0) {
        MainWindow mainwindow;
        mainwindow.show();

        return app.exec();
    }

    return regwindresult;
}
