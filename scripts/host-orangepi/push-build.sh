#!/bin/bash

git_remote=$1
deployment_target=$2

if [[ -z "$git_remote"  || -z "$deployment_target" ]]; then
    echo "Error: You didn't specify a git remote or deployment target."
    echo ""
    echo "Usage: scripts/host-orangepi/push-build.sh [git remote] [deployment target]"
    echo "  where [git remote] is the name of git remote that you want to push to, and"
    echo "  [deployment target] is the IP address or SSH name of the remote machine."
    echo ""
    echo "  [git remote] should be the name that you set up with set-up-push.sh."
    exit 1
fi

ssh "titan@$deployment_target" "cd /home/titan/Projects/Titan-Processing && git checkout main --force"
git push "$git_remote" HEAD:__titan_deployment_staging --force
ssh "titan@$deployment_target" "cd /home/titan/Projects/Titan-Processing && git checkout __titan_deployment_staging; rm -r build; mkdir build && cd build; cmake -B . -S .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake -GNinja; ninja"

