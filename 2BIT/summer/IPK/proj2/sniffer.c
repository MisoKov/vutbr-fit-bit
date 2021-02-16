/**
 *	@file	sniffer.c
 *	@author	Michal Koval, xkoval17 FIT
 *	@date	29.04.2020
 *	@brief	Implementation of sniffer part of project
 *	@note	Compiler: gcc 7.5
 */

#include "sniffer.h"
#include "errors.h"

int print_interfaces(){
    pcap_if_t *alldevsp;
    char errbuf[PCAP_ERRBUF_SIZE];
    // finding the intefaces
    if (pcap_findalldevs(&alldevsp, errbuf) == PCAP_ERROR){
        fprintf(stderr,"Error occured during finding of interfaces: %s\n", errbuf);
        return ERR_INTERFACE;
    }
    // printing of found devices
    pcap_if_t *iterator = alldevsp;
    while (iterator != NULL){
        printf("%s\n", iterator->name);
        iterator = iterator->next;
    }
    // freeing the list of found devices
    pcap_freealldevs(alldevsp);
    return OK;
}


pcap_t* open_device(char *device, bpf_u_int32 *net, bpf_u_int32 *mask){
// function inspired by:
// https://www.tcpdump.org/pcap.html
// https://www.devdungeon.com/content/using-libpcap-c?fbclid=IwAR2NP-LDruBjX-Hwa1To67uX6_4KmmxAxWzbT_6TIpOkG93p2bR3o4A9-XU
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    handle = pcap_open_live(device, BUFSIZ, 1, 0, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Error couldn't open device %s\n", errbuf);
        return NULL;
    }
    // checking link layer
    if (pcap_datalink(handle) != DLT_EN10MB) {
        fprintf(stderr, "Error device %s doesn't provide Ethernet headers - not supported\n", device);
        pcap_close(handle);
        return NULL;
    }
    // getting interface netmask and ip
    if (pcap_lookupnet(device, net, mask, errbuf) == -1) {
        fprintf(stderr, "Error couldn't get netmask for device %s: %s\n", device, errbuf);
        *net = 0;
        *mask = 0;
    }
    return handle;
}

void set_filter_format(char *filter, int mode, char *port){
    memset(filter, '\0', SIZE_FILTER);
    if (port != NULL){
        strcat(filter, "port ");
        strcat(filter, port);
        strcat(filter, " and ");
    }
    if (mode == MODE_BOTH){
        if (port != NULL) {
            strcat(filter, "(udp or tcp)");
        } else {
            strcat(filter, "(udp or tcp)");
        }
    } else if (mode == MODE_TCP){
        strcat(filter, "tcp");
    } else if (mode == MODE_UDP){
        strcat(filter, "udp");
    }
}

int setup_filter(char *filter, pcap_t *handle, int mode, char *port, int net){
// function inspired by:
// https://www.tcpdump.org/pcap.html
// https://www.devdungeon.com/content/using-libpcap-c?fbclid=IwAR2NP-LDruBjX-Hwa1To67uX6_4KmmxAxWzbT_6TIpOkG93p2bR3o4A9-XU
    struct bpf_program compiled_filter;
    set_filter_format(filter, mode, port);
    if (pcap_compile(handle, &compiled_filter, filter, 0, net) == -1) {
        fprintf(stderr, "Error couldn't parse filter %s: %s\n", filter, pcap_geterr(handle));
        return ERR_FILTER;
    }
    if (pcap_setfilter(handle, &compiled_filter) == -1) {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handle));
        return ERR_FILTER;
    }
    pcap_freecode(&compiled_filter);
    return OK;
}

