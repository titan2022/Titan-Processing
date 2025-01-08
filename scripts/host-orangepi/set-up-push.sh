#!/bin/bash

source credentials.sh

git remote add "$git_remote" "$username@$deployment_target:/home/$username/Projects/Titan-Processing"
ssh "$username@$deployment_target" "cd /home/$username/Projects/Titan-Processing && git branch __titan_deployment_staging main"
