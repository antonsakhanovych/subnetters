#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define print_err(x) (fprintf(stderr, "ERROR: " x "\n"))

typedef union {
  in_addr_t full;
  uint8_t octets[4];
} Ipv4;

/* Normalize function for convenient bit operation on IPs. 192.168.1.78
 *  would be stored in array like this: [192,168,1,78] which seems
 *  good but in memory it is layed out like this 78 1 168 192 and we
 *  cannot manipulate bits easily. This function swaps elements to
 *  this representation: 192 168 1 78 -> [78,1,168,192].
 */
void normalize_ip(Ipv4 *ip) {
  for (size_t i = 0; i < 2; i++) {
    uint8_t temp = ip->octets[i];
    ip->octets[i] = ip->octets[3 - i];
    ip->octets[3 - i] = temp;
  }
}

void printIP(const Ipv4 *ip) {
  printf("----------------------\n");
  printf("IP Address: %u.%u.%u.%u\n", ip->octets[3], ip->octets[2],
         ip->octets[1], ip->octets[0]);
  printf("----------------------\n");
}

uint32_t generate_cidr_mask(int cidr_mask) {
  return UINT32_MAX - ((1 << (32 - cidr_mask)) - 1);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    print_err("Incorrect Usage");
    print_err("Please provide IP and CIDR mask");
    return EXIT_FAILURE;
  }

  const char *ip_addr = argv[1];
  Ipv4 ip = {0};
  if (!inet_pton(AF_INET, ip_addr, &ip.full)) {
    print_err("Invalid ip provided.");
  }
  normalize_ip(&ip);

  int subnet_mask = atoi(argv[2]);
  uint32_t bit_network_mask = generate_cidr_mask(subnet_mask);
  uint32_t bit_broadcast_mask = ~bit_network_mask;
  printIP(&ip);
  ip.full &= bit_network_mask;
  printIP(&ip);
  ip.full |= bit_broadcast_mask;
  printIP(&ip);

  return 0;
}
