#!/bin/bash

source credentials.sh

ssh "$username@$remote_hostname" "journalctl -b -u titan2022-apriltag.service"

