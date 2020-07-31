#define SET_BIT(n, pos) (n =  n | (1 << pos))
#define UNSET_BIT(n, pos) (n = n & ((1 << pos) ^ 0xFFFFFFFF))

#define MAX_IP_BITS 32

int little_endian();

void get_broadcast_address(char *ip_addr, char mask, char* broadcast_addr);
unsigned int get_ip_integral_equivalent(char *ip_addr);
void get_abcd_ip_format(unsigned int ip_addr, char *output_buffer);
void get_network_id(char *ip_addr, char mask, char* network_id);
unsigned int get_subnet_cardinality(char mask);
int check_ip_subnet_membership(char *network_id, char mask, char* check_ip);