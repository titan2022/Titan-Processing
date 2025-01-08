#!/bin/bash

source credentials.sh

ssh "$username@$remote_hostname" "yes \"$password\" | sudo -S reboot now"

