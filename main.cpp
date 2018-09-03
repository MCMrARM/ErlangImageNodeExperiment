#include <erl_interface.h>
#include "Application.h"

int main() {
    erl_init(nullptr, 0);

    Application app ("imagep");
    app.run();

    return 0;
}