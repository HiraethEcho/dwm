#!/bin/sh
#
LIGHT=$(xbacklight -get)

printf "\x11"
# printf " %s%%" "$LIGHT"
printf "  %.0f "  "$(xbacklight)" 
