#include <iostream>

#include "erl_interface.h"
#include "ErlangCNode.h"

int main() {
    erl_init(nullptr, 0);

    ErlangCNode node ("imagep@node");
    node.createServerSocket();
    node.publish();

    return 0;
}