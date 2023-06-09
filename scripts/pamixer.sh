#!/bin/sh
MUTED=$(pamixer --get-mute)
VOL=$(pamixer --get-volume)

printf "\x0e "
    	if [ "$MUTED" = "true" ]; then
	        if [ "$VOL" -gt -1 ] && [ "$VOL" -le 33 ]; then
	            printf "󰸈 %s" "$VOL"
	        elif [ "$VOL" -gt 33 ] && [ "$VOL" -le 66 ]; then
	            printf "󰸈 %s" "$VOL"
	        else
	            printf "󰸈 %s" "$VOL"
          fi
    	else
	        if [ "$VOL" -gt -1 ] && [ "$VOL" -le 33 ]; then
	            printf "🔈 %s" "$VOL"
	        elif [ "$VOL" -gt 33 ] && [ "$VOL" -le 66 ]; then
	            printf "󰕾 %s" "$VOL"
	        else
	            printf "🔊 %s" "$VOL"
	        fi
      fi
printf " "

# if [ "$MUTED" = true ]; then
#     printf "🔇%s%%" "$VOL"
# else
#   if  [ "$VOL" -le 33 ]; then
#       printf "🔈 %s%%" "$VOL"
#     elif [ "$VOL" -gt 33 ] && [ "$VOL" -le 66 ]; then
#       printf "%s%%" "$VOL"
#     elif [ "$VOL" -gt 66 ] && [ "$VOL" -le 100 ]; then
#       printf "🔊 %s%%" "$VOL"
#   fi
# fi
