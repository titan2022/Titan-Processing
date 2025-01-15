#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

ssh "$username@$remote_hostname" "cd ~/Projects/Titan-Processing && git checkout main --force"
git push "$git_remote" HEAD:__titan_deployment_staging --force
ssh "$username@$remote_hostname" "cd ~/Projects/Titan-Processing && git checkout __titan_deployment_staging; ./scripts/orangepi/build.sh"

