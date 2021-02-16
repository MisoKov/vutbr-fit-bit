/**
 *	@file	sniffer.h
 *  @author	Michal Koval, xkoval17 FIT
 *	@date	29.04.2020
 *	@brief	Header file of Packet sniffer
 *	@note	Compiler: gcc 7.5
 */

#ifndef SNIFFER_H
#define SNIFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <sys/types.h>
#include <pcap-bpf.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

//      DEFINES
// Modes
#define MODE_BOTH 0
#define MODE_UDP 1
#define MODE_TCP 2
// Magic numbers
#define SIZE_ETHERNET 14
#define SIZE_FILTER 28
#define MIN_TCP_HEADER_SIZE 20
#define MIN_IP_HEADER_SIZE 20
#define PORT_LEN 6
#define SIZE_UDP 8;
#define IPV4 4
#define IPV6 6
// Macro functions
#define IP_HEADER_LEN(ip)   ((((ip)->version_len) & 0x0f)*4)
#define IP_HEADER_VER(ip)   ((((ip)->version_len) & 0xf0) >> 4)
#define TH_OFF(tcp) (((tcp)->offsetx2 & 0xf0) >> 4)

//      GLOBAL VARIABLES
// used only for passing arguments to and from callback function of pcap_loop
int required_packets_num;
int error_code;
pcap_t *loop_handle;

// -----------------------------
// source:
// https://www.tcpdump.org/pcap.html
// slightly changed, but i dont consider this to be my code
//      STRUCTURES
struct ip_header {
    u_char version_len;
    u_char service_type;
    u_short len;
    u_short id;
    u_short offset;
    u_char ttl;
    u_char protocol;
    u_short checksum;
    struct in_addr source_addr;
    struct in_addr dest_addr;
};

struct tcp_header {
    u_short source_port;
    u_short dest_port;
    u_int seq_num;
    u_int ack_num;
    u_char offsetx2;
    u_char flags;
    u_short window;
    u_short checksum;
    u_short urgent_ptr;
};
// -------------------------------------------
struct udp_header{
    u_short source_port;
    u_short dest_port;
    u_short len;
    u_short checksum;
};
//      PROTOTYPES
/**
 * Finds and prints available interfaces
 * If 0 interfaces are found it still counts as success
 * @return OK on success, ERR_INTERFACE in case of error
 */
int print_interfaces();

/**
 * Opens device for sniffing
 * @param device string name of device
 * @param net pointer to where network number will be stored
 * @param mask pointer to where netmask number will be stored
 * @return pointer to opened device on success, NULL in case of error
 */
pcap_t* open_device(char *device, bpf_u_int32 *net, bpf_u_int32 *mask);

/**
 * Creates fillter expression according to mode and port
 * @param filter result filter string will be stored here
 * @param mode mode of scanning (MODE_BOTH, MODE_TCP, MODE_UDP)
 * @param port string port number
 */
void set_filter_format(char *filter, int mode, char *port);

/**
 * Compiles filter expression and sets the device filter
 * @param filter string containing filter expression
 * @param handle pointer to opened device for sniffing
 * @param mode mode of scanning (MODE_BOTH, MODE_TCP, MODE_UDP)
 * @param port port string port number
 * @param net netmask
 * @return OK on succes, ERR_FILTER in case of error
 */
int setup_filter(char *filter, pcap_t *handle, int mode, char *port, int net);

/**
 * Prints packet data in readable format
 * @param payload pointer to the start of packet body
 * @param len leght of packet body in bytes
 * @param starting_count number of already printed bytes
 */
void print_packet_body(const u_char *payload, int len, int starting_count);

/**
 * Finds hostname from IP address
 * If address can not be resolved ret_addr is filled with addr
 * @param ret_addr return pointer for hostname
 * @param ret_len length of ret_addr buffer
 * @param version version of IP
 * @param addr address which is to be resolved
 * @param port port of address
 * @return OK on success, ERR_ADDR_CON in case of error when addr cannot be translated from network byte code
 */
int get_hostname(char *ret_addr, int ret_len, u_int version, struct in_addr addr, u_short port);

/**
 * Prints time when packet was caught, source ip and port, destination ip and port
 * @param packet_header structure holding the packet header data
 * @param ip structure holding the ip header data
 * @param source_p source port
 * @param dest_p destination port
 * @return OK on success, ERR_ADDR_CON in case of error
 */
int print_packet_info(const struct pcap_pkthdr packet_header, struct ip_header ip, u_short source_p, u_short dest_p);

/**
 * The callback function of pcap_loop which sniffs the packets
 * @param args optional arguments
 * @param packet_header pointer to packet header
 * @param packet_body pointer to packet body
 */
void loop_callback(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body);

/**
 * Main function of sniffer module, initializes and start the sniffing
 * @param interface string name of interfase used for sniffing
 * @param mode mode of scanning (MODE_BOTH, MODE_TCP, MODE_UDP)
 * @param n_packets number of packets to sniff
 * @param port number for filltering
 * @return OK on success, error code from errors.h in case of particular error
 */
int start_sniffing(char *interface, int mode, int n_packets, char *port);

#endif //SNIFFER_H
