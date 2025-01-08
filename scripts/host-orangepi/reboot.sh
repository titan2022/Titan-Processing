#!/bin/bash

source credentials.sh

ssh "$username@$deployment_target" "yes \"$password\" | sudo -S reboot now"

