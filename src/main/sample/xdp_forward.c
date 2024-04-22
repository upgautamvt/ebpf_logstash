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
        // Modify destination IP address and port
        ip->daddr = htonl(ip->saddr); // Destination IP address is the source IP address
        tcp->dest = htons(80); // Destination port is set to 80 (HTTP)

        // Print modified destination IP address
        bpf_printk("Modified destination IP: %u.%u.%u.%u\n",
                   (ip->daddr >> 24) & 0xFF, (ip->daddr >> 16) & 0xFF,
                   (ip->daddr >> 8) & 0xFF, ip->daddr & 0xFF);

        // Print modified destination port
        bpf_printk("Modified destination port: %u\n", ntohs(tcp->dest));

        // Redirect the packet back to the original sender
        return bpf_redirect(ctx->ingress_ifindex, 0);
    }

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
