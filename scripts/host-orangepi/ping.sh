#!/bin/bash

source "$( dirname -- "${BASH_SOURCE[0]}" )/credentials.sh"

ping "$remote_hostname"
