#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

git remote set-url "$git_remote" "$username@$remote_hostname:/home/$username/Projects/Titan-Processing"
