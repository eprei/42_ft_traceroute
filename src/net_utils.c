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
    struct addrinfo hints, *result;
    int error_code;
    bool is_ipv4 = false;

    ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    error_code = getaddrinfo(ping->destination_host, NULL, &hints, &result);
    if (error_code != 0)
    {
        fprintf(stderr, "ft_traceroute: unknown host\n");
        freeaddrinfo(result);
        return false;
    }

    while (result && !is_ipv4)
    {
        if (result->ai_family == AF_INET)
        {
            inet_ntop(result->ai_family, &((struct sockaddr_in *)result->ai_addr)->sin_addr, ping->ip, IPV4_ADDR_SIZE);
            ft_memcpy(&ping->sa, result->ai_addr, sizeof(struct sockaddr_in));
            is_ipv4 = true;
        }
        result = result->ai_next;
    }

    freeaddrinfo(result);

    return true;
}
