#!/bin/bash

#/usr/bin/python /home/pi/theLAB/raspi/led_blink.py
#/usr/bin/pd -nogui -open /home/pi/Desktop/shell_test.pd
#nimm das hier unten
#/usr/bin/pd -nogui -rt -open /home/pi/kavd_container/pd/LAB_main.pd
#/usr/bin/pd -nogui -rt -open /home/pi/kavd_container/pd/synthtest.pd
sudo /usr/bin/pd -nogui -rt -open /home/pi/kavd_container/pd/LAB_main.pd -send "raspiOrNot 1"
