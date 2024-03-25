#!/bin/bash

# This script will return the Jetson to desktop mode.

# Disable the desktop
sudo systemctl set-default graphical.target
sudo systemctl disable titan2022-apriltag