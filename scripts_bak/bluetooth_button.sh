#!/bin/sh
#
case "$1" in
  # 2) exec st ;;
  1) notify-send  "Connected:
    $(bluetoothctl devices Connected |grep "Device" |awk '{print $3}')

Paired:
$(bluetoothctl devices Paired |grep "Device" |awk '{print $3}')
    "
    ;;
  3) exec blueberry;;
  esac
