#include "Application.h"

#include <stdexcept>
#include <thread>
#include <unistd.h>
#include <cstring>
#include "Log.h"
#include "ErlangBufferReadHelper.h"

const char* Application::TAG = "Application";

void Application::run() {
    node.createServerSocket();
    node.publish();
    while (true) {
        ErlConnect connection;
        int fd = node.accept(connection);
        if (fd < 0) {
            Log::warn(TAG, "Accept failed: %i %s", fd, strerror(erl_errno));
            continue;
        }

        std::thread thread ([this, fd, connection]() {
            handleConnection(fd, connection);
        });
        thread.detach();
    }
}

void Application::handleConnection(int fd, ErlConnect connect) {
    Log::trace(TAG, "Handling connection from node: %s", connect.nodename);
    ei_x_buff buf;
    if (ei_x_new(&buf) < 0)
        throw std::runtime_error("Failed to create connection buffer");
    erlang_msg msg;
    while (true) {
        int r = ei_xreceive_msg(fd, &msg, &buf);
        if (r < 0 && erl_errno != EAGAIN) {
            Log::warn(TAG, "(%s) ei_xreceive_msg failed: %i", connect.nodename, r);
            break;
        }
        if (r == ERL_TICK || msg.msgtype != ERL_REG_SEND)
            continue;
        ErlangBufferReadHelper reader (buf.buff);
        reader.decodeVersion();
        Log::trace(TAG, "Test: %c", reader.getType());

    }
    ei_x_free(&buf);
    close(fd);
}