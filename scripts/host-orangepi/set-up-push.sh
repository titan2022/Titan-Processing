#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

git remote add "$git_remote" "$username@$remote_hostname:/home/$username/Projects/Titan-Processing"
sshpass -p "$password" ssh "$username@$remote_hostname" "cd /home/$username/Projects/Titan-Processing && git branch __titan_deployment_staging main"
