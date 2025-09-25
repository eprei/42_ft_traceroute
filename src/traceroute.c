#include "traceroute.h"
int ft_traceroute(t_traceroute *ping) {
    if (convert_address(ping) || dns_lookup(ping)){
        loop(ping);
        return 0;
    }
    return 1;
}

void clear_sock_buffer(const int socket_fd)
{
    char dummy[1024];
    while (recvfrom(socket_fd, dummy, sizeof(dummy), MSG_DONTWAIT, NULL, NULL) > 0){}
}

void loop(t_traceroute *traceroute) {
    int ttl = 1;
    int msg_count = 0, addr_len;
    char recv_buffer[RECV_BUFFER_SIZE];
    struct ping_pkt packet;
    struct sockaddr_in recv_addr;
    bool destination_reached = false;
    bool destination_unreachable = false;
    const bool is_localhost = traceroute->sa.sin_addr.s_addr == htonl(INADDR_LOOPBACK);

    set_timeout(traceroute);
    print_first_line(traceroute);

    while (ttl <= MAX_HOPS && !destination_reached && !destination_unreachable) {

        if (setsockopt(traceroute->socket_fd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)) != 0) {
            fprintf(stderr, "Setting socket options to TTL failed!\n");
            return;
        }

        printf("%3d", ttl);
        int i = 0;
        while (i < PROBES_PER_HOP){
            struct timeval start_time;
            unsigned short id = (unsigned short) (getpid() + ttl + i);
            bool packet_sent = true;

            clear_sock_buffer(traceroute->socket_fd);
            fill_packet(&msg_count, &packet, &id);
            gettimeofday(&start_time, NULL);

            if (sendto(traceroute->socket_fd, &packet, sizeof(packet), 0, (struct sockaddr*)&traceroute->sa, sizeof(traceroute->sa)) <= 0) {
                // fprintf(stderr, "Packet Sending Failed!\n");
                packet_sent = false;
            }

            // Receive packet
            ft_memset(recv_buffer, 0, sizeof(recv_buffer));
            addr_len = (int) sizeof(recv_addr);
            ft_memset(&recv_addr, 0, sizeof(recv_addr));

            const size_t bytes_received = recvfrom(traceroute->socket_fd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr*)&recv_addr, (socklen_t *) &addr_len);
            long double rtt = get_elapsed_time_ms(&start_time);
            rtt = rtt != -1 ? rtt : 42.42;

            if (bytes_received == (size_t) -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    printf("  *");
                    // fflush(stdout);
                    i++;
                } else {
                    // fprintf(stderr, "Packet receive failed!\n");
                }
            } else {
                if (packet_sent) {
                    char *sender_ip = inet_ntoa(recv_addr.sin_addr);
                    const struct iphdr *ip_hdr = (struct iphdr *)recv_buffer;
                    const struct icmphdr *icmp_hdr = (struct icmphdr *)(recv_buffer + ip_hdr->ihl * 4); // offset by ip header length

                    if (is_localhost) {
                        if (i == 0) {
                            printf("  %s", sender_ip);
                        }
                        printf("  %.3Lfms", rtt);
                        destination_reached = true;
                      i++;
                    } else {
                        bool to_print = false;

                        if (icmp_hdr->type == ICMP_TIME_EXCEEDED ){
                            const struct iphdr *original_ip = (struct iphdr *)((char *)icmp_hdr + 8);
                            const struct icmphdr *original_icmp = (struct icmphdr *)((char *)original_ip + original_ip->ihl * 4);
                            if (original_icmp->un.echo.id == id) {
                                to_print = true;
                            }
                        } else if (icmp_hdr->type == ICMP_ECHOREPLY){
                            if (icmp_hdr->un.echo.id == id) {
                                to_print = true;
                                destination_reached = true;
                            }
                        } else if (icmp_hdr->type == ICMP_UNREACH) {
                            to_print = true;
                            destination_unreachable = true;
                        }

                        if (to_print){
                            if (i == 0) {
                                printf("  %s", sender_ip);
                            }
                            printf("  %.3Lfms", rtt);
                            print_unreachable(destination_unreachable, icmp_hdr);
                            i++;
                        }
                    }
                }
            }
        }
        printf("\n");
        ttl++;
    }
}
