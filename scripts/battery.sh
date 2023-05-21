#!/bin/sh
    # Change BAT1 to whatever your battery is identified as. Typically BAT0 or BAT1
    CHARGE=$(cat /sys/class/power_supply/BAT1/capacity)
    STATUS=$(cat /sys/class/power_supply/BAT1/status)

printf "\x0d "
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
            # printf "🔋%s" "$CHARGE" 
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
printf " "
