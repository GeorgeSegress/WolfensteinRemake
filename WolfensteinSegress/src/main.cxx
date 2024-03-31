#include "controller.hxx"
#include <iostream>
#include <string>

int
main(int argc, char *argv[])
{
    bool testing = argc > 2 && strcmp(argv[2], "testing") == 0;
    if(argv[1] != nullptr) {
        Controller(argv[1], testing).run();
    }

    return 0;
}
