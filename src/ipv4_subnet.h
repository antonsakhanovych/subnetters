#ifndef IPV4_SUBNET_HEADER
#define IPV4_SUBNET_HEADER

#include <netinet/in.h>
#include <stdint.h>

typedef union {
    in_addr_t addr;
    uint8_t octets[4];
} Ipv4;

typedef struct {
    Ipv4 ip;
    in_addr_t mask;
} Subnetv4;

Subnetv4 create_subnetv4(in_addr_t addr, uint8_t cidr_mask);

void print_ipv4(const Ipv4* ip);

in_addr_t generate_cidr_mask_v4(uint8_t cidr_mask);

uint32_t calculate_size_v4(const Subnetv4* subnet);

uint8_t get_subnet_power(int num_hosts);

#endif
