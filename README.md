# ft_traceroute

A custom implementation of the traceroute utility program, developed as part of the 42 school curriculum.

## Description

ft_traceroute tracks the route that data packets follow from the local machine to a destination host on an IP network. It sends ICMP packets with incrementally increasing TTL values to discover each hop along the path to the destination.

## Features

- IPv4 address and hostname support
- DNS resolution for hostnames
- FQDN display without DNS resolution in hop output
- Round-trip time measurement for each hop
- Error handling for unreachable destinations
- Support for `--help` option

## Requirements

- Root privileges (uses raw sockets)
- Linux environment
- GCC compiler

## Usage

```bash
sudo ./ft_traceroute <destination>
sudo ./ft_traceroute --help
```

### Examples

```bash
sudo ./ft_traceroute google.com
sudo ./ft_traceroute 8.8.8.8
sudo ./ft_traceroute localhost
```

## Compilation

```bash
make
```

## Implementation Details

- Uses raw ICMP sockets for packet transmission
- Implements RFC 1071 checksum calculation
- Maximum 64 hops with 3 probes per hop
- 3-second timeout for packet reception
- Custom libft functions for string/memory operations

## 42 Project

This project is part of the 42 school UNIX network programming curriculum, focusing on:
- Network protocols (ICMP, IP)
- Raw socket programming
- System calls and network utilities
- Time measurement and packet analysis
