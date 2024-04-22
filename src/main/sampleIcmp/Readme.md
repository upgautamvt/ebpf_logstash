sudo xdp-loader unload enp1s0 --all
clang -O2 -g -Wall -target bpf -c SimpleIcmpEcho.c -o SimpleIcmpEcho.o
sudo xdp-loader load -m skb -s xdp_icmp_echo enp1s0 SimpleIcmpEcho.o
sudo bpftool prog tracelog show xdp_icmp_echo