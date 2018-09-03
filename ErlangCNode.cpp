#include "ErlangCNode.h"

#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include "Log.h"

const char* ErlangCNode::TAG = "ErlangCNode";

ErlangCNode::ErlangCNode(std::string thisnodename, std::string cookie, short creation) {
    if (ei_connect_init(&ec, thisnodename.c_str(), cookie.c_str(), creation) < 0)
        throw std::runtime_error("Failed to create a new ei_connect node");
}

ErlangCNode::ErlangCNode(std::string thishostname, std::string thisalivename, std::string thisnodename,
                         Erl_IpAddr thisipaddr, std::string cookie, short creation) {
    if (ei_connect_xinit(&ec, thishostname.c_str(), thisalivename.c_str(), thisnodename.c_str(), thisipaddr,
                         cookie.c_str(), creation) < 0)
        throw std::runtime_error("Failed to create a new ei_connect node");
}

ErlangCNode::~ErlangCNode() {
    unpublish();
    closeServerSocket();
}

void ErlangCNode::createServerSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = 0; // Any
    if (bind(serverSocket, (sockaddr*) &addr, sizeof(addr)) < 0)
        throw std::runtime_error("Failed to bind to the specified port");
    if (listen(serverSocket, 5) < 0)
        throw std::runtime_error("Failed to start listening");
    socklen_t addr_len = sizeof(addr);
    getsockname(serverSocket, (sockaddr*) &addr, &addr_len);
    if (addr.sin_family != AF_INET)
        throw std::runtime_error("getsockname() returned a different family that AF_INET");
    serverSocketPort = addr.sin_port;
    Log::info(TAG, "Server created on port: %i", serverSocketPort);
}

void ErlangCNode::closeServerSocket() {
    if (publishFd != -1)
        Log::warn(TAG, "You should unpublish() the server before calling closeServerSocket()");
    if (serverSocket >= 0) {
        close(serverSocket);
        Log::info(TAG, "Server on port %i has been closed", serverSocketPort);
        serverSocket = -1;
        serverSocketPort = -1;
    }
}

void ErlangCNode::publish() {
    if (serverSocketPort == -1)
        createServerSocket();
    publishFd = ei_publish(&ec, serverSocketPort);
    if (publishFd < 0)
        throw std::runtime_error("Failed to publish the node");
}

void ErlangCNode::unpublish() {
    if (publishFd >= 0) {
        close(publishFd);
        publishFd = -1;
    }
}

std::string ErlangCNode::readSystemCookie() {
    const char* home = getenv("HOME");
    std::ifstream file (std::string(home) + "/.erlang.cookie");
    if (!file)
        return std::string();
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}