#!/bin/bash

source credentials.sh

ssh "$username@$remote_hostname" "cd ~/Projects/Titan-Processing && yes \"$password\" | ./scripts/orangepi/autostart.sh"

