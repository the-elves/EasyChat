#include <iostream>
#include <QTime>
#include <QScrollBar>
#include <QCloseEvent>
#include "chatclient.h"
#include "ui_chatclient.h"
#include <QSignalMapper>
#include <QHostAddress>
#include <QMessageBox>
std::vector<QPushButton*> ChatClient::onlineUsers;
std::vector<Unicast*> ChatClient::unichat;

ChatClient::ChatClient(QString subject, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatClient) {
    ui->setupUi(this);
    this->subject = subject;
    ui->pushButtonConnect->setEnabled(false);
    ui->textBrowserChat->hide();
    ui->pushButtonSend->hide();
    ui->textEditMsg->hide();
    ui->labelOnlineUsers->hide();
}

void ChatClient::heartBeatSend(){
    heartBeatReceived = false;
    const QString s = "###heartbeat";
    writeMessage(s);
    std::cout<<"Heartbeat sent"<<std::endl;
}

void ChatClient::heartBeatReceive(){
    if(heartBeatReceived)
        std::cout<<"heartbeat received"<<std::endl;
    else{
        std::cout<<"heartbeat not received"<<std::endl;
    }
}

ChatClient::~ChatClient() {
    client.close();
    delete ui;
}

void ChatClient::start(const QString& msg) {
    //std::cout << "ChatClient start ";
    QSignalMapper* mapper = new QSignalMapper(this);
    connect(&client, SIGNAL(connected()), mapper, SLOT(map()));
    mapper->setMapping(&client, msg);
    connect(mapper, SIGNAL(mapped(const QString&)), this, SLOT(writeMessage(const QString&)));
    connect(&client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    QTimer *heartBeatSendTimer;
    heartBeatSendTimer = new QTimer(this);
    connect(heartBeatSendTimer, SIGNAL(timeout()), this, SLOT(heartBeatSend()));
    QTimer *heartBeatReceiveTimer;
    heartBeatReceiveTimer = new QTimer(this);
    connect(heartBeatReceiveTimer, SIGNAL(timeout()), this, SLOT(heartBeatReceive()));
    heartBeatSendTimer->start(5000);
    QThread::sleep(2);
    heartBeatReceiveTimer->start(5000);

}

void ChatClient::startConnection(QString address, quint16 port) {
    //std::cout << "ChatClient startconnection ";
    QHostAddress addr(address);
    client.connectToHost(addr, port);
}

void ChatClient::writeMessage(const QString& msg) {
    //std::cout << "ChatClient transfer ";
    const QString* msgQStr = &msg;
    std::string messageStr = msgQStr->toStdString();
    const char* messageCh = messageStr.c_str();
    client.write(messageCh, 50);
}

void ChatClient::readyRead() {

    while(client.bytesAvailable() > 0) {
        QString str = client.readAll();
        if(str.toStdString().substr(0, 3) == "!@#") {
            foreach( QPushButton* button, ChatClient::onlineUsers )  {
                button->hide();
            }
            onlineUsers.clear();
            QStringList users = str.split("!@#");
            for(int i = 0; i < users.size(); i++) {
                QPushButton *user = new QPushButton(this);
                user->setText(users.at(i));
                if(users.at(i) == ui->lineEditName->text()) {
                    QFont font = user->font();
                    font.setBold(true);
                    user->setFont(font);
                }
                user->setFlat(true);
                connect(user,SIGNAL(clicked()),this,SLOT(unicast()));
                ChatClient::onlineUsers.push_back(user);
            }
            int i = 1;
            foreach( QPushButton* button, ChatClient::onlineUsers )  {
                button->setGeometry(QRect(360, i * 20, 67, 17));
                button->show();
                i++;
            }
        }
        else if(str.contains("$%^")) {
            std::vector<QPushButton*>::iterator i;
            for(i = ChatClient::onlineUsers.begin(); i != ChatClient::onlineUsers.end(); i++) {
                if(str.left(str.indexOf(":")) == (*i)->text()) {
                    (*i)->click();
                    std::vector<Unicast*>::iterator j;
                    for(j = ChatClient::unichat.begin(); j != ChatClient::unichat.end(); j++) {
                        if(str.left(str.indexOf(":")) == (*j)->getSubject()) {
                            connect(this, SIGNAL(sendMsgToUnicast(QString)), (*j), SLOT(receiveMessage(QString)));
                            emit ChatClient::sendMsgToUnicast(str.remove(ui->lineEditName->text() + "$%^", Qt::CaseInsensitive));
                            disconnect(this, SIGNAL(sendMsgToUnicast(QString)), (*j), SLOT(receiveMessage(QString)));
                        }
                    }
                }
            }
        }
        else if(str.contains("###heartbeat")){
            heartBeatReceived = true;
        }
        else {
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + str + "\n");
        }
        std::cout<<str.toStdString()<<std::endl;
    }



}

void ChatClient::unicast() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    QString str = button->text();
    if(str != ui->lineEditName->text()) {
        std::cout<< str.toStdString();
        if(ChatClient::unichat.empty()) {
            Unicast *uni = new Unicast(str);
            uni->show();
            ChatClient::unichat.push_back(uni);
            connect(uni, SIGNAL(sendMessage(QString)), this, SLOT(writeMessage(QString)));
        }
        else {
            int flag = 0;
            std::vector<Unicast*>::iterator i;
            for(i = ChatClient::unichat.begin(); i != ChatClient::unichat.end(); i++) {
                if(str == (*i)->getSubject()) {
                    //std::cout<<"Unicast if";
                    (*i)->show();
                    (*i)->activateWindow();
                    flag = 1;
                }
            }
            if(!flag) {
                Unicast *uni = new Unicast(str);
                uni->show();
                ChatClient::unichat.push_back(uni);
                connect(uni, SIGNAL(sendMessage(QString)), this, SLOT(writeMessage(QString)));
            }
        }
    }
}

void ChatClient::on_pushButtonSend_clicked() {
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + ui->lineEditName->text() + ": " + ui->textEditMsg->toPlainText() + "\n");
    ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
    writeMessage(ui->textEditMsg->toPlainText());
    ui->textEditMsg->setText("");
}

void ChatClient::on_textEditMsg_textChanged() {
    if(ui->textEditMsg->toPlainText() == "")
        ui->pushButtonSend->setEnabled(false);
    else
        ui->pushButtonSend->setEnabled(true);
}

void ChatClient::on_lineEditName_textChanged() {
    if(ui->lineEditName->text() == "")
        ui->pushButtonConnect->setEnabled(false);
    else
        ui->pushButtonConnect->setEnabled(true);
}

void ChatClient::on_pushButtonConnect_clicked() {
    start(ui->lineEditName->text());
    startConnection("127.0.0.1", 1234);
    this->setWindowTitle(ui->lineEditName->text());
    ui->textBrowserChat->show();
    ui->pushButtonSend->show();
    ui->pushButtonSend->setEnabled(false);
    ui->textEditMsg->show();
    ui->labelOnlineUsers->show();
    ui->lineEditName->hide();
    ui->pushButtonConnect->hide();
    ui->labelName->hide();
    ui->textEditMsg->setText("");
}

void ChatClient::closeEvent(QCloseEvent *event) {
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient", tr("Are you sure?"), QMessageBox::Cancel | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        writeMessage("###close" + ui->lineEditName->text());
        event->accept();
    }
}
