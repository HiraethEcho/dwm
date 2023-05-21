#!/bin/sh

case "$1" in
  1) xbacklight -set 70;;
  2) xbacklight -set 30;;
  3) xbacklight -set 100;;
  4) xbacklight -inc 5 ;;
  5) xbacklight -dec 5 ;;
esac
