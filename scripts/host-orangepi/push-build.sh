#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

sshpass -p "$password" "$username@$remote_hostname" "cd ~/Projects/Titan-Processing && git checkout main --force"
git push "$git_remote" HEAD:__titan_deployment_staging --force
sshpass -p "$password" ssh "$username@$remote_hostname" "cd ~/Projects/Titan-Processing && git checkout __titan_deployment_staging; ./scripts/orangepi/build.sh"

