#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

sshpass -p "$password" ssh "$username@$remote_hostname" bash <<EOF
source ~/miniforge3/etc/profile.d/conda.sh
cd /home/$username/Projects/Titan-Processing

echo "Removing old environment..."
conda remove -n Titan-Processing --all

echo "Rebuilding environment..."
conda env create -f environment.yml
EOF
