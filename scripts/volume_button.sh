#!/bin/sh
case "$1" in
    4) pactl set-sink-mute @DEFAULT_SINK@ toggle ;;
    5) pactl set-sink-volume @DEFAULT_SINK@ 50% ;;
esac
