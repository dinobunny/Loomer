#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidget>
#include <QStringBuilder>
#include <QDir>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <enums.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    qDebug() << QCoreApplication::applicationDirPath();

    Read_Config(socket); // read from config

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    this->setStyleSheet(Style_Sheete());




    QIcon buton_icon(Get_Path(IMAGED, BUTON));
    ui->pushButton->setIcon(buton_icon);

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

void MainWindow::on_pushButton_clicked() { on_lineEdit_returnPressed(); }

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
            QString appDir = QCoreApplication::applicationDirPath();
            QListWidgetItem *item =
                new QListWidgetItem(QIcon(Get_Path(USER, IMAGED)), i);
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

QString MainWindow::Get_Path(qintptr target, qintptr directory) {
    QString path;
    QString file;
    QString extention;

    if (target == IMAGED) {
        path = "images";
        extention = ".png";
    }
    if (target == STYLES) {
        path = "styles";
        extention = ".qss";
    }
    if (target == CONFIG) {
        file = "config";
        extention = ".json";
        path = "NO";
    }
    if (directory == USER) {
        file = "user";
    }
    if (directory == BUTON) {
        file = "send";
    }
    if (directory == STYLE) {
        file = "style";
    }

    QString appDir = QCoreApplication::applicationDirPath();
    QDir baseDir(appDir);
    baseDir.cdUp(); // Поднимаемся к папке build
    baseDir.cdUp(); // Поднимаемся к папке Client
    baseDir.cdUp();
    if (path != "NO")
        baseDir.cd(path);

    QString filePath = baseDir.filePath(file + extention);
    qDebug() << "File path:" << filePath;

    return filePath;
}

QString MainWindow::Style_Sheete() {
    QFile styleFile(Get_Path(STYLES, STYLE)); // Убедитесь, что путь корректный
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        styleFile.close();
        return styleSheet;
    }
    qDebug() << "Style File not open";
}

void MainWindow::Read_Config(QTcpSocket *socket) {
    QFile file(Get_Path(CONFIG, -1));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error open config";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Парсим JSON
    QJsonDocument config_json = QJsonDocument::fromJson(data);

    if (config_json.isNull()) {
        qDebug() << "Error worck with config";
        return;
    }

    QJsonObject config_obj = config_json.object();

    QString server_ip =
        config_obj.value("Settings").toObject().value("server-ip").toString();
    qint16 server_port =
        config_obj.value("Settings").toObject().value("server-port").toInt();

    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << server_ip;
        qDebug() << server_port;
        socket->connectToHost(server_ip, server_port);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->listWidget_2->addItem("Clivked");
}