void print_packet_body(const u_char *payload, int len, int starting_count){
    const u_char *temp_ptr = payload;
    const u_char *line_start_ptr;
    int printed_together = 0;
    int hex_count = starting_count;
    for ( ; printed_together + 16 < len; printed_together += 16){
        printf("0x%04x: ", hex_count);
        line_start_ptr = temp_ptr;
        for (int i = 0; i < 16; i++){
            printf("%02x ", *temp_ptr);
            temp_ptr++;
        }
        temp_ptr = line_start_ptr;
        for (int i = 0; i < 16; i++){
            if (isprint(*temp_ptr)){
                printf("%c", *temp_ptr);
            } else {
                printf(".");
            }
            temp_ptr++;
        }
        hex_count += 16;
        printf("\n");
    }
    if (printed_together < len){
        printf("0x%04x: ", hex_count);
        line_start_ptr = temp_ptr;
        int i = 0;
        while((printed_together + i) < len){
            printf("%02x ", *temp_ptr);
            i++;
            temp_ptr++;
        }
        temp_ptr = line_start_ptr;
        i = 0;
        while((printed_together + i) < len){
            if (isprint(*temp_ptr)){
                printf("%c", *temp_ptr);
            } else {
                printf(".");
            }
            i++;
            temp_ptr++;
        }
        printf("\n");
    }
}

int get_hostname(char *ret_addr, int ret_len, u_int version, struct in_addr addr, u_short port){
    char string_port[PORT_LEN];
    snprintf(string_port, 6, "%d", port);
    char service_buf[NI_MAXSERV];
    struct addrinfo filter = {0};
    struct addrinfo * result;
    // IPV4
    if (version == IPV4){
        if (inet_ntop(AF_INET, &addr, ret_addr, ret_len) == NULL){
            fprintf(stderr, "Error couldn't convert source ip address\n");
            return ERR_ADDR_CON;
        }
        filter.ai_family = AF_INET;
    // IPV6
    } else if (version == IPV6){
        if (inet_ntop(AF_INET6, &addr, ret_addr, ret_len) == NULL){
            fprintf(stderr, "Error couldn't convert source ip address\n");
            return ERR_ADDR_CON;
        }
        filter.ai_family = AF_INET6;
    } else {
        fprintf(stderr, "Error couldn't convert source ip address - version check\n");
        return ERR_ADDR_CON;
    }
    //return OK;   //   --- !!!  Uncomment to disable resolving of IP address  !!! ---
    // address info
    if (getaddrinfo(ret_addr,string_port,&filter,&result) != OK){
        freeaddrinfo(result);
        return OK;
    }
    if (getnameinfo(result->ai_addr, result->ai_addrlen, ret_addr, ret_len, service_buf, NI_MAXSERV, NI_NAMEREQD) != OK){
        freeaddrinfo(result);
        return OK;
    }
    freeaddrinfo(result);
    return OK;
}

int print_packet_info(const struct pcap_pkthdr packet_header, struct ip_header ip, u_short source_p, u_short dest_p){
    printf("/-----------------------------------------------------------------------/\n");
    // time
    struct tm *converted_time = localtime(&(packet_header.ts.tv_sec));
    printf("%02d:%02d:%02d.%ld ", converted_time->tm_hour, converted_time->tm_min, converted_time->tm_sec, packet_header.ts.tv_usec);
    // addresses and ports
    char addr_string[NI_MAXHOST];
    u_int version = (u_int)IP_HEADER_VER(&ip);
    // source
    memset(addr_string, '\0', NI_MAXHOST);
    if (get_hostname(addr_string, NI_MAXHOST, version,ip.source_addr, source_p) != OK){
        return ERR_ADDR_CON;
    }
    printf("%s : %d ", addr_string, source_p);
    // host
    memset(addr_string, '\0', NI_MAXHOST);
    if (get_hostname(addr_string, NI_MAXHOST, version, ip.dest_addr, dest_p) != OK){
        return ERR_ADDR_CON;
    }
    printf("> %s : %d\n\n", addr_string, dest_p);
    return OK;
}

