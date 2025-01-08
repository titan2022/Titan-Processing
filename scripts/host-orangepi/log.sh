#!/bin/bash

source credentials.sh

ssh "$username@$deployment_target" "journalctl -b -u titan2022-apriltag.service"

