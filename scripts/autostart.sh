#!/bin/bash
#
#

# /bin/bash ~/suckless/dwm/scripts/dwm_refresh.sh &

feh --randomize --bg-fill --no-fehbg -z ~/pictures/*

picom -b

dwmblocks &

fcitx5 &

