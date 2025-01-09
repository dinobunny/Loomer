#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QListWidget>
#include<QStringBuilder>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    if (socket->state() != QAbstractSocket::ConnectedState) {
        socket->connectToHost("192.168.186.1", 2323);
    }

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::slotReadyRead() {
    QDataStream in(socket);

    in.setVersion(QDataStream::Qt_6_0);

    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;

         ui->textBrowser->append(str);

        QString Identifier = str.left(2);
        int messType = Identifier.toInt();

        switch (messType) {

            case 02:  //my_identifier
            {
                QStringList parts = str.split(", ");
                QString num = parts[2];
                setWindowTitle(num);
                qDebug()<<"My socket"<<MySocket;
                break;
            }


            case 03: //the_identifier
            {
                QStringList parts = str.split(", ");
                if(!Sockets.contains(parts[2]))
                {
                    Sockets.push_back(parts[2]);
                    MainWindow::Socket_print();
                }

                 parts.clear();
                break;
            }


            case 04:   //message
            {
                QStringList parts = str.split(",");
                ui->textBrowser->append(parts[3]);
                break;
            }

            case 05:
            {
                QStringList parts = str.split(", ");
                Sockets.removeAll(parts[2]);
                MainWindow::Socket_delete(parts[2]);
            }

        }



         if (str.startsWith("tOrEmUvE ")) {
             qDebug() << "Delete request received";
             QString identifierPart = str.mid(QString("tOrEmUvE ").length());
             QStringList parts = identifierPart.split(", ");

             if (parts.size() > 1) {
                 QString valueToRemove = parts[1];  // Значення, яке потрібно знайти та видалити
                 qDebug()<<"To delete" << valueToRemove;
                 QList<QListWidgetItem*> items = ui->listWidget->findItems(valueToRemove, Qt::MatchExactly);

                 if (!items.isEmpty()) {
                     // Видаляємо всі знайдені елементи
                     for (QListWidgetItem* item : items) {
                         int row = ui->listWidget->row(item);
                         delete ui->listWidget->takeItem(row);
                     }
                     qDebug() << "Item with value" << valueToRemove << "deleted";
                 } else {
                     qDebug() << "Error: Item with value" << valueToRemove << "not found";
                 }
             } else {
                 qDebug() << "Error: Insufficient parts in the identifier";
             }
         }

         if(str.startsWith("AlOuSeComMuniCaTiOn ")){
             QString identifier = str.mid(QString("AlOuSeComMuniCaTiOn ,").length()).trimmed();
             qDebug()<<identifier << "reciver";
         }

    }

    else {
        ui->textBrowser->append("Read error");
    }
}


void MainWindow::SendToServer(const QString& str) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_0);
        out << str;
        socket->write(data);
        qDebug()<<socket;
        // this->setWindowTitle(data);
    }
    else {
        ui->textBrowser->append("Socket not connected");
    }
}


void MainWindow::on_pushButton_2_clicked() {
    SendToServer(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::on_lineEdit_returnPressed() {
    // Формируем строку сообщения
    QString message = QString("04,%1,%2,%3")
                          .arg(Interlocutor)
                          .arg(MySocket)
                          .arg(ui->lineEdit->text());


    SendToServer(message);

    ui->lineEdit->clear();
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString num = item->text();
    Interlocutor = num;
}




void MainWindow::Socket_print(){
    for(auto i : Sockets){
        QList<QListWidgetItem*> item = ui->listWidget->findItems(i, Qt::MatchExactly);
        if(item.isEmpty()){
            ui->listWidget->addItem(i);
        }

    }
}

void MainWindow::Socket_delete(QString socket_to_delete){

        QList<QListWidgetItem*> items = ui->listWidget->findItems(socket_to_delete, Qt::MatchExactly);
        if(!items.isEmpty()){
            for (QListWidgetItem* item : items) {
                int row = ui->listWidget->row(item); // Получить строку элемента
                delete ui->listWidget->takeItem(row);
            }
        }

}
