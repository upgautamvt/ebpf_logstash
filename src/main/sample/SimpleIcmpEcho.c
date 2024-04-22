#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <netinet/in.h>

#define SEC(NAME) __attribute__((section(NAME), used))

SEC("xdp_icmp_echo")
int xdp_prog(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    struct iphdr *ip = data + sizeof(struct ethhdr);

    if (ip + 1 > data_end)
        return XDP_DROP; //beyond packet data boundary, should pass to Network Stack

    if (eth->h_proto == htons(ETH_P_IP) && ip->protocol == IPPROTO_ICMP) {
        // Trace the ICMP ping
        bpf_printk("ICMP ping detected\n");
         // Swap source and destination IP addresses
        /* __u32 temp_ip = ip->saddr;
         ip->saddr = ip->daddr;
         ip->daddr = temp_ip;
        return XDP_TX;*/

         /*__be32 temp_ip = ip->saddr;
         ip->saddr = ip->daddr;
         ip->daddr = temp_ip;*/
         return XDP_PASS; //todo: instead passing to network stack, I want to handle myself
    }

    return XDP_DROP;
}

char _license[] SEC("license") = "GPL";
