#!/bin/bash

source credentials.sh

ssh "$username@$deployment_target" "cd ~/Projects/Titan-Processing && git checkout main --force"
git push "$git_remote" HEAD:__titan_deployment_staging --force
ssh "$username@$deployment_target" "cd ~/Projects/Titan-Processing && git checkout __titan_deployment_staging; ./scripts/orangepi/build.sh"

