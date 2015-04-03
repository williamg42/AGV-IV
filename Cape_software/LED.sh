#!/bin/bash



while true

do



echo 1 > /sys/class/gpio/gpio30/value

sleep .$[ ( $RANDOM % 4 ) + 1 ]s

echo 0 > /sys/class/gpio/gpio30/value

sleep .$[ ( $RANDOM % 4 ) + 1 ]s

	

done
