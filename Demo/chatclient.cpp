#include <iostream>
#include <QTime>
#include <QScrollBar>
#include <QCloseEvent>
#include "chatclient.h"
#include "ui_chatclient.h"
#include <QSignalMapper>
#include <QHostAddress>
#include <QtGlobal>
#include <host.h>
#include <QMessageBox>
std::vector<QPushButton*> ChatClient::onlineUsers;
std::vector<Unicast*> ChatClient::unichat;

ChatClient::ChatClient(QString subject, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatClient) {
    ui->setupUi(this);
    this->subject = subject;
    this->isFallback = false;
    ui->pushButtonConnect->setEnabled(false);
    ui->textBrowserChat->hide();
    ui->pushButtonSend->hide();
    ui->textEditMsg->hide();
    handlingRequest=false;
    ui->labelOnlineUsers->hide();
    Ip = "127.0.0.1";
}

void ChatClient::heartBeatSend(){
    heartBeatReceived = false;
    const QString s = "###heartbeat";
    writeMessage(s);
    std::cout<<"Heartbeat sent"<<std::endl;
}

void ChatClient::heartBeatReceive(){
    if(heartBeatReceived){
        std::cout<<"heartbeat received"<<std::endl;
    }
    else{
        modeCentralized = false;
        std::cout<<"heartbeat not received"<<std::endl;
        emit stopTimers();
        client.disconnectFromHost();
        client.close();
        if(this->isFallback) {
            initializeIncoming();
            this->startFallbackServer();
            Host a;
            a.ip = Ip;
            a.port = inPort;
            ring.append(a);
        }
        else {
            int seed = 0;
            QString s = this->windowTitle();
            char a[50] ;
            strcpy(a, s.toStdString().c_str());
            for(int i = 0;i<s.length();i++){
                seed = seed * 10 + a[i];
            }
            qsrand(seed);
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "seed set to = " +QString::number(seed) + "\n");
            QThread::sleep(5 + qrand()%3);
            initializeIncoming();
            this->connectToFallbackServer();
        }
    }
}

//common
void ChatClient::initializeIncoming(){
   incomming = new QTcpServer(this);
   qsrand(QTime::currentTime().msec());
   inPort = 1024 + qrand()%(65535 - 1024);
   if(!incomming->listen(QHostAddress::Any, inPort)){
       //std::cout << "incomming server not started";
       ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " incoming server failed " +QString::number(inPort) + "\n");
   }
   else{
       ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "server ruuning on My local Ip" +QString::number(inPort) + "\n");
      // std::cout << "incomming server server running";
   }
   connect(incomming, SIGNAL(newConnection()), this, SLOT(incommingNewConnection()));
}


//new connection on incomming port for client
void ChatClient::incommingNewConnection(){
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "someone connected on incommming "+ "\n");
    peerToPeerIncomming = incomming->nextPendingConnection();
    connect(peerToPeerIncomming,SIGNAL(readyRead()),this, SLOT(msgFromPrevious()));

}

//onclient message received from previous node in chain
void ChatClient::msgFromPrevious(){
    while(peerToPeerIncomming->bytesAvailable() > 0){
        QString str = peerToPeerIncomming->readAll();
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "raw msg Received" + str + "\n");
        //<originator=
        QString temp = str.remove(0, 12);
        QString name = temp.mid(temp.indexOf(">") + 1, temp.length());
        if(!(this->windowTitle() == name)){
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + name + ":" + str + "\n");
            outgoing->write(str.toStdString().c_str());
        }
    }
}

//onclinet
void ChatClient::connectToFallbackServer() {
    QHostAddress addr(this->Ip);
    peer.connectToHost(addr, port);
    if(peer.waitForConnected(5000))
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "fallback connected" + "\n");
    else
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "fallback not connected" + QString::number(port) + this->Ip + "\n");
    connect(&peer, SIGNAL(readyRead()), this, SLOT(fallbackReadyRead()));
    QString str = "$$$" + Ip + "$" + QString::number(inPort);
    peer.write(str.toStdString().c_str());
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "sent my info "+str + "\n");
}

