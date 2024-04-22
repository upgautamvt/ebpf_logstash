#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <netinet/in.h>

#define SEC(NAME) __attribute__((section(NAME)))

SEC("xdp_icmp_echo")
int xdp_prog(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    if (eth + 1 > data_end)
        return XDP_DROP;

    struct iphdr *ip = data + sizeof(struct ethhdr);
    if (ip + 1 > data_end)
        return XDP_DROP;

    if (ip->protocol == IPPROTO_ICMP) {
        struct icmphdr *icmp = (struct icmphdr *)(ip + 1);
        if (icmp + 1 > data_end)
            return XDP_DROP;

        bpf_printk("Forwarding ICMP packet\n");
        return XDP_TX;
    }

    return XDP_DROP;
}

char _license[] SEC("license") = "GPL";
