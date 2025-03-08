#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

sshpass -p "$password" ssh "$username@$remote_hostname" "mkdir -p /home/$username/Projects/Titan-Processing/ && cd /home/$username/Projects/Titan-Processing/ && git init; git branch -m main; git config user.name titan2022-multiuser-system; git config user.email titan2022.multiuser.system@gmail.com; git commit -m 'Initial commit' --allow-empty"
