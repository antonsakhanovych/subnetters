#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "ipv4_subnet.h"

/* Normalize function for convenient bit operation on IPs. 192.168.1.78
 *  would be stored in array like this: [192,168,1,78] which seems
 *  good but in memory it is layed out like this 78 1 168 192 and we
 *  cannot manipulate bits easily. This function swaps elements to
 *  this representation: 192 168 1 78 -> [78,1,168,192].
 */
static in_addr_t _normalize_ip(in_addr_t ip)
{
    static uint8_t bit8 = 0xFF;
    for (size_t i = 0; i < 2; i++) {
        uint8_t temp = (ip >> (i * 8)) & bit8;
        uint8_t target = (ip >> (3 - i) * 8) & bit8;
        ip &= ~(bit8 << (i * 8));
        ip &= ~(bit8 << (3 - i) * 8);
        ip |= (temp << (3 - i) * 8);
        ip |= (target << (i * 8));
    }
    return ip;
}

in_addr_t generate_cidr_mask_v4(uint8_t cidr_mask)
{
    return UINT32_MAX - ((1 << (32 - cidr_mask)) - 1);
}

Subnetv4 create_subnetv4 (in_addr_t addr, uint8_t cidr_mask)
{
    Subnetv4 subnet = {0};
    subnet.ip.addr = _normalize_ip(addr);
    subnet.cidr_mask = cidr_mask;
    return subnet;
}

uint32_t calculate_size_v4 (const Subnetv4* subnet)
{
    in_addr_t mask = generate_cidr_mask_v4(subnet->cidr_mask);
    in_addr_t network_addr = subnet->ip.addr & mask;
    in_addr_t broadcast_addr = subnet->ip.addr | ~mask;
    return broadcast_addr - network_addr + 1;
}

uint8_t get_subnet_power(int num_hosts)
{
    return ceil(log2(num_hosts + 2));
}

void print_ipv4(const Ipv4* ip)
{
    printf("----------------------\n");
    printf("IP Address: ");
    printf ("%u.%u.%u.%u", ip->octets[0], ip->octets[1], ip->octets[2], ip->octets[3]);
    printf ("\n");
    printf("----------------------\n");
}
