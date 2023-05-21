#!/bin/sh
# updates=$({ timeout 20 checkupdates 2>/dev/null || true; } | wc -l) # arch
#   if [ -z "$updates" ]; then
#     printf "  ^c$green^    Fully Updated"
#   else
#     printf "  ^c$green^    $updates"" updates"
#   fi

printf "\x0b test:"

dev=$(bluetoothctl info | grep "Device" | awk '{print $2}')

status=$(bluetoothctl show | grep "Powered" | awk '{print $2}')

if [ "$status" == "yes" ]; then
    printf " " 
else
    printf "󰂲 "
fi


read -r rate </sys/class/power_supply/BAT0/current_now
# printf "%s" "$rate"
# [ "$rate" = 0 ] && notify-send "Battery fully charged" && exit
read -r ac </sys/class/power_supply/AC/online
# printf "%s" "$ac"

read -r charge_now </sys/class/power_supply/BAT0/charge_now
# printf "%s" "$charge_now"

if [ "$ac" = 1 ] ; then
    read -r charge_full </sys/class/power_supply/BAT0/charge_full
    val="$(( charge_full-charge_now ))"
else
    val="$charge_now"
fi
hr="$(( val / rate ))"
mn="$(( (val * 60) / rate - hr * 60 ))"

# printf "%s%s%s%s%s" "$charge_now"  "$ac" "$val" "$rate" "$hr" "$mn"
