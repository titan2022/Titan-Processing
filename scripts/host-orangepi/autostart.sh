#!/bin/bash

source credentials.sh

ssh "$username@$deployment_target" "cd ~/Projects/Titan-Processing && yes \"$password\" | ./scripts/orangepi/autostart.sh"

