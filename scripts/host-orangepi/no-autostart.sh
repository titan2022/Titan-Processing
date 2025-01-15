#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

ssh "$username@$remote_hostname" "cd ~/Projects/Titan-Processing && yes \"$password\" | ./scripts/orangepi/no-autostart.sh"

