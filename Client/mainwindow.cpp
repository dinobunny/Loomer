#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>
#include <QStringBuilder>
#include <QTimer>

enum MesageIdentifiers {

    ID_MY     = 02,
    ID_CLIENT = 03,
    ID_DELETE = 04,
    MESAGE    = 05

};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    if (socket->state() != QAbstractSocket::ConnectedState) {
        socket->connectToHost("192.168.1.168", 2323);
    }

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    this->setStyleSheet(
        "#MainWindow {"
        "background-color :rgb(32, 43, 54);"
        "}"
        );

    ui->listWidget->setStyleSheet(
        "QListWidget {"
        "    background-color: rgb(32, 43, 54);"  // Светло-синий фон
        "    color: white"
        "}"
        );

    ui->listWidget_2->setStyleSheet(
         "QListWidget {"
        "background-color: rgb(64, 93, 114)"
        "}"
        "QListWidget::item {"
        "   background-color: lightblue;"  // Светло-голубой фон
        "   border-radius: 15px;"  // Круглые углы
        "   padding: 10px;"  // Отступы внутри элемента (влияят на размер)
        "   font-size: 14px;"  // Размер шрифта
        "   color: black;"  // Цвет текста "
        "   max-width: 15px;"
        "   min-width: 15px;"
        "}"
        );

    ui->pushButton->setStyleSheet(
        "QPushButton {"
        "background-color: lightblue;"
        "}"
        );

    ui->lineEdit->setStyleSheet(
        "QLineEdit {"
        "background-color: lightblue;"
        "}"
        );



    ui->listWidget_2->setSpacing(7);

}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::slotReadyRead() {
    QDataStream in(socket);

    in.setVersion(QDataStream::Qt_6_0);

    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;


        QStringList parts = str.split(",");
        int messType = parts[0].toInt();

        switch (messType) {

            case ID_MY: // my_identifier
            {
                MySocket = parts[2];
                QString num = parts[2];
                setWindowTitle(num);
                qDebug() << "My socket" << MySocket;
                break;
            }

            case ID_CLIENT: // the_identifier
            {
                if (!Sockets.contains(parts[2])) {
                    Sockets.push_back(parts[2]);
                    MainWindow::Socket_print();
                }

                parts.clear();
                break;
            }

            case ID_DELETE: // delete_client;
            {
                Sockets.removeAll(parts[2]);
                MainWindow::Socket_delete(parts[2]);
                break;
            }

            case MESAGE: // message
            {
                ui->listWidget_2->addItem(parts[3]);
                break;
            }

        }
    }
}

void MainWindow::SendToServer(const QString &str) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        out << str;
        socket->write(data);
        qDebug() << socket;
    } else {
        ui->listWidget_2->addItem("Socket not connected");
    }
}

void MainWindow::on_pushButton_2_clicked() {
    SendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_returnPressed() {
    QString message = QString("%1,%2,%3,%4")
                          .arg(MESAGE)
                          .arg(Interlocutor)
                          .arg(MySocket)
                          .arg(ui->lineEdit->text());

    QListWidgetItem *item = new QListWidgetItem(ui->lineEdit->text());

    item->setTextAlignment(Qt::AlignRight);

    ui->listWidget_2->addItem(item);

    SendToServer(message);

    ui->lineEdit->clear();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    QString num = item->text();
    Interlocutor = num;
    ui->lineEdit->setFocus();
}

void MainWindow::Socket_print() {
    for (auto i : Sockets) {
        QList<QListWidgetItem *> item =
            ui->listWidget->findItems(i, Qt::MatchExactly);
        if (item.isEmpty()) {
            QListWidgetItem *item  = new QListWidgetItem(QIcon("user.png"), i);
            ui->listWidget->setIconSize(QSize(25, 25));

            ui->listWidget->addItem(item);
        }
    }
}

void MainWindow::Socket_delete(QString socket_to_delete) {

    QList<QListWidgetItem *> items =
        ui->listWidget->findItems(socket_to_delete, Qt::MatchExactly);
    if (!items.isEmpty()) {
        for (QListWidgetItem *item : items) {
            int row = ui->listWidget->row(item); // Получить строку элемента
            delete ui->listWidget->takeItem(row);
        }
    }
}
