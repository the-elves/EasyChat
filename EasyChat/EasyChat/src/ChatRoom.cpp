#include "ChatRoom.h"
#include<string>
#include<strings.h>
#include<ifaddrs.h>
#include<iostream>
int ChatRoom::iter;
using namespace std;
/**
    Constructor
*/
ChatRoom::ChatRoom(string subject)
{
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    firstUser = true;
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;
    char myIp[12];
    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            if(strcmp(ifa->ifa_name,"eth0") == 0)
                strcpy(myIp, addr);
        }
    }

    freeifaddrs(ifap);
    pthread_mutex_init(&lock,NULL);
    sockaddr_in localhost, client;
    unsigned long clilen;
    this->subject = subject;
    //Scoket initialization
    servSocket = socket(AF_INET,SOCK_STREAM ,0);
    bzero(&localhost,sizeof(sockaddr_in));
    localhost.sin_family = AF_INET;
    inet_aton(myIp, &(localhost.sin_addr));
    localhost.sin_port = htons(1234);
    int b = bind(servSocket,(sockaddr*)&localhost,sizeof(sockaddr_in));
    if(b<0){
        exit(1);
    }

//
//    heartBeatSocket = socket(AF_INET,SOCK_STREAM ,0);
//    bzero(&localhost,sizeof(sockaddr_in));
//    localhost.sin_family = AF_INET;
//    inet_aton("127.0.0.1", &(localhost.sin_addr));
//    localhost.sin_port = htons(4321);
//    int b = bind(heartBeatSocket,(sockaddr*)&localhost,sizeof(sockaddr_in));
//    if(b<0){
//        cout<<"heartBeat failed"<<endl;
//        exit(1);
//    }

//socket Initialization ends

    cout<<"waiting for join"<<endl;
    bzero(&client, sizeof(client));
    listen(servSocket, 5);
    thread a(&ChatRoom::chatReadManagement,this);
    if(this->subject == "main") {
        while(1){
            bzero(&client, sizeof(sockaddr_in));
            listen(servSocket,5);
            int clisocket = accept(servSocket, (sockaddr*)&client, (socklen_t*)&clilen);
            string clientIp = inet_ntoa(client.sin_addr);
            int port = ntohs(client.sin_port);
            char cliName [100];
            int n = read(clisocket, cliName,100);
            cliName[n] = '\0';
            maxfd = maxfd>clisocket?maxfd:clisocket;
            pthread_mutex_lock(&lock);
            addUser(string(cliName), clientIp,clisocket,port);
            cout<<clisocket<<"added"<<endl;
            usleep(100000);
            if(firstUser == true){
                cout<<"FirstUser"<<endl;
                firstUser = false;
                notifyFallBackResponsiblity(clisocket);
            }
            else{
                cout<<"not first user"<<endl;
                sendFallBackInfo(clisocket);
            }
            pthread_mutex_unlock(&lock);
            cout<<cliName<<" joined"<<endl;
        }
    }

}

/**
    This function notifies the fallback server responsiblity to the first node
    that connects to chat server
*/

void ChatRoom::notifyFallBackResponsiblity(int cli){
    string fallBackInfoString = "###";
    fallBackInfoString += "youarefallback\0";
    cout<<"writubg to "<<cli<<endl;
    cout<<"notify Writting"<<endl; cout<<write(cli, fallBackInfoString.c_str(), fallBackInfoString.length())<<endl;
    cout<<"resp notified"<<endl;
}
/** Sets the fallback string,
    It is ip and port of the fallback server
*/
void ChatRoom::setFallBack(string str){
    fallBackString = str;
    cout<<"fallback set"<<endl;
}
/**
    This function send a fall back info to the newly conncected clients
*/
void ChatRoom::sendFallBackInfo(int cli){
    cout<<"send fbinfo Writting"<<endl;
    cout<<write(cli, fallBackString.c_str(), fallBackString.length())<<endl;
}

/**
    This sets the Feature Descriptor macros
*/
void ChatRoom::setFds(){

    vector<User>::iterator i;
    i = activeUsers.begin();
    FD_ZERO(&readfds);
    for(i=activeUsers.begin();i!=activeUsers.end();i++){
        FD_SET(i->getSocket(),&readfds);
    }
}

/**
Destructor
*/
ChatRoom::~ChatRoom(){
    close(servSocket);
}
/**
Adds a new user to the vector of current users.

*/
void ChatRoom::addUser(string name, string clientIp, int sock,int port){
    User tempUser(name, clientIp, sock, port);
    activeUsers.push_back(tempUser);
    vector<User>::iterator i;
    string onlineUsersName;
    for(i = activeUsers.begin();i!=activeUsers.end();i++){
        onlineUsersName += "!@#" + i->getName();
    }
    for(i = activeUsers.begin();i!=activeUsers.end();i++){
        cout<<"adduser Writting"<<endl; write(i->getSocket(), onlineUsersName.c_str(), strlen(onlineUsersName.c_str())+1);
    }
}
/**
removes a user from the vector of current users.

*/
void ChatRoom::removeUser(string s){
    vector<User>::iterator i;
    pthread_mutex_lock(&lock);
    cout<<"Aquiring lock"<<endl;
    i = activeUsers.begin();
    int indexToDelete = 0;
    while(i != activeUsers.end()){
        if(i->getName() == s){
            i=activeUsers.erase(i);
            break;
        }else{
            i++;
            indexToDelete++;
        }
    }
    string onlineUsersName;
    for(i = activeUsers.begin();i!=activeUsers.end();i++){
        onlineUsersName += "!@#" + i->getName();
    }
    for(i = activeUsers.begin();i!=activeUsers.end();i++){
        cout<<"Remove user Writting"<<endl; write(i->getSocket(), onlineUsersName.c_str(), strlen(onlineUsersName.c_str())+1);
    }
    pthread_mutex_unlock(&lock);
}

