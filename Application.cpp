#include "Application.h"

#include <stdexcept>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <vips/vips.h>
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
        handleMessage(reader);
    }
    ei_x_free(&buf);
    close(fd);
}

void Application::handleMessage(ErlangBufferReadHelper& reader) {
    reader.decodeVersion();
    int count = reader.decodeTupleHeader();

    char atom[MAXATOMLEN];
    reader.decodeAtom(atom);
    Log::trace(TAG, "Received message: %s [%i args]", atom, count);
    if (strcmp(atom, "process_image") == 0) {
        Log::trace(TAG, "Starting process_image");
        std::string inputPath = reader.decodeBinary();
        std::string outputPath = reader.decodeBinary();
        int width = (int) reader.decodeLong();
        int height = (int) reader.decodeLong();
        handleProcessImage(inputPath, outputPath, width, height);
    }
}

void Application::handleProcessImage(std::string const& inputPath, std::string const& outputPath,
                                     int width, int height) {
    VipsImage* image;
    if (vips_thumbnail(inputPath.c_str(), &image, width,
                       "height", height,
                       "crop", VIPS_INTERESTING_CENTRE, nullptr)) {
        Log::warn(TAG, "Failed to open image: %s", inputPath.c_str());
        return;
    }
    vips_image_remove(image, VIPS_META_ICC_NAME);
    if (vips_webpsave(image, outputPath.c_str(), nullptr) < 0) {
        Log::warn(TAG, "Failed to save image");
    }
    g_object_unref(image);
    Log::info(TAG, "Saved image");
}