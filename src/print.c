#include "traceroute.h"

void print_first_line(t_traceroute* ping){
       printf("traceroute to %s (%s), %d hops max\n", ping->destination_host, ping->ip, MAX_HOPS);
}

void print_usage() {
    fprintf(stderr, "%s\n", USAGE_MSG);
}

void print_usage_error(){
    fprintf(stderr, "%s\n", USAGE_ERROR);
}

void print_unreachable(const bool destination_unreachable, const struct icmphdr* icmp_hdr)
{
    char c[3];

    if (destination_unreachable){
        switch (icmp_hdr->code){
        case ICMP_HOST_UNREACH:
            ft_strncpy(c, "!H", strlen("!H") + 1);
            break;
        case ICMP_NET_UNREACH:
            ft_strncpy(c, "!N", ft_strlen("!N") + 1);
            break;
        case ICMP_PROT_UNREACH:
            ft_strncpy(c, "!P", ft_strlen("!P") + 1);
            break;
        default:
            break;
        }
        printf(" %s", c);
    }
}
