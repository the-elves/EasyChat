#include "ChatRoom.h"
#include<string>
#include<iostream>
using namespace std;
ChatRoom::ChatRoom(string subject)
{
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    firstUser = true;
    pthread_mutex_init(&lock,NULL);
    sockaddr_in localhost, client;
    unsigned long clilen;
    this->subject = subject;
    //Scoket initialization
    servSocket = socket(AF_INET,SOCK_STREAM ,0);
    bzero(&localhost,sizeof(sockaddr_in));
    localhost.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(localhost.sin_addr));
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
                setFallBack(clientIp, port);
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



void ChatRoom::notifyFallBackResponsiblity(int cli){
    string fallBackInfoString = "###";
    fallBackInfoString += "youarefallback\0";
    cout<<"writubg to "<<cli<<endl;
    cout<<"notify Writting"<<endl; cout<<write(cli, fallBackInfoString.c_str(), fallBackInfoString.length())<<endl;
    cout<<"resp notified"<<endl;
}

void ChatRoom::setFallBack(string ip, int port){
    this->fallBackIp = ip;
    this->fallBackPort = port;
    cout<<"fallback set"<<endl;
}

void ChatRoom::sendFallBackInfo(int cli){
    string fallBackInfoString = "###";
    fallBackInfoString += "fallback" + fallBackIp + "#" +to_string(fallBackPort)+"\0";
    cout<<"writubg to "<<cli<<endl;
    cout<<"sending fallabck stirng :"<<fallBackInfoString<<endl;
    cout<<"send fbinfo Writting"<<endl; cout<<write(cli, fallBackInfoString.c_str(), fallBackInfoString.length())<<endl;

}
void ChatRoom::setFds(){
    vector<User>::iterator i;
    i = activeUsers.begin();
    FD_ZERO(&readfds);
    for(i=activeUsers.begin();i!=activeUsers.end();i++){
        FD_SET(i->getSocket(),&readfds);
    }
}
ChatRoom::~ChatRoom(){
    close(servSocket);
}
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


void ChatRoom::handleControlMessage(Message m){
    if(strstr(m.getText(),"###close")!=NULL)
        removeUser(m.getFrom());
    else if(strstr(m.getText(),"###heartBeat"))
        replyHeartBeat(m.getFrom(),m.getText());
}

void ChatRoom::replyHeartBeat(string s, char *text){
    vector<User>::iterator i;
    pthread_mutex_lock(&lock);
    cout<<"Aquiring lock for heartbeat"<<endl;
    while(i != activeUsers.end()){
        if(i->getName() == s){
            break;
        }
        i++;
    }
    for(i = activeUsers.begin();i!=activeUsers.end();i++){
        cout<<"Heartbeat Writting"<<endl; write(i->getSocket(), text, strlen(text));
    }
    pthread_mutex_unlock(&lock);
}

bool ChatRoom::isControlMessage(Message m){

    cout<<"Checkning cm "<<endl;
    if(strstr(m.getText(),"###") != NULL)
        return true;
    else
    return false;
}

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
