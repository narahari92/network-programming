#include<string.h>
#include<math.h>
#include<stdio.h>
#include "network_math.h"

unsigned int create_mask(char mask);

void get_broadcast_address(char *ip_addr, char mask, char* broadcast_addr) {
    unsigned int integral_ip, broadcast_integral;
    unsigned int bitmask = create_mask(mask);

    bitmask = ~bitmask;
    integral_ip = get_ip_integral_equivalent(ip_addr);
    broadcast_integral = integral_ip | bitmask;

    get_abcd_ip_format(broadcast_integral, broadcast_addr);
}

void get_network_id(char *ip_addr, char mask, char* network_id) {
    unsigned int integral_ip, network_id_integral;
    unsigned int bitmask = create_mask(mask);

    integral_ip = get_ip_integral_equivalent(ip_addr);
    network_id_integral = integral_ip & bitmask;

    get_abcd_ip_format(network_id_integral, network_id);
}

unsigned int get_subnet_cardinality(char mask) {
    return pow(2, MAX_IP_BITS - mask) - 2;
}

int check_ip_subnet_membership(char *network_id, char mask, char* check_ip) {
    char check_ip_network_id[strlen(network_id)];
    get_network_id(check_ip, mask, check_ip_network_id);

    if(strcmp(network_id, check_ip_network_id) == 0) {
        return 0;
    }

    return -1;
}

unsigned int get_ip_integral_equivalent(char *ip_addr) {
    unsigned int integral_ip = 0;
    unsigned int mask = 0x01;
    unsigned int ip_parts[4];
    int ip_part_index = 0;
    unsigned int ip_part = 0;
    int power_index = 0;

    for(int i = strlen(ip_addr) - 1 ; i >= 0 ; i--) {
        if(ip_addr[i] != '.') {
            ip_part += (ip_addr[i] - '0') * pow(10, power_index++);
            continue;
        }

        ip_parts[ip_part_index++] = ip_part;
        ip_part = 0;
        power_index = 0;
    }

    ip_parts[ip_part_index] = ip_part;

    for(int i = 0 ; i < 4 ; i++) {
        for(int j = 0 ; j < 8 ; j++) {
            if(ip_parts[i] & (mask << j)) {
                integral_ip += pow(2, i * 8 + j);
            }
        }
    }

    return integral_ip;
}

void get_abcd_ip_format(unsigned int ip_addr, char *output_buffer) {
    unsigned int part1 = ip_addr & 0xff;
    unsigned int part2 = (ip_addr >> 8) & 0xff;
    unsigned int part3 = (ip_addr >> 16) & 0xff;
    unsigned int part4 = (ip_addr >> 24) & 0xff;
    sprintf(output_buffer, "%d.%d.%d.%d", part4, part3, part2, part1);
}

unsigned int create_mask(char mask) {
    unsigned int bitmask = 0xFFFFFFFF;

    for(int i = 0 ; i < MAX_IP_BITS - mask ; i++) {
        UNSET_BIT(bitmask, i);
    }

    return bitmask;
}