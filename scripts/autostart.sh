#!/bin/bash
#
#

# /bin/bash ~/suckless/dwm/scripts/dwm_refresh.sh &
killall nm-applet

feh --randomize --bg-fill --no-fehbg -z ~/pictures/*

picom -b

dwmblocks &

fcitx5 &

nm-applet &

# pnmixer &

