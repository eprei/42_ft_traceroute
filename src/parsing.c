#include "traceroute.h"

bool parsing(const int argc, char **argv, t_traceroute *ping) {
    if (argc < 2) {
        print_usage_error();
        return false;
    }

    if (ft_strncmp("--help", argv[1], strlen("--help")) == 0) {
        print_usage();
        return false;
    }

    ping->destination_host = argv[1];
    return true;
}
