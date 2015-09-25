#ifndef CHATROOM_H
#define CHATROOM_H
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<thread>
#include<vector>
#include<string.h>
#include<string>
#include "User.h"
#include<unistd.h>
#include "Message.h"
#include<pthread.h>
using namespace std;
class ChatRoom
{
    public:
        ChatRoom(string);
        ~ChatRoom();
    protected:
    private:
        pthread_mutex_t lock;
        string chatRoomName;
        bool firstUser;
        int maxfd=0;
        void replyHeartBeat(string,char *);
        struct timeval tv;
        void setFds();
        fd_set readfds;
        string fallBackIp;
        int fallBackPort;
        int servSocket;
        int heartBeatSocket;
        vector<User> activeUsers;
        vector<Message> msgs;
        vector<Message> msgsToWrite;
        string subject;
        void notifyFallBackResponsiblity(int);
        void setFallBack(string, int);
        void addUser(string name, string clientIp, int sock, int port);
        void removeUser(string name);
        void handleControlMessage(Message m);
        bool isControlMessage(Message m);
        bool isActive;
        void sendMsgToUser(User,Message);
        void chatReadManagement();
        void sendFallBackInfo(int);
        void setFallBackInfo(string clientIp,int clisocket,int port);
        void chatWriteManagement();
        void heartBeat();
};

#endif // CHATROOM_H
