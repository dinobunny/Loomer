#include <QApplication>
#include "mainwindow.h"
#include "RegWindow.h"
#include "UserData.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Инициализируем синглтон
    UserData& userdata = UserData::getInstance();  // Получаем экземпляр синглтона

    RegWindow* regwindow = new RegWindow();  // Создаем окно регистрации динамически
    regwindow->show();

    int regwindresult = app.exec(); // Ожидаем, что вернется результат после закрытия окна регистрации

    // Закрываем и удаляем окно регистрации, если оно закрыто с результатом 0
    if (regwindresult == 0) {
        delete regwindow;  // Удаляем объект окна регистрации
        MainWindow mainwindow;  // Создаем и показываем основное окно
        mainwindow.show();
        return app.exec();
    }

    delete regwindow;  // Если регистрация не прошла, удаляем окно регистрации
    return regwindresult;
}
