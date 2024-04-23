import matplotlib.pyplot as plt

# Packet counts
packet_counts = [20000, 50000, 10000000]

# Times faster eBPF is than Logstash
times_faster = [5.579, 4.322, 1.444]

# Plotting
plt.figure(figsize=(10, 6))

plt.plot(packet_counts, times_faster, marker='o')

plt.title('eBPF vs Logstash: Times Faster')
plt.xlabel('Packet Counts')
plt.ylabel('Times Faster (eBPF vs Logstash)')
plt.xscale('log')
plt.grid(True)

plt.savefig('howMuchFaster.png')