import matplotlib.pyplot as plt

# Packet counts
packet_counts = [20000, 50000, 10000000]

# Throughput for eBPF and Logstash (packets per second)
ebpf_throughput = [259740.26, 442477.88, 3117765.59]
logstash_throughput = [46511.63, 102459.01, 2157726.29]

# Plotting
plt.figure(figsize=(10, 6))

plt.plot(packet_counts, ebpf_throughput, marker='o', label='eBPF')
plt.plot(packet_counts, logstash_throughput, marker='o', label='Logstash')

plt.title('Throughput Comparison: eBPF vs Logstash')
plt.xlabel('Packet Counts')
plt.ylabel('Throughput (Packets per Second)')
plt.xscale('log')
plt.grid(True)
plt.legend()

plt.savefig('throughputGraph.png')