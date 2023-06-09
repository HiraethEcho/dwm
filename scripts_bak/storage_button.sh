#!/bin/sh
case "$1" in
  1) st ranger;;
  2) st ncdu;;
  3) exec thunar;;
  # 4) xbacklight -inc 5 ;;
  # 5) xbacklight -dec 5 ;;
esac

