#pragma once

#include <string>
#include <ei.h>

class ErlangCNode {

private:
    static const char* TAG;

    ei_cnode ec;
    int serverSocket = -1;
    int serverSocketPort = -1;
    int publishFd = -1;

public:
    static std::string readSystemCookie();

    ErlangCNode(std::string thisnodename, std::string cookie = readSystemCookie(), short creation = 0);

    ErlangCNode(std::string thishostname, std::string thisalivename, std::string thisnodename,
               Erl_IpAddr thisipaddr, std::string cookie = readSystemCookie(), short creation = 0);

    ~ErlangCNode();

    void createServerSocket(int maxBacklog = 5);

    void closeServerSocket();

    int accept(ErlConnect& connection);

    void publish();

    void unpublish();

};