#include <erl_interface.h>
#include <vips/vips8>
#include "Application.h"

int main(int argc, char **argv) {
    erl_init(nullptr, 0);
    if (VIPS_INIT(argv[0]))
        vips_error_exit(nullptr);

    Application app ("imagep");
    app.run();

    return 0;
}