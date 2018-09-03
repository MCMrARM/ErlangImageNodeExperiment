#pragma once

#include "ErlangCNode.h"

class Application {

private:
    static const char* TAG;

    ErlangCNode node;

    void handleConnection(int fd, ErlConnect connect);

public:
    Application(std::string nodeName) : node(nodeName) {}

    void run();

};