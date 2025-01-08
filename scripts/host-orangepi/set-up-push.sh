#!/bin/bash

source credentials.sh

git remote add "$git_remote" "$username@$remote_hostname:/home/$username/Projects/Titan-Processing"
ssh "$username@$remote_hostname" "cd /home/$username/Projects/Titan-Processing && git branch __titan_deployment_staging main"
