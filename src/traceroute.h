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

# define NOT_A_FLAG 0
# define V_FLAG 1
# define TTL_FLAG 2
# define W_FLAG 3
# define w_FLAG 4
# define H_FLAG 5

# define PING_PKT_S 64              // ping packet size
# define PORT_NO 0                  // no port
# define SLEEP_RATE 500000         // ping sleep rate (in microseconds)
# define DEFAULT_TIMEOUT 3             // timeout for receiving packets (in seconds)
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

// ICMP messages
// Destination Unreachable Message
# define NET_UNREACHABLE "Destination Net Unreachable"
# define HOST_UNREACHABLE "Destination Host Unreachable"
# define PROTOCOL_UNREACHABLE "Protocol Unreachable"
# define PORT_UNREACHABLE "Port Unreachable"
# define FRAGMENTATION_NEEDED "Fragmentation needed and DF set"
# define SOURCE_ROUTE_FAILED "Source route failed"

// Time Exceeded Message
# define TTL_EXCEEDED "Time to live exceeded"
# define FRAGMENT_REASSEMBLY_TIME_EXCEEDED "Fragment reassembly time exceeded"

// Parameter Problem Message
# define PARAMETER_PROBLEM_POINTER "Pointer indicates the error"

// Quench Message
# define SOURCE_QUENCH "Source quench"

// Redirect Message
# define REDIRECT_NETWORK "Redirect datagram for the network"
# define REDIRECT_HOST "Redirect datagram for the host"
# define REDIRECT_NETWORK_TOS "Redirect datagram for the type of service and network"
# define REDIRECT_HOST_TOS "Redirect datagram for the type of service and host"

// Timestamp or Timestamp Reply Message
# define TIMESTAMP_REQUEST "Timestamp request"
# define TIMESTAMP_REPLY "Timestamp reply"

// Information Request or Information Reply Message
# define INFO_REQUEST "Information request"
# define INFO_REPLY "Information reply"

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
int is_flag(const char *str);
bool get_flag_value(const int *argc, char **argv, int *i, int *flag_value_store, int flag);
bool get_ttl_value(t_traceroute *ping, char *arg);
void print_usage();
void print_usage_error();
bool verify_usage(const t_traceroute *ping);
void fill_packet(int *msg_count, struct ping_pkt *packet, const unsigned short *id);
void exit_error(char *str);
void process_rtt(t_rtt **rtt_list, long double rtt);
long double get_last_rtt(t_rtt *rtt_list);
long double get_min_rtt(t_rtt *rtt_list);
long double get_max_rtt(t_rtt *rtt_list);
long double get_avg_rtt(t_rtt *rtt_list);
long double get_mdev_rtt(t_rtt *rtt_list);
void free_rtt_list(t_rtt *rtt_node);
void print_stats(const t_traceroute *ping, int msg_count, int msg_received_count, int errors);
void set_timeout(const t_traceroute *traceroute);
void print_stats(const t_traceroute *ping, const int msg_count, const int msg_received_count, const int errors);
void print_non_echo_icmp(const t_traceroute *ping, const struct iphdr *base_ip_hdr, const struct icmphdr *icmp_hdr, char* sender_ip, const unsigned long received_packet_size);
void print_custom_message(const struct icmphdr* icmp_hdr);
void print_packet_content(const struct iphdr* base_ip_hdr);
void print_first_line(t_traceroute* ping);
void print_option_need_argument(const int flag_value);
long double get_elapsed_time_ms(const struct timespec *time_start);
void print_timeout(const bool *W_flag, const int *msg_count);

#endif
