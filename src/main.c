#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ipv4_subnet.h"
#include "vector.h"

VECTOR_IMPLEMENTATION(Subnetv4, 10)
VECTOR_IMPLEMENTATION(int, 10)

#define print_err(x) (fprintf(stderr, "ERROR: " x "\n"))

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
    return i - j;
}

QUICKSORT_IMPLEMENTATION(int, compare_int)

int main(int argc, char **argv)
{
    if (argc < 3) {
        print_err("Incorrect Usage");
        print_err("Please provide IP and CIDR mask");
        return EXIT_FAILURE;
    }

    const char *ip_addr = argv[1];
    in_addr_t ip_addr_num = 0;
    if (!inet_pton (AF_INET, ip_addr, &ip_addr_num)) {
        print_err("Invalid ip provided.");
    }

    uint8_t mask = atoi (argv [2]);

    Subnetv4 main_subnet = create_subnetv4(ip_addr_num, mask);

    intVec subnet_sizes = create_intVec();

    for(int i = 3; i < argc; i++) {
        insert_intVec(&subnet_sizes, atoi(argv[i]));
    }

    quicksort_intVec(&subnet_sizes);

    print_ipv4(&main_subnet.ip);

    free(subnet_sizes.array);
    return 0;
}
