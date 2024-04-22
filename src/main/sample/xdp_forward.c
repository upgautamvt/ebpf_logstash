#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <netinet/in.h>

#define SEC(NAME) __attribute__((section(NAME), used))

SEC("xdp_program_forward")
int xdp_program(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    struct iphdr *ip = data + sizeof(struct ethhdr);
    struct tcphdr *tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);

    // Ensure the packet is large enough to contain Ethernet, IP, and TCP headers
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct tcphdr) > data_end)
        return XDP_PASS;

    // Check if it's an IPv4 packet and TCP packet
    if (eth->h_proto == htons(ETH_P_IP) && ip->protocol == IPPROTO_TCP) {
        // Modify destination IP and port
        ip->daddr = htonl(192 << 24 | 168 << 16 | 1 << 8 | 10);
        tcp->dest = htons(9090);

        // Print modified destination IP address
        __u32 dest_ip = ip->daddr;
        bpf_printk("Modified destination IP address: %u.%u.%u.%u\n",
                   dest_ip & 0xff, (dest_ip >> 8) & 0xff,
                   (dest_ip >> 16) & 0xff, (dest_ip >> 24) & 0xff);

        // Print modified destination port
        bpf_printk("Modified destination port: %u\n", ntohs(tcp->dest));

        return XDP_TX;
    }

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
