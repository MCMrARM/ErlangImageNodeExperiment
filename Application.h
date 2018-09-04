#pragma once

#include "ErlangCNode.h"

class ErlangBufferReadHelper;

class Application {

private:
    static const char* TAG;

    ErlangCNode node;

    void handleConnection(int fd, ErlConnect connect);

    void handleMessage(ErlangBufferReadHelper& reader);

    void handleProcessImage(std::string const& inputPath, std::string const& outputPath, int width, int height);

public:
    Application(std::string nodeName) : node(nodeName) {}

    void run();


};