void loop_callback(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *packet_body){
// function inspired by:
// https://www.tcpdump.org/pcap.html
// https://www.devdungeon.com/content/using-libpcap-c?fbclid=IwAR2NP-LDruBjX-Hwa1To67uX6_4KmmxAxWzbT_6TIpOkG93p2bR3o4A9-XU
    // checking number of caught packets
    static int success_packets = 0;
    if (success_packets >= required_packets_num){
        error_code = OK;
        pcap_breakloop(loop_handle);
        return;
    }

    // checking whether we caught IPv4 packet
    struct ether_header *eth_header;
    eth_header = (struct ether_header *) packet_body;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        fprintf(stderr, "Caught not IP packet, continuing the work\n");
        return;
    }
    // dividing the contents of packet
    const struct ip_header *ip = (struct ip_header*)(packet_body + SIZE_ETHERNET);
    u_int ip_size = IP_HEADER_LEN(ip);
    if (ip_size < MIN_IP_HEADER_SIZE){
        fprintf(stderr, "Error invalid IP header length: %u bytes\n", ip_size);
        error_code = ERR_IP_H;
        pcap_breakloop(loop_handle);
        return;
    }
    // checking protocol type
    u_short source_port;
    u_short dest_port;
    u_int protocol_size;
    if (ip->protocol == IPPROTO_TCP) {
        const struct tcp_header *tcp = (struct tcp_header*)(packet_body + SIZE_ETHERNET + ip_size);
        protocol_size = TH_OFF(tcp) * 4;
        if (protocol_size < MIN_TCP_HEADER_SIZE){
            fprintf(stderr, "Error invalid TCP header length: %u bytes\n", protocol_size);
            error_code = ERR_TCP_H;
            pcap_breakloop(loop_handle);
            return;
        }
        source_port =  ntohs(tcp->source_port);
        dest_port = ntohs(tcp->dest_port);
    } else if (ip->protocol == IPPROTO_UDP){
        const struct udp_header *udp = (struct udp_header*)(packet_body + SIZE_ETHERNET + ip_size);
        protocol_size = SIZE_UDP;
        source_port = ntohs(udp->source_port);
        dest_port = ntohs(udp->dest_port);
    } else {
        fprintf(stderr, "Error unsupported protocol type caught\n");
        error_code = ERR_PROTOCOL;
        pcap_breakloop(loop_handle);
        return;
    }
    // printing the packet
    const u_char *payload = (u_char *)(packet_body + SIZE_ETHERNET + ip_size + protocol_size);
    int payload_len = packet_header->caplen - ( SIZE_ETHERNET + ip_size + protocol_size);
    const u_char *start_of_packet = (u_char *) packet_body;
    int headers_len = SIZE_ETHERNET + ip_size + protocol_size;
    if (print_packet_info(*packet_header, *ip, source_port, dest_port) != OK){
        error_code =  ERR_ADDR_CON;
    }
    print_packet_body(start_of_packet, headers_len, 0); // headers
    printf("\n");
    print_packet_body(payload, payload_len, headers_len); // payload/body

    success_packets++;
}

int start_sniffing(char *interface, int mode, int n_packets, char *port){
// general process of sniffing using pcap library heavily inspired by these two sources:
// https://www.tcpdump.org/pcap.html
// https://www.devdungeon.com/content/using-libpcap-c?fbclid=IwAR2NP-LDruBjX-Hwa1To67uX6_4KmmxAxWzbT_6TIpOkG93p2bR3o4A9-XU
    // declarations of variables
    pcap_t *handle;
    char filter[SIZE_FILTER];
    bpf_u_int32 mask;
    bpf_u_int32 net;

    // opening and setting the interface
    if ((handle = open_device(interface, &net, &mask)) == NULL){
        return ERR_INTERFACE;
    }
    // filter setup
    if (setup_filter(filter, handle, mode, port, net) != OK){
        return ERR_FILTER;
    }
    // sniffing
    loop_handle = handle;
    required_packets_num = n_packets;
    pcap_loop(handle, 0, loop_callback, NULL);
    pcap_close(handle);

    return error_code;
}

