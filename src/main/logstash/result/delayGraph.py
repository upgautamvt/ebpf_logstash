import matplotlib.pyplot as plt

# Packet counts
packet_counts = [20000, 50000, 10000000]

# Average delay per packet for eBPF and Logstash (seconds)
ebpf_delay = [0.00000385, 0.00000226, 0.0000003206]
logstash_delay = [0.0000215, 0.00000976, 0.0000004628]

# Plotting
plt.figure(figsize=(10, 6))

plt.plot(packet_counts, ebpf_delay, marker='o', label='eBPF')
plt.plot(packet_counts, logstash_delay, marker='o', label='Logstash')

plt.title('Average Delay Comparison: eBPF vs Logstash')
plt.xlabel('Packet Counts')
plt.ylabel('Average Delay per Packet (seconds)')
plt.xscale('log')
plt.grid(True)
plt.legend()

plt.savefig('delayGraph.png')
