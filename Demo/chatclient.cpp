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
/**
    This is a constructor

*/
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
    Ip = "172.17.13.59";
}
/**
 * @brief ChatClient::heartBeatSend
 * This function sends a hearbeat signal to the server.
 *
 */

void ChatClient::heartBeatSend(){
    heartBeatReceived = false;
    const QString s = "###heartbeat";
    writeMessage(s);
    std::cout<<"Heartbeat sent"<<std::endl;
}

/**
 * @brief ChatClient::heartBeatReceive
 * Checks if the sent hearbeat is received. If not switches to peer to pper mode
 */

void ChatClient::heartBeatReceive(){
    if(heartBeatReceived){
        std::cout<<"heartbeat received"<<std::endl;
    }
    else{
        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "Switching " + "\n");
        ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
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
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "fallback Strarted " + "\n");
            ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
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
            //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "seed set to = " +QString::number(seed) + "\n");
            QThread::sleep(5 + qrand()%3);
            initializeIncoming();
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " attempted to connect to peers"  + "\n");
            this->connectToFallbackServer();
            ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
        }
    }
}


//common

/**
 * @brief ChatClient::initializeIncoming
 * It initialize the incoming socket on every node.
 */
void ChatClient::initializeIncoming(){
   incomming = new QTcpServer(this);
   qsrand(QTime::currentTime().msec());
   inPort = 1024 + qrand()%(65535 - 1024);
   do{
       inPort = 1024 + qrand()%(65535 - 1024);
   }
   while(!incomming->listen(QHostAddress::Any, inPort));


//   if(!incomming->listen(QHostAddress::Any, inPort)){
//       //std::cout << "incomming server not started";
//       ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " incoming server failed " +QString::number(inPort) + "\n");
//   }
//   else{
//       ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "server ruuning on My local Ip" +QString::number(inPort) + "\n");
//      // std::cout << "incomming server server running";
//   }
   connect(incomming, SIGNAL(newConnection()), this, SLOT(incommingNewConnection()));
}


//new connection on incomming port for client

/**
 * @brief ChatClient::incommingNewConnection
 * Handles connectin requests for the incoming socket on every node.
 */
void ChatClient::incommingNewConnection(){
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "someone connected on incommming "+ "\n");
    peerToPeerIncomming = incomming->nextPendingConnection();
    connect(peerToPeerIncomming,SIGNAL(readyRead()),this, SLOT(msgFromPrevious()));

}

//onclient message received from previous node in chain
/**
 * @brief ChatClient::msgFromPrevious
 * Handles a message received in the ring topology created.
 * It forwards the message if it is not the originator of the message.
 */
void ChatClient::msgFromPrevious(){
    while(peerToPeerIncomming->bytesAvailable() > 0){
        QString str = peerToPeerIncomming->readAll();
        QString str1(str);
        //<originator
        QString temp = str.mid(0, str.indexOf(">"));
        temp.remove(0,11);
        str.remove(0,str.indexOf(">")+1);
        if(!(this->windowTitle() == temp)){
            ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + temp + ":" + str + "\n");
            ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
            ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
            outgoing->write(str1.toStdString().c_str());
        }
    }
}

//onclinet
/**
 * @brief ChatClient::connectToFallbackServer
 * This method connects a node to a fallback server.
 * It then sends its own incoming socket address to fall back server to connect
 */
void ChatClient::connectToFallbackServer() {
    QHostAddress addr(this->Ip);
    peer.connectToHost(addr, port);
    if(peer.waitForConnected(5000))
        //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "fallback connected" + "\n");
        std::cout<<"fallbackConnected";
    else
        std::cout<<"fallback not connected";
        //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "fallback not connected" + QString::number(port) + this->Ip + "\n");
    connect(&peer, SIGNAL(readyRead()), this, SLOT(fallbackReadyRead()));
    QString str = "$$$" + Ip + "$" + QString::number(inPort);
    peer.write(str.toStdString().c_str());
    //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "sent my info "+str + "\n");
}

