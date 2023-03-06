#!/bin/sh

STATUS=$(amixer sget Master | tail -n1 | sed -r "s/.*\[(.*)\]/\1/")
case "$1" in
    # 1)  
  # if [ "$STATUS" = "off" ]; then
    # amixer sset Master unmute
  # else
    # amixer sset Master mute
  # fi ;; 
    # 2) 
    3) exec "$TERMINAL" -e alsamixer;;
esac
