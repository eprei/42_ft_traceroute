#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <stdbool.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <sys/socket.h>
# include <time.h>
# include <netdb.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <signal.h>
# include <unistd.h>
# include <sys/types.h>
# include <limits.h>
# include <float.h>
# include <math.h>
# include <errno.h>

# define PING_PKT_S 64             // ping packet size
# define PORT_NO 0                 // no port
# define SLEEP_RATE 500000         // ping sleep rate (in microseconds)
# define DEFAULT_TIMEOUT 3         // timeout for receiving packets (in seconds)
# define MAX_HOPS 64
# define RECV_BUFFER_SIZE 128
# define PROBES_PER_HOP 3

# define UNKNOWN_HOST_MSG "%s: %s: Name or service not known        \n"

# define USAGE_ERROR "ft_traceroute: missing host operand           \n\
Try 'ft_traceroute --help' for more information.                      "

# define USAGE_MSG "Usage: ft_traceroute HOST                       \n\
Print the route packets trace to network host.                      \n\
\n\
--help                 give this help list                            "

struct ping_pkt {
    struct icmphdr icmp_header;
    char msg[PING_PKT_S - sizeof(struct icmphdr)];
};

typedef struct s_rtt {
    long double rtt_value;
    struct s_rtt *next;
} t_rtt;

typedef struct s_traceroute {
    char *binary_name;
    char *destination_host;
    char ip[INET_ADDRSTRLEN * sizeof(char)];
    int socket_fd;
    struct sockaddr_in sa;
    t_rtt *rtt_list;
} t_traceroute;

bool parsing(int argc, char *argv[], t_traceroute *ping);
int ft_traceroute(t_traceroute *ping);
void signal_handler(int signal);
bool convert_address(t_traceroute *ping);
void init_struct(t_traceroute *traceroute, char *string);
bool is_root(const t_traceroute *ping);
bool dns_lookup(t_traceroute *ping);
bool create_raw_socket(t_traceroute *ping);
void loop(t_traceroute *traceroute);
unsigned short checksum(void *b, int len);
void print_usage();
void print_usage_error();
void fill_packet(int *msg_count, struct ping_pkt *packet, const unsigned short *id);
void exit_error(char *str);
void set_timeout(const t_traceroute *traceroute);
void print_first_line(t_traceroute* ping);
long double get_elapsed_time_ms(const struct timespec *time_start);
size_t	ft_strlen(const char *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strncpy(char *dest, const char *src, size_t n);
void print_unreachable(bool destination_unreachable, const struct icmphdr* icmp_hdr);
void *ft_memset(void *b, const int c, size_t len);

#endif
