#!/bin/bash

# This script will return the Jetson to desktop mode.

# Disable the desktop
sudo -S systemctl set-default graphical.target
sudo -S systemctl disable titan2022-apriltag