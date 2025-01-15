#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

sshpass -p "$password" ssh "$username@$remote_hostname" "cd ~/Projects/Titan-Processing && yes \"$password\" | ./scripts/orangepi/autostart.sh"

