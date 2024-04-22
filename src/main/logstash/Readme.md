sudo xdp-loader unload enp1s0 --all
clang -O2 -g -Wall -target bpf -c SimpleXdpLogstash.c -o SimpleXdpLogstash.o
sudo xdp-loader load -m skb -s simple_xdp_logstash enp1s0 SimpleXdpLogstash.o
sudo bpftool prog tracelog show simple_xdp_logstash

















sudo dnf install libbpf-devel libxdp-devel xdp-tools bpftool
clang -O2 -g -Wall -target bpf -c xdp_forward.c -o xdp_forward.o
objdump -S xdp_forward.o
llvm-objdump -S xdp_forward.o
llvm-objdump -h xdp_forward.o
sudo xdp-loader load -m skb -s xdp_program_forward enp1s0 xdp_forward.o
    (or using iproute2: sudo ip link set enp1s0 xdpgeneric obj xdp_program_forward.o sec xdp_program_forward)
    (or need to unload first using: sudo xdp-loader unload enp1s0 --all )
sudo xdp-loader status 
    (or using iproute2: sudo ip link show enp1s0)

upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/xdp$ sudo xdp-loader unload enp1s0 --all (or using iproute2: sudo ip link set enp1s0 xdpgeneric off)
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/xdp$ sudo xdp-loader load -m skb -s xdp_program_forward enp1s0 xdp_forward.o
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/xdp$ sudo ip link show enp1s0
2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 xdpgeneric qdisc fq_codel state UP mode DEFAULT group default qlen 1000
link/ether 52:54:00:eb:c7:1c brd ff:ff:ff:ff:ff:ff
prog/xdp id 171 name xdp_dispatcher tag 90f686eb86991928 jited
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/xdp$ sudo xdp-loader unload enp1s0 --all
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/xdp$  sudo ip link show enp1s0
2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
link/ether 52:54:00:eb:c7:1c brd ff:ff:ff:ff:ff:ff
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/xdp$  sudo xdp-loader load -m skb -s xdp_program_forward enp1s0 xdp_forward.o
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/xdp$ sudo xdp-loader status 
CURRENT XDP PROGRAM STATUS:

Interface        Prio  Program name      Mode     ID   Tag               Chain actions
--------------------------------------------------------------------------------------
lo                     <No XDP program loaded!>
enp1s0                 xdp_dispatcher    skb      251  90f686eb86991928
=>              50     xdp_program               260  837cfd771ebf0682  XDP_PASS
enp7s0                 <No XDP program loaded!>
docker0                <No XDP program loaded!>

//to display all bpf programs currently loaded in the system
sudo bpftool prog show


sudo bpftool prog tracelog show xdp_program_forward


ping sender (from this machine I do ping 192.168.1.10 ip of xdp host)
upgautamvt@fedora:~/Documents$ ip addr show enp1s0
2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
link/ether 52:54:00:93:26:58 brd ff:ff:ff:ff:ff:ff
inet 192.168.1.10/24 brd 192.168.1.255 scope global dynamic noprefixroute enp1s0
valid_lft 346sec preferred_lft 346sec
inet6 fe80::bd09:73fe:1e9d:939/64 scope link noprefixroute
valid_lft forever preferred_lft forever
upgautamvt@fedora:~/Documents$ arp -a
_gateway (192.168.1.1) at 2e:97:73:9c:23:eb [ether] on enp1s0

xdp (ping receiver) (this machine receives ping sent by 192.168.1.11 and then xdp program modifies destination ip to be 192.168.11 and destination mac to be 2e:97:73:9c:23:eb)
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/sample$ ip addr show enp1s0
2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
link/ether 52:54:00:eb:c7:1c brd ff:ff:ff:ff:ff:ff
inet 192.168.1.11/24 brd 192.168.1.255 scope global dynamic noprefixroute enp1s0
valid_lft 429sec preferred_lft 429sec
inet6 fe80::5400:4469:e44:70ad/64 scope link noprefixroute
valid_lft forever preferred_lft forever
upgautamvt@fedora:~/IdeaProjects/ebpf_logstash/src/main/sample$ arp -a
_gateway (192.168.1.1) at 2e:97:73:9c:23:eb [ether] on enp1s0
