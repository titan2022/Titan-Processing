#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

ssh "$username@$remote_hostname" "yes \"$password\" | sudo -S reboot now"

