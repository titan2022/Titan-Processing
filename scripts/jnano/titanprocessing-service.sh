#!/bin/bash

cd ~/Projects/Titan-Processing
# source ~/.venvs/FRC-2024-Vision/bin/activate
echo "titanprocessing.service: Starting..." | systemd-cat -p info
./bin/one_cam_detect | systemd-cat -p info
