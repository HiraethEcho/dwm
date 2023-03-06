#!/bin/sh
case $BUTTON in
	1) light -A 5 ;;
	2) "$TERMINAL" -e "$EDITOR" "$0" ;;
	3) light -U 5 ;;
	4) xbacklight -inc 10 ;;
	5) xbacklight -dec 10 ;;
esac
