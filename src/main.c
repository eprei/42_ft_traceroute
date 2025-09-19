#include "traceroute.h"

int main(const int argc, char **argv) {

    signal(SIGINT, signal_handler);

    t_traceroute traceroute = {0};
    init_struct(&traceroute, argv[0]);

    if (!is_root(&traceroute) ||
        !parsing(argc, argv, &traceroute) ||
        !create_raw_socket(&traceroute)) {
            return 1;
    }

    return ft_traceroute(&traceroute);
}

void init_struct(t_traceroute *traceroute, char *string) {
    traceroute->binary_name = string;
    traceroute->sa.sin_family = AF_INET;
    traceroute->sa.sin_port = htons(PORT_NO);
}

bool is_root(const t_traceroute *ping) {
    if (getuid() != 0) {
        fprintf(stderr, "%s must be run as root\n", ping->binary_name);
        return false;
    }
    return true;
}

bool create_raw_socket(t_traceroute *ping){
    ping->socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (ping->socket_fd < 0) {
        //        fprintf(stderr, "\nSocket file descriptor not received!\n");
        return false;
    } else {
        //        printf("\nSocket file descriptor %d received\n", ping->socket_fd);
        return true;
    }
}
