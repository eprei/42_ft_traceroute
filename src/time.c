#include "traceroute.h"

long double get_elapsed_time_ms(const struct timespec *time_start) {
    struct timespec time_end;

    clock_gettime(CLOCK_MONOTONIC, &time_end);
    const double nanoseconds_elapsed = ((double)(time_end.tv_nsec - (*time_start).tv_nsec)) / 1000000.0;
    const long double total_elapsed_time_ms = (time_end.tv_sec - time_start->tv_sec) * 1000.0 + nanoseconds_elapsed;
    return total_elapsed_time_ms;
}

void set_timeout(const t_traceroute *traceroute) {
    struct timeval timeout;

    timeout.tv_sec = DEFAULT_TIMEOUT;
    timeout.tv_usec = 0;

    setsockopt(traceroute->socket_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
}