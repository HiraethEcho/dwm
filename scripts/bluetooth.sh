#!/bin/sh
status=$(bluetoothctl show | grep "Powered" | awk '{print $2}')

if [ "$status" == "yes" ]; then
  printf "  %s" "$(bluetoothctl devices Connected |grep "Device" |awk '{print $3}')"
else
    printf "󰂲 "
fi
