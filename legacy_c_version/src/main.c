#include <arpa/inet.h>

#include <netinet/in.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "ipv4_subnet.h"
#include "vector.h"

#define MAX_MASK_V4 32

VECTOR_IMPLEMENTATION(Subnetv4, 10)
VECTOR_IMPLEMENTATION(int, 10)

#define PRINT_ERR(x) (fprintf(stderr, "ERROR: " x "\n"))

void printIntArray(int* vec, size_t size)
{
    printf("[ ");
    for (size_t i = 0; i < size; i++) {
        printf ("%d ", vec [i]);
    }
    printf("]\n");
}
int compare_int(int i, int j)
{
    return j - i;
}

QUICKSORT_IMPLEMENTATION(int, compare_int)

void check_strtol(long int num)
{
    if (num == 0 && errno != 0) {
        PRINT_ERR("Couldn't parse mask!\nPlease Provide mask in range 0 to 32");
        exit(EXIT_FAILURE);
    }
}

void get_next_ip(Subnetv4* subnet, uint8_t host_bits)
{
    // 0000_0000.0000_0000.0000_0000.0000_0000;
    int num_bits = MAX_MASK_V4 - subnet->cidr_mask - host_bits;
    uint32_t subnet_bit_mask = ((1 << num_bits) - 1) << host_bits;
    in_addr_t subnet_bits = (subnet->ip.addr & subnet_bit_mask) >> host_bits;
    subnet_bits += 1;
    subnet_bits <<= host_bits;
    subnet->ip.addr &= ~subnet_bit_mask;
    subnet->ip.addr |= subnet_bits;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        PRINT_ERR("Incorrect Usage");
        PRINT_ERR("Please provide IP and CIDR mask");
        return EXIT_FAILURE;
    }

    const char *ip_addr = argv[1];
    in_addr_t ip_addr_num = 0;
    if (!inet_pton (AF_INET, ip_addr, &ip_addr_num)) {
        PRINT_ERR("Invalid ip provided.");
    }

    long int parse_result = strtol(argv[2], NULL, 10);
    check_strtol(parse_result);

    if (parse_result < 0 || parse_result > MAX_MASK_V4) {
        PRINT_ERR("Mask cannot be bigger than the ip.\nEnter mask in range [0, 32] ");
        return EXIT_FAILURE;
    }

    /* Creation of the main subnet
     */
    uint8_t mask = parse_result;
    Subnetv4 main_subnet = create_subnetv4(ip_addr_num, mask);
    uint32_t main_subnet_size = calculate_size_v4(&main_subnet);

    intVec subnet_sizes = create_intVec();
    Subnetv4Vec subnet_vector = create_Subnetv4Vec();

    uint32_t summed_subnet_size = 0;

    for(int i = 3; i < argc; i++) {
        long int read_value = strtol(argv[i], NULL, 10);
        check_strtol(read_value);
        summed_subnet_size += 1 << get_subnet_power(read_value);
        insert_intVec(&subnet_sizes, read_value);
    }

    if(summed_subnet_size > main_subnet_size) {
        PRINT_ERR("Subnet given cannot fit all hosts.");
        return EXIT_FAILURE;
    }

    quicksort_intVec(&subnet_sizes);

    for(size_t i = 0; i < subnet_sizes.size; i++) {
        uint8_t host_bits = get_subnet_power(subnet_sizes.array[i]);
        Subnetv4 curr_subnet = main_subnet;
        insert_Subnetv4Vec(&subnet_vector, curr_subnet);
        curr_subnet.cidr_mask = MAX_MASK_V4 - host_bits;
        get_next_ip(&main_subnet, host_bits);
    }

    for(size_t i = 0; i < subnet_vector.size; i++) {
        print_ipv4(&subnet_vector.array[i].ip);
    }

    print_ipv4(&main_subnet.ip);

    free(subnet_vector.array);
    free(subnet_sizes.array);
    return 0;
}
