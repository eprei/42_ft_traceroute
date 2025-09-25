#include "traceroute.h"

long double get_elapsed_time_ms(const struct timeval *start_time) {
    struct timeval time_end;

    if (gettimeofday(&time_end, NULL) != 0) {
        return -1;
    }

    const double microseconds_in_ms = (double)(time_end.tv_usec - (*start_time).tv_usec) / 1000.0;
    const long double total_elapsed_time_ms = (time_end.tv_sec - start_time->tv_sec) * 1000.0 + microseconds_in_ms;

    return total_elapsed_time_ms;
}

void set_timeout(const t_traceroute *traceroute) {
    struct timeval timeout;

    timeout.tv_sec = DEFAULT_TIMEOUT;
    timeout.tv_usec = 0;

    setsockopt(traceroute->socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
}