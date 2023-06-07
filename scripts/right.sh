#!/bin/sh

bluetoothstatus=$(bluetoothctl show | grep "Powered" | awk '{print $2}')
CONNAME=$(nmcli -a | grep 'Wired connection' | awk 'NR==1{print $1}')
if [ "$CONNAME" = "" ]; then
    CONNAME=$(nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -c 5-)
fi
if grep -xq 'up' /sys/class/net/w*/operstate 2>/dev/null ; then
	wifiicon="$(awk '/^\s*w/ { print "📶" }' /proc/net/wireless)"
elif grep -xq 'down' /sys/class/net/w*/operstate 2>/dev/null ; then
	grep -xq '0x1003' /sys/class/net/w*/flags && wifiicon="📡 " || wifiicon="❌ "
fi

printf "] "
printf "%s%s%s%s" "$wifiicon" "$(sed "s/down/❎/;s/up/🌐/" /sys/class/net/e*/operstate 2>/dev/null)" "$CONNAME"  "$(sed "s/.*/🔒/" /sys/class/net/tun*/operstate 2>/dev/null)"
printf "|"
CHARGE=$(cat /sys/class/power_supply/BAT1/capacity)
STATUS=$(cat /sys/class/power_supply/BAT1/status)
        if [ "$STATUS" = "Charging" ]; then
	        if [ "$CHARGE" -gt -1 ] && [ "$CHARGE" -le 10 ]; then
	            printf "󰢜 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 10 ] && [ "$CHARGE" -le 20 ]; then
	            printf "󰂆 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 20 ] && [ "$CHARGE" -le 30 ]; then
	            printf "󰂈 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 30 ] && [ "$CHARGE" -le 40 ]; then
	            printf "󰂈 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 40 ] && [ "$CHARGE" -le 50 ]; then
	            printf "󰢝 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 50 ] && [ "$CHARGE" -le 60 ]; then
	            printf "󰂉 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 60 ] && [ "$CHARGE" -le 70 ]; then
	            printf "󰂉 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 70 ] && [ "$CHARGE" -le 80 ]; then
	            printf "󰢞 %s" "$CHARGE"
	        elif [ "$CHARGE" -gt 80 ] && [ "$CHARGE" -le 90 ]; then
	            printf "󰂋 %s" "$CHARGE"
	        else
	            printf "󰂅 %s" "$CHARGE"
          fi
        else
	        if [ "$CHARGE" -gt -1 ] && [ "$CHARGE" -le 10 ]; then
	            printf "󰁺%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 10 ] && [ "$CHARGE" -le 20 ]; then
	            printf "󰁻%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 20 ] && [ "$CHARGE" -le 30 ]; then
	            printf "󰁼%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 30 ] && [ "$CHARGE" -le 40 ]; then
	            printf "󰁽%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 40 ] && [ "$CHARGE" -le 50 ]; then
	            printf "󰁾%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 50 ] && [ "$CHARGE" -le 60 ]; then
	            printf "󰁿%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 60 ] && [ "$CHARGE" -le 70 ]; then
	            printf "󰂀%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 70 ] && [ "$CHARGE" -le 80 ]; then
	            printf "󰂁%s" "$CHARGE"
	        elif [ "$CHARGE" -gt 80 ] && [ "$CHARGE" -le 90 ]; then
	            printf "󰂂%s" "$CHARGE"
	        else
	            printf "󰁹%s" "$CHARGE"
          fi
        fi
printf "|"
if [ "$bluetoothstatus" == "yes" ]; then
  printf "%s" "$(bluetoothctl devices Connected |grep "Device" |awk '{print $3}')"
else
    printf "󰂲"
fi
