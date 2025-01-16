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
        socket->connectToHost("192.168.186.1", 2323);
    }

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
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
                ui->textBrowser->append(parts[3]);
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
        ui->textBrowser->append("Socket not connected");
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

    SendToServer(message);

    ui->lineEdit->clear();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    QString num = item->text();
    Interlocutor = num;
}

void MainWindow::Socket_print() {
    for (auto i : Sockets) {
        QList<QListWidgetItem *> item =
            ui->listWidget->findItems(i, Qt::MatchExactly);
        if (item.isEmpty()) {
            ui->listWidget->addItem(i);
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
