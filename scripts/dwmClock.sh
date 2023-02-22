#!/bin/sh

clock=$(date '+%MM/%DD')

# case "$clock" in
# 	"00") clock_icon="🕛" ;;
# 	"01") clock_icon="🕐" ;;
# 	"02") clock_icon="🕑" ;;
# 	"03") clock_icon="🕒" ;;
# 	"04") clock_icon="🕓" ;;
# 	"05") clock_icon="🕔" ;;
# 	"06") clock_icon="🕕" ;;
# 	"07") clock_icon="🕖" ;;
# 	"08") clock_icon="🕗" ;;
# 	"09") clock_icon="🕘" ;;
# 	"10") clock_icon="🕙" ;;
# 	"11") clock_icon="🕚" ;;
# 	"12") clock_icon="🕛" ;;
# esac

# case $BUTTON in
# 	1) notify-send "月份信息" "$(cal --color=always | sed "s/..7m/<b><span color=\"red\">/;s/..27m/<\/span><\/b>/")" && notify-send "Appointments" "$(calcurse -D ~/.config/calcurse -d3)" ;;
# 	2) setsid -f "$TERMINAL" -e calcurse ;;
# 	3) notify-send "📆 日期模块" "\- 查看月份信息
# - 左键点击显示日历和未来三天的计划
# - 中键点击打开calcurse(如果已安装)" ;;
# 	6) "$TERMINAL" -e "$EDITOR" "$0" ;;
# esac

LOCALTIME=$(date +"%m/%d-%H:%M")
#LOCALTIME=$(date +"${clock_icon}:%H:%M")
printf "${LOCALTIME}"

