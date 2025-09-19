#include "traceroute.h"

void signal_handler(const int signal) {
    if (signal == SIGINT) {
        printf("\n");
        exit(1);
    }
}
