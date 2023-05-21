#!/bin/sh
case "$1" in
    1) pamixer -u ;;
    2) exec pavucontrol ;;
    3) pamixer -m ;;
    4) pamixer --increase 5 ;;
    5) pamixer --decrease 5 ;;
esac
