#!/bin/bash

echo "Installing LED service to /opt/LED/."

mkdir -p /opt/LED

cp -f LED.sh /opt/LED/LED.sh

chmod o+x /opt/LED/LED.sh

echo "Installing systemd service to run at boot."

cp -f LED.service /lib/systemd/system/LED.service

echo "Enabling systemd service."

systemctl enable LED.service > /dev/null
