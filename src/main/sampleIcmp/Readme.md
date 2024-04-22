sudo xdp-loader unload enp1s0 --all
clang -O2 -g -Wall -target bpf -c SimpleIcmpEcho.c -o SimpleIcmpEcho.o
sudo xdp-loader load -m skb -s xdp_icmp_echo enp1s0 SimpleIcmpEcho.o
sudo bpftool prog tracelog show xdp_icmp_echo


sudo ip -s -s neigh flush all 


ping-sender
    upgautamvt@client:~/Documents$ arp -a
    _gateway (192.168.1.1) at 2e:97:73:9c:23:eb [ether] on enp1s0
    upgautamvt@client:~/Documents$ ip addr show enp1s0
    2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 52:54:00:93:26:58 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.10/24 brd 192.168.1.255 scope global dynamic noprefixroute enp1s0
    valid_lft 543sec preferred_lft 543sec
    inet6 fe80::bd09:73fe:1e9d:939/64 scope link noprefixroute
    valid_lft forever preferred_lft forever

ping-receiver
    upgautamvt@server:~/IdeaProjects/ebpf_logstash/src/main/sample$ arp -a
    _gateway (192.168.1.1) at 2e:97:73:9c:23:eb [ether] on enp1s0
    upgautamvt@server:~/IdeaProjects/ebpf_logstash/src/main/sample$ ip addr show enp1s0
    2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 52:54:00:eb:c7:1c brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.11/24 brd 192.168.1.255 scope global dynamic noprefixroute enp1s0
    valid_lft 472sec preferred_lft 472sec
    inet6 fe80::5400:4469:e44:70ad/64 scope link noprefixroute
    valid_lft forever preferred_lft forever