//on client read form fallback server
/**
 * @brief ChatClient::fallbackReadyRead
 * Receives the replies from fallback server
 * It then fetches the node it should connect to
 */
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
            //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "attempt|" +QString::number(rport)+":" + rIp + "|\n");
            outgoing->connectToHost(addr, rport);
            if(outgoing->waitForConnected(5000))
                std::cout<<"attemp success";
                //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "attempt succss" +QString::number(rport)+":" + rIp + "\n");
            else
                std::cout<<"faild";
                //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "attempt failed" +QString::number(rport)+":" + rIp + "\n");

        }
        else if(str.startsWith("takenewsockaddr")){
            initializeIncoming();
            QString str = "$$$" + Ip + "$" + QString::number(inPort);
            peer.write(str.toStdString().c_str());
            //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "sent my info "+str + "\n");
        }
    }
}
//on miniserver
/**
 * @brief ChatClient::startFallbackServer
 * Starts fall back server. After heart beat is failed
 */
void ChatClient::startFallbackServer() {
    server = new QTcpServer(this);
    if(!server->listen(QHostAddress::Any, port)){
        //ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "server failed" + QString::number(port) + "\n");
        std::cout<<server->errorString().toStdString()<<std::endl;
    }
//    else
//        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "server running" + "\n");

    connect(server, SIGNAL(newConnection()), this, SLOT(myNewConnection()));
}
//on newconn to miniserver
/**
 * @brief ChatClient::myNewConnection
 * Handles new connection to the fallback server.
 * Send the client the should connect to according
 */
void ChatClient::myNewConnection() {
    handlingRequest = true;
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(newHostReadyRead()));
    qsrand(socket->localPort());
    QThread::usleep(qrand()%5000);
    QString instructionString = "youconnecto" + ring.last().ip+ "#" + QString::number(ring.last().port);
    socket->write(instructionString.toStdString().c_str());
    clients.append(socket);
    currentClient = socket;
//    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + "told to " +instructionString + "\n");

}

//on miniserver to read client ip
/**
 * @brief ChatClient::newHostReadyRead
 * Callback for a message from new host connected. It reads the hosts socket address
 * If the socket address is conflicting asks the host to pick another
 * Then it adds the hosts to ring topology
 * Instructs host to connect to last member of ring
 * And the mini server(fallback server) connects to the new host.
 */
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
//                ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() +
//                                             " " + "take an new port" + "\n");
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
//        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() +
//                                     " " + "complete ring " +ipstring + "\n");
        QHostAddress addr(tempIp);
        outgoing->connectToHost(addr, tempport);
//        ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + "head Connected to" +QString::number(tempport) + tempIp + "\n");

    }
}




//old--------------------------------------------------------------------------------------
/**
 * @brief ChatClient::~ChatClient
 * Destructor
 */
ChatClient::~ChatClient() {
    client.close();
    delete ui;
}
/**
 * @brief ChatClient::start Initialization function called by qt
 * @param msg
 */
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
/**
 * @brief ChatClient::startConnection starts connection with the server
 * @param address
 * @param port
 */
void ChatClient::startConnection(QString address, quint16 port) {
    //std::cout << "ChatClient startconnection ";
    QHostAddress addr(address);
    client.connectToHost(addr, port);
}
/**
 * @brief ChatClient::writeMessage writes message to the server
 * @param msg
 */
void ChatClient::writeMessage(const QString& msg) {
    //std::cout << "ChatClient transfer ";
    const QString* msgQStr = &msg;
    std::string messageStr = msgQStr->toStdString();
    const char* messageCh = messageStr.c_str();
    client.write(messageCh, 50);
}

/**
 * @brief ChatClient::readyRead Callback for receiving message from server
 * It can be a control message or it can be a Text message
 * It takes action accordingly
 */
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
/**
 * @brief ChatClient::unicast
 */
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
/**
 * @brief ChatClient::on_pushButtonSend_clicked
 * GUI callback for send button click event
 */
void ChatClient::on_pushButtonSend_clicked() {
    if(modeCentralized){
    ui->textBrowserChat->setText(ui->textBrowserChat->toPlainText() + QTime::currentTime().toString() + " " + ui->lineEditName->text() + ": " + ui->textEditMsg->toPlainText() + "\n");
    writeMessage(ui->textEditMsg->toPlainText());
//    ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
//    ui->textBrowserChat->verticalScrollBar()->setValue(ui->textBrowserChat->verticalScrollBar()->maximum());
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
/**
 * @brief ChatClient::on_textEditMsg_textChanged
 * GUI callback for type in edit message text box event
 */
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
    startConnection(Ip, 1234);
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
