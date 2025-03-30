#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

sshpass -p "$password" ssh "$username@$remote_hostname" bash <<EOF
source ~/miniforge3/etc/profile.d/conda.sh
cd /home/$username/Projects/Titan-Processing

echo "Updating environment..."
conda env update -f environment.yml
EOF
