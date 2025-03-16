#include "RegWindow.h"
#include "ui_regwindow.h"
#include "userdata.h"
#include "enums.h"
#include "serverconnector.h"

#include<QFile>


ServerConnector  serverConnector;
UserData& userData = UserData::getInstance();

RegWindow::RegWindow(const QString& configPath, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::regwindow)
    , serverConnector(new ServerConnector(nullptr, this))

{
    qDebug() << "RegWindow initialized with config: " << configPath;

    ui->setupUi(this);
    this->setStyleSheet(Style_Sheete());
    QIcon retrun_buton_icon("./images/Return_buton.png");
    this->ui->pushButton_return->setIcon(retrun_buton_icon);

    this->ui->label->setText("Log In in your account or create new one");

    StartUpWindow();

    serverConnector->ConnectToServer();
    connect(this, &RegWindow::CloseWindow, this,[this](){
        userData.mainWindStarted = true;
        this->close();
    });


}

RegWindow::~RegWindow()
{
    delete ui;
}

void RegWindow::StartUpWindow()
{
    ui->label->show();
    ui->pushButton_Log_in->show();
    ui->pushButton_Sing_up->show();


    ui->lineEdit_name->hide();
    ui->lineEdit_pass->hide();
    ui->pushButton_end->hide();
    ui->pushButton_return->hide();
}

QString RegWindow::Style_Sheete() {

    QString res = "" ;

    QFile styleFile("./styles/regstyle.qss"); // Убедитесь, что путь корректный
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        styleFile.close();
        res = styleSheet;
    }
    else qDebug() << "Style File not open";

    return res;
}

void RegWindow::on_pushButton_Log_in_clicked()
{
    userData.status = RegWind::LOG;
    ShowMainForm();
    ui->pushButton_end->setText("Log In");
}

void RegWindow::on_pushButton_Sing_up_clicked()
{
    userData.status = RegWind::SIGN;
    ShowMainForm();
    ui->pushButton_end->setText("Sing Up");
}


void RegWindow::ShowMainForm()
{
    ui->lineEdit_name->show();
    ui->lineEdit_pass->show();
    ui->pushButton_end->show();
    ui->pushButton_return->show();


    ui->pushButton_Log_in->hide();
    ui->pushButton_Sing_up->hide();
}




void RegWindow::on_pushButton_return_clicked()
{
    StartUpWindow();
}


void RegWindow::on_pushButton_end_clicked()
{

    userData.name = ui->lineEdit_name->text();
    userData.pass = ui->lineEdit_pass->text();

    int STATUS;

    if(userData.status == RegWind::LOG) {
        STATUS = LOG;
        serverConnector->SendMyData(STATUS);
    }
    else if (userData.status == RegWind::SIGN) {
        STATUS = SIGN;
    }


    // if (userData.getSocket() != nullptr){
    //     this->close();
    // }

}


