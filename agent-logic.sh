#!/bin/sh

# shell script to run ssh-agent if it's not running
# also adds all keys specified as arguments
# I keep this in my .zshrc as eval $(~/scripts/agent-logic.sh ~/.ssh/id_ed25519)

# do we already have a socket set?
[[ -z "${SSH_AUTH_SOCK}" ]] || exit 0

# maybe the socket exists, but isn't loaded as a variable
SSH_AUTH_SOCK="$(find /tmp/ssh-*/agent.* 2> /dev/null | head -n1)"
export SSH_AUTH_SOCK


if [[ -z "${SSH_AUTH_SOCK}" ]]; then
    # nope, no socket
    eval $(ssh-agent -s) > /dev/null
else
    # pid is usually socket extension + 1
    SSH_AGENT_PID=$(( ${SSH_AUTH_SOCK##*.} + 1 ))
    export SSH_AGENT_PID
fi

# simulate ssh-agent output
echo "SSH_AUTH_SOCK=${SSH_AUTH_SOCK}; export SSH_AUTH_SOCK;"
echo "SSH_AGENT_PID=${SSH_AGENT_PID}; export SSH_AGENT_PID;"
# echo "echo Agent pid ${SSH_AGENT_PID};"

[[ -z "$@" ]] && exit 0 # no keys to add

KEYS=$(ssh-add -l)

for key in "$@"; do
    KEYFP=$(ssh-keygen -lf "$key")

    # if our key is already added, don't operate on it
    echo "$KEYS" | grep "$KEYFP" -q
    if [[ "$?" -ne "0" ]]; then
        ssh-add -q "$key" > /dev/null
    fi
done

exit 0
