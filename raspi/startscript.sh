#!/bin/bash

#/usr/bin/python /home/pi/theLAB/raspi/led_blink.py

#nimm das hier unten
#sudo /usr/bin/pd -rt -open /home/pi/kavd_container/pd/LAB_main.pd -send "raspiOrNot 1"
sudo /usr/bin/pd -nogui -rt -audiobuf 300 -blocksize 2048 -open /home/pi/kavd_container/pd/LAB_main.pd -send "raspiOrNot 1"
