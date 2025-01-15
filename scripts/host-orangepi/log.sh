#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

sshpass -p "$password" ssh "$username@$remote_hostname" "journalctl -b -u titan2022-apriltag.service"

