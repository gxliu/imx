#!/bin/sh
ifconfig can0 down

#����CAN�����ʣ�500Kbit/s
echo 500000 > /sys/devices/platform/FlexCAN.0/bitrate

#��ȡ���õĲ�����
echo "can0 bitrate: "
cat /sys/devices/platform/FlexCAN.0/bitrate
echo "bit/s\r\n"

#����CAN�豸
ifconfig can0 up 