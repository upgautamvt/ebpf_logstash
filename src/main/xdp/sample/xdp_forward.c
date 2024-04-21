#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <netinet/in.h>

#define SEC(NAME) __attribute__((section(NAME), used))

#define LOCAL_PORT  8080
#define REMOTE_IP   0xC0A8010A      // 192.168.1.10 in network byte order
#define REMOTE_PORT 8080
#define LOCAL_IP    0xC0A8010B      // 192.168.1.11 in network byte order

SEC("xdp_program_forward")
int xdp_program(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    struct iphdr *ip = data + sizeof(struct ethhdr);
    struct tcphdr *tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);




    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
