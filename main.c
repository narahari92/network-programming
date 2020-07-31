#include<stdio.h>
#include "network_math/network_math.h"

int identify_endian();
int ip_maths();

int main() {
    return ip_maths();
}

int identify_endian() {
    int endian = little_endian();
    printf("Is little endian: %d\n", endian);

    return 0;
}

int ip_maths() {
    char* ip_addr = "172.25.12.14";
    char *check_ip_addr = "172.25.12.20";
    char mask = 21;
    char reconverted_ip[17];
    char broadcast_addr[17];
    char network_id[17];
    unsigned int cardinality;
    int is_in_subnet;

    printf("ip address: %s\n", ip_addr);

    unsigned int integral_ip = get_ip_integral_equivalent(ip_addr);
    printf("integral ip: %u\n", integral_ip);

    get_abcd_ip_format(integral_ip, reconverted_ip);
    printf("reconverted ip: %s\n", reconverted_ip);

    get_broadcast_address(ip_addr, mask, broadcast_addr);
    printf("broadcast address: %s\n", broadcast_addr);

    get_network_id(ip_addr, mask, network_id);
    printf("network id: %s\n", network_id);

    cardinality = get_subnet_cardinality(mask);
    printf("subnet cardinality: %u\n", cardinality);

    is_in_subnet = check_ip_subnet_membership(network_id, mask, check_ip_addr);
    printf("check ip in subnet: %d\n", is_in_subnet);

    return 0;
}