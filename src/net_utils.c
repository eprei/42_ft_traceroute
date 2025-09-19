#include "traceroute.h"

bool convert_address(t_traceroute *ping) {
    const int convert_address_result = inet_pton(AF_INET, ping->destination_host, &ping->sa.sin_addr);

    if (convert_address_result <= 0) {
        if (convert_address_result == 0) {
//            fprintf(stderr, "Not in presentation format");
        }
        else {
            perror("inet_pton");
        }
        return false;
    }

    ft_strncpy(ping->ip, ping->destination_host, 15);
    return true;
}

bool dns_lookup(t_traceroute *ping) {
    struct hostent *host_entity;
    if ((host_entity = gethostbyname(ping->destination_host)) == NULL) {
        // fprintf(stderr, UNKNOWN_HOST_MSG, ping->binary_name, ping->destination_host);
        fprintf(stderr, "ft_traceroute: unknown host\n");
        return false;
    }

    ft_strncpy(ping->ip, inet_ntoa(*(struct in_addr *)host_entity->h_addr), 15);
    ping->sa.sin_family = host_entity->h_addrtype;
    ping->sa.sin_addr = *(struct in_addr *)host_entity->h_addr;

    return true;
}
