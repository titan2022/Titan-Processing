#!/bin/bash

git_remote=$1
deployment_target=$2

if [[ -z "$git_remote"  || -z "$deployment_target" ]]; then
    echo "Error: You didn't specify a git remote or deployment target."
    echo ""
    echo "Usage: scripts/host-orangepi/set-up-push.sh [git remote] [deployment target]"
    echo "  where [git remote] is the name of git remote that you want to push to, and"
    echo "  [deployment target] is the IP address or SSH name of the remote machine."
    echo ""
    echo "  [git remote] should be a name that is not already used."
    exit 1
fi

git remote add "$git_remote" "titan@$deployment_target:/home/titan/Projects/Titan-Processing"
ssh "titan@$deployment_target" "cd /home/titan/Projects/Titan-Processing && git branch __titan_deployment_staging main"
