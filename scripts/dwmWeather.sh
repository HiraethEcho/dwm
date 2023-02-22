#!/bin/sh

# Displays todays precipication chance (☔) and daily low (🥶) and high (🌞).
# Usually intended for the statusbar.

# If we have internet, get a weather report from wttr.in and store it locally.
# You could set up a shell alias to view the full file in a pager in the
# terminal if desired. This function will only be run once a day when needed.
weatherreport="${XDG_DATA_HOME:-$HOME/.local/share}/weatherreport"
# getforecast() { curl -sf "wttr.in/CTU?m" > "$weatherreport" || exit 1 ;}
getforecast() { curl -sf "wttr.in/beijing?m" > "$weatherreport" || exit 1 ;}

# Some very particular and terse stream manipulation. We get the maximum
# precipication chance and the daily high and low from the downloaded file and
# display them with coresponding emojis.
showweather() { printf "%s" "$(sed '16q;d' "$weatherreport" |
	grep -wo "[0-9]*%" | sort -rn | sed "s/^/☔/g;1q" | tr -d '\n')"
sed '13q;d' "$weatherreport" | grep -o "m\\([-+]\\)*[0-9]\\+" | sort -n -t 'm' -k 2n | sed -e 1b -e '$!d' | tr '\n|m' ' ' | awk '{print  $1"°","/",$2"°"}' ;}

case $BUTTON in
	1) setsid -f "$TERMINAL" -e less -Srf "$weatherreport" ;;
	3) getforecast && showweather && pkill -RTMIN+8 dwmblocks ;;
	# 3) notify-send "🌈 天气预报模块" "\- 左键点击查看完整预测.
# - 中键点击更新天气预报.
# ☔: 百分之多少几率可能会下雨
# 🥶: 当日最低温度
# 🌞: 当日最高温度" ;;
	# 6) "$TERMINAL" -e "$EDITOR" "$0" ;;
esac

# The test if our forcecast is updated to the day. If it isn't download a new
# weather report from wttr.in with the above function.
[ "$(stat -c %y "$weatherreport" 2>/dev/null | cut -d' ' -f1)" = "$(date '+%Y-%m-%d')" ] ||
	getforecast

showweather
