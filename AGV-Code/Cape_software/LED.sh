#!/bin/bash

echo 30 > /sys/class/gpio/export
echo low > /sys/class/gpio/gpio30/direction

while true

do



echo 1 > /sys/class/gpio/gpio30/value

sleep .$[ ( $RANDOM % 4 ) + 1 ]s

echo 0 > /sys/class/gpio/gpio30/value

sleep .$[ ( $RANDOM % 4 ) + 1 ]s

	

done
