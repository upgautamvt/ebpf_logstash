#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/tcp.h>

#define SEC(NAME) __attribute__((section(NAME)))

#define NEW_SRC_MAC "\x52\x54\x00\xeb\xc7\x1c" // Mac of the server
#define NEW_DEST_MAC "\x52\x54\x00\x93\x26\x58" // Mac of the client
#define REVERSED_HOST_IP_ADDRESS 0x0b01a8c0 // 10.1.168.192 as c0 = 192 a8 = 168, 01 = 1, and 0b = 11
#define HOST_IP_ADDRESS 0xc0a8010b // 192.168.1.10

SEC("simple_xdp_logstash")
int xdp_swap(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    if (eth + 1 > data_end)
        return XDP_DROP;

    struct iphdr *ip = data + sizeof(struct ethhdr);
    if (ip + 1 > data_end)
        return XDP_DROP;


    if (ip->protocol == IPPROTO_TCP) {
        struct tcphdr *tcp = (struct tcphdr *)(ip + 1);

        if (tcp + 1 > data_end)
            return XDP_DROP;

        // Print values before modifications
        bpf_printk("Source IP: %d.%d.%d.%d, Destination IP: %d.%d.%d.%d\n",
                     (ip->saddr >> 0) & 0xFF, (ip->saddr >> 8) & 0xFF, (ip->saddr >> 16) & 0xFF, (ip->saddr >> 24) & 0xFF,
                     (ip->daddr >> 0) & 0xFF, (ip->daddr >> 8) & 0xFF, (ip->daddr >> 16) & 0xFF, (ip->daddr >> 24) & 0xFF);
        bpf_printk("Source MAC: %x:%x:%x:%x:%x:%x, Destination MAC: %x:%x:%x:%x:%x:%x\n",
                      eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5],
                      eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);


        ip->daddr = ip->saddr; //assign source address to destination address
        ip->saddr = REVERSED_HOST_IP_ADDRESS; //hardcoded host address. This will be new source address
        memcpy(eth->h_source, NEW_SRC_MAC, ETH_ALEN);
        memcpy(eth->h_dest, NEW_DEST_MAC, ETH_ALEN);

        // Print values after modifications
        bpf_printk(" After modification, Source IP: %d.%d.%d.%d, Destination IP: %d.%d.%d.%d\n",
                     (ip->saddr >> 0) & 0xFF, (ip->saddr >> 8) & 0xFF, (ip->saddr >> 16) & 0xFF, (ip->saddr >> 24) & 0xFF,
                     (ip->daddr >> 0) & 0xFF, (ip->daddr >> 8) & 0xFF, (ip->daddr >> 16) & 0xFF, (ip->daddr >> 24) & 0xFF);
        bpf_printk("After modification, Source MAC: %x:%x:%x:%x:%x:%x, Destination MAC: %x:%x:%x:%x:%x:%x\n",
                      eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5],
                      eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);



        return XDP_TX;
    }

    return XDP_PASS; //pass to the host network stack
}

char _license[] SEC("license") = "GPL";