//on client read form fallback server
void ChatClient::fallbackReadyRead() {
    while(peer.bytesAvailable() > 0)
    {
        QString str = peer.readAll();
        //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + str + "\n");
        //youconnectto
        if(str.startsWith("youconnecto")){
            outgoing = new QTcpSocket();
            QString temp = str.remove(0, 11);
            QString rIp= temp.left(temp.indexOf("#"));
            int rport = temp.mid(temp.indexOf("#") + 1, temp.length()).toInt();
            QHostAddress addr(rIp);
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "attempt|" +QString::number(rport)+":" + rIp + "|\n");
            outgoing->connectToHost(addr, rport);
            if(outgoing->waitForConnected(5000))
                ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "attempt succss" +QString::number(rport)+":" + rIp + "\n");
            else
                ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "attempt failed" +QString::number(rport)+":" + rIp + "\n");

        }
        else if(str.startsWith("takenewsockaddr")){
            initializeIncoming();
            QString str = "$$$" + Ip + "$" + QString::number(inPort);
            peer.write(str.toStdString().c_str());
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "sent my info "+str + "\n");
        }
    }
}
//on miniserver
void ChatClient::startFallbackServer() {
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, port)){
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "server failed" + QString::number(port) + "\n");
        std::cout<<server->errorString().toStdString()<<std::endl;
    }
    else
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "server running" + "\n");

    connect(server, SIGNAL(newConnection()), this, SLOT(myNewConnection()));
}
//on newconn to miniserver
void ChatClient::myNewConnection() {
    handlingRequest = true;
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(newHostReadyRead()));
    qsrand(socket->localPort());
    QThread::usleep(qrand()%3500);
    QString instructionString = "youconnecto" + ring.last().ip+ "#" + QString::number(ring.last().port);
    socket->write(instructionString.toStdString().c_str());
    clients.append(socket);
    currentClient = socket;
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "told to " +instructionString + "\n");

}

//on miniserver to read client ip
void ChatClient::newHostReadyRead(){
    while(currentClient->bytesAvailable() > 0)
    {
        outgoing = new QTcpSocket();
        QString str = currentClient->readAll();
        QString temp = str.remove(0, 3);
        QString tempIp = temp.left(temp.indexOf("$"));
        int tempport = temp.mid(temp.indexOf("$") + 1, temp.length()).toInt();
        for(int i = 0; i<ring.length(); i++){
            if(ring.at(i).ip == tempIp && ring.at(i).port == tempport){
                currentClient->write("takenewsockaddr");
                ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() +
                                             " " + "take an new port" + "\n");
                return;
            }
        }
        Host temph;
        temph.ip = tempIp;
        temph.port = tempport;
        ring.append(temph);
        QString ipstring;
        for(int i = 0; i<ring.length(); i++){
            ipstring = ipstring + ring.at(i).ip + ":" + QString::number(ring.at(i).port) + "#";
        }
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() +
                                     " " + "complete ring " +ipstring + "\n");
        QHostAddress addr(tempIp);
        outgoing->connectToHost(addr, tempport);
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "head Connected to" +QString::number(tempport) + tempIp + "\n");

    }
}




//old--------------------------------------------------------------------------------------
void ChatClient::createRing() {
    for(int i = 0; i < clients.length(); i++) {

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
    connect(this, SIGNAL(stopTimers()), heartBeatReceiveTimer, SLOT(stop()));
    connect(this, SIGNAL(stopTimers()), heartBeatSendTimer, SLOT(stop()));
    modeCentralized = true;

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
        else if(str == "###youarefallback") {
            this->isFallback = true;
            qsrand(QTime::currentTime().second());
            port = qrand();
            while(port < 1024 || port > 65535)
                port = qrand();
            QString str = "###fallback127.0.0.1#" + QString::number(port);
            writeMessage(str);
        }
        else if(str.startsWith("###fallback")) {
            QString temp = str.remove(0, 11);
            this->Ip = temp.left(temp.indexOf("#"));
            this->port = temp.mid(temp.indexOf("#") + 1, temp.length()).toInt();
//            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "setup" +QString::number(port) + Ip + "\n");

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
    if(modeCentralized){
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + ui->lineEditName->text() + ": " + ui->textEditMsg->toPlainText() + "\n");
    writeMessage(ui->textEditMsg->toPlainText());
    ui->textEditMsg->setText("");
    }
    else{
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + ui->lineEditName->text() + ": " + ui->textEditMsg->toPlainText() + "\n");
        ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
        ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
        QString string = "<originator" + this->windowTitle()+ ">" + ui->textEditMsg->toPlainText();
        outgoing->write(string.toStdString().c_str()
                        );
        ui->textEditMsg->setText("");
    }
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
        server->close();
        incomming->close();
        writeMessage("###close" + ui->lineEditName->text());
        event->accept();
    }
}
