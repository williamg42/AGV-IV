#!/bin/bash

echo "Installing RC service to /opt/RC/."

mkdir -p /opt/RC

cp -f RC.sh /opt/RC/RC.sh

chmod o+x /opt/RC/RC.sh

echo "Installing systemd service to run at boot."

cp -f RC.service /lib/systemd/system/RC.service

echo "Enabling systemd service."

systemctl enable RC.service > /dev/null
