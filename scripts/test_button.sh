#!/bin/sh
#
dev=$(bluetoothctl info | grep "Name" | awk '{print $2 $3}')
case "$1" in
  1) notify-send "$dev Paired: $(bluetoothctl info | grep "Paired" | awk '{print $2}')"
    ;;
  2) notify-send "Connected:
    $(bluetoothctl devices Connected |grep "Device" |awk '{print $3}')

Paired:
$(bluetoothctl devices Paired |grep "Device" |awk '{print $3}')
    "
    ;;

  3) exec blueberry;;
  esac
