#!/bin/sh

# Prints the current volume or 🔇 if muted.

case $BUTTON in
	1) setsid -f "$TERMINAL" -e pulsemixer ;;
	2) pamixer -t; pkill -RTMIN+7 dwmblocks ;;
	4) pamixer --allow-boost -i 1;;
	5) pamixer --allow-boost -d 1;;
	# 3) notify-send "📢 音量模块" "\- 音量调节:🔇,🔈,🔉,🔊
# - 左键点击打开pulsemixer
# - 中键点击静音.
# - 滑轮上下调整音量." ;;
	# 6) "$TERMINAL" -e "$EDITOR" "$0" ;;
esac

MUTED=$(pamixer --get-mute)
# VOL=$(pamixer --get-volume)

VOL=$(amixer get Master | tail -n1 | sed -r "s/.*\[(.*)%\].*/\1/")

# if [ "$MUTED" = true ]; then
#     printf "🔇:%s%%" "$VOL"
#     return
# fi

if [ "$VOL" -eq 0 ]; then
    printf "🔇:%s%%" "$VOL"
elif [ "$VOL" -gt 00 ] && [ "$VOL" -le 30 ]; then
    printf "🔈:%s%%" "$VOL"
elif [ "$VOL" -gt 30 ] && [ "$VOL" -le 70 ]; then
    printf "🔉:%s%%" "$VOL"
else
    printf "🔊:%s%%" "$VOL"
fi
