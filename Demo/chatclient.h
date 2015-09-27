#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QPushButton>
#include "unicast.h"
#include <QTimer>
#include<host.h>
#include <QThread>
#include <QMap>

namespace Ui {
class ChatClient;
}

class ChatClient : public QMainWindow {
    Q_OBJECT

public:

    static std::vector<QPushButton*> onlineUsers;
    static std::vector<Unicast*> unichat;
    explicit ChatClient(QString subject, QWidget *parent = 0);
    ~ChatClient();
    bool heartBeatReceived;
    void start(const QString& msg);
    void startConnection(QString address, quint16 port);
    void closeEvent(QCloseEvent *event);
    void startFallbackServer();
    void connectToFallbackServer();
    void createRing();
    void initializeIncoming();

    //QTimer *heartBeatTimer;

public slots:
    void writeMessage(const QString& msg);
    void readyRead();
    void unicast();
    void heartBeatSend();
    void heartBeatReceive();
    void myNewConnection();
    void fallbackReadyRead();
    void incommingNewConnection();
    void newHostReadyRead();
    void msgFromPrevious();
private slots:
    void on_pushButtonSend_clicked();

    void on_textEditMsg_textChanged();

    void on_pushButtonConnect_clicked();

    void on_lineEditName_textChanged();

signals:
    void sendMsgToUnicast(QString msg);
    void stopTimers();

private:
    bool handlingRequest;
    bool modeCentralized;
    QTcpSocket *currentClient;
    QString subject;
    Ui::ChatClient *ui;
    QTcpSocket client;
    QTcpSocket *peerToPeerIncomming;
    QTcpSocket peer;
    QList<Host> ring;
    QList<QTcpSocket*> clients;
    QTcpServer *server;
    QTcpServer *incomming;
    QTcpSocket *outgoing;
    bool isFallback;
    QString Ip;
    int port, inPort, outPort;
};

#endif // CHATCLIENT_H
