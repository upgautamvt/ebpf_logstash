#!/bin/bash

# Switch to root shell
sudo -i << EOF
    # Switch to the desired directory
    cd /home/upgautamvt/IdeaProjects/ebpf_logstash/src/main/logstash

    # Run logstash with the specified configuration file
    logstash -f /home/upgautamvt/IdeaProjects/ebpf_logstash/src/main/logstash/logstash_script.conf
EOF