/**
Handles a control Message According to its type
*/
void ChatRoom::handleControlMessage(Message m){
    cout<<"handle control Message : "<<m.getText()<<endl;
    if(strstr(m.getText(),"###close")!=NULL)
        removeUser(m.getFrom());
    else if(strstr(m.getText(),"###heartbeat")!=NULL)
        replyHeartBeat(m.getFrom(),m.getText());
    else if(strstr(m.getText(),"###fallback")!=NULL){
        setFallBack(m.getText());
        cout<<"fbaddress received"<<m.getText()<<endl;
    }

}
/**
Replies to hearbeat control message
*/
void ChatRoom::replyHeartBeat(string s, char *text){
        vector<User>::iterator i;
        i=activeUsers.begin();
        //cout<<"Aquiring lock for heartbeat"<<endl;
        pthread_mutex_lock(&lock);
        while(i != activeUsers.end()){
            if(i->getName() == s){
                break;
            }
            i++;
        }
        //cout<<"Heartbeat Writting"<<endl;
        write(i->getSocket(), text, strlen(text));
        pthread_mutex_unlock(&lock);
}
/**
Checks if current message is a control Message or not
*/
bool ChatRoom::isControlMessage(Message m){

    cout<<"Checkning cm "<<endl;
    if(strstr(m.getText(),"###") != NULL)
        return true;
    else
    return false;
}
/**
This is a function which runs in an independent thread.
It loops over all the sockes using FDs and checks if there is a new message in any of them.
When it finds new message,
It checks if it is a unicast message, broadcast message, or control message.
Then takes actions accordingly
All the messages are stored in msgs for logging purpose.
*/
void ChatRoom::chatReadManagement(void ){
    vector<User>::iterator i;
    vector<Message> removeList;
    char msg[300] = "";
    bool msgReceived = false;
    while(1){
        pthread_mutex_lock(&lock);
        setFds();
        select(maxfd+1, &readfds, NULL, NULL, &tv);
        for(i = activeUsers.begin(); i != activeUsers.end(); i++){
            //cout<<"checking "<<i->getSocket()<<endl;
            if(FD_ISSET(i->getSocket(), &readfds)){
                int n = read(i->getSocket(), msg, 300);
                if(n>0)
                {
                    msg[n] = '\0';
                    cout<<msg<<endl;
                    msgReceived = true;
                }
            }
            if(msgReceived){
                msgReceived = false;
                Message m(msg, "curtime", "all", i->getName(), Msg);
                strcpy(msg,"");
                if(isControlMessage(m)){
                    cout<<"controlMessage"<<endl;
                    removeList.push_back(m);
                }
                else {
                    cout<<"not a control Message"<<endl;
                    msgs.push_back(m);
                    msgsToWrite.push_back(m);
                    chatWriteManagement();
                }
                //cout<<"in read mmgt"<<endl;
            }

        }
        pthread_mutex_unlock(&lock);
        vector<Message>::iterator mi;
        for(mi = removeList.begin();mi!=removeList.end();mi++){
            handleControlMessage(*mi);
        }
        removeList.clear();
        usleep(100000);

    }
}

/**
Called from read management to forward messages to individual sockets
The messages are are read from a vector<Message> msgsToWrite;
*/
void ChatRoom::chatWriteManagement(){
    //cout<< "in writewrite managemt"<<endl;
    vector<User>::iterator i;
    Message m;
    //int n;
    while(!msgsToWrite.empty()){
        m = msgsToWrite.back();
        string str = m.getText();
        for(i = activeUsers.begin();i!=activeUsers.end();i++){
            //cout<<"Checking "<<i->getName()<<endl;
            if(i->getName() != m.getFrom()) {
                string::size_type found = str.find("$%^");
                if(found != string::npos) {
                    if((str.substr(0,i->getName().size()) == i->getName()) && (str.substr(i->getName().size(), 3) == "$%^")) {
                        string msg = m.getFrom();
                        msg += ": ";
                        msg += m.getText();
                        cout<<"write mgt Writting 1"<<endl; write(i->getSocket(), msg.c_str(), strlen(msg.c_str())+1);
                        //msgsToWrite.pop_back();
                        break;
                    }
                    continue;
                }
                string msg = m.getFrom();
                msg += ": ";
                msg += m.getText();
                cout<<"chat mgt Writting 2"<<endl; write(i->getSocket(), msg.c_str(), strlen(msg.c_str())+1);
            }
        }
        msgsToWrite.pop_back();
    }
}
