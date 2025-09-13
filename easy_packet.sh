#!/usr/bin/env bash
# live_capture.sh <host_or_ip> <port>
# Example: ./live_capture.sh example.com 8080

HOST="$1"
PORT="$2"

if [ -z "$HOST" ] || [ -z "$PORT" ]; then
    echo "usage: $0 <host_or_ip> <port>"
    exit 1
fi

# Resolve if a domain name was provided
if [[ "$HOST" =~ [a-zA-Z] ]]; then
    # getent returns IP addresses for the hostname
    IP=$(getent ahosts "$HOST" | awk '/STREAM/ {print $1; exit}')
else
    IP="$HOST"
fi

if [ -z "$IP" ]; then
    echo "could not resolve host"
    exit 1
fi

echo "Capturing packets to/from $HOST ($IP) on port $PORT"

exec sudo tcpdump -i any -s 0 -nn -A -q \
    "host $IP and port $PORT and tcp[13] & 8 != 0"
