#!/bin/sh

# Prints all batteries, their percentage remaining and an emoji corresponding
# to charge status (🔌 for plugged up, 🔋 for discharging on battery, etc.).

case $BUTTON in
	# 3) notify-send "🔋 Battery module" "🔋: discharging
# 🛑: not charging
# 🔌: stagnant charge
# 🔌: charging
# 🔌: charged
# 🔥: battery very low!
# - Scroll to change adjust xbacklight." ;;
	1) xbacklight -inc 10 ;;
	3) xbacklight -dec 10 ;;
	# 6) "$TERMINAL" -e "$EDITOR" "$0" ;;
esac

# Loop through all attached batteries and format the info
for battery in /sys/class/power_supply/BAT?*; do
	# If non-first battery, print a space separator.
	[ -n "${capacity+x}" ] && printf " "
	# Sets up the status and capacity
	case "$(cat "$battery/status" 2>&1)" in
		"Full") status="🔌[OK]:" ;;
		"Discharging") status="🔋:" ;;
		"Charging") status="🔌:" ;;
		"Not charging") status="🛑:" ;;
		"Unknown") status="🔌:" ;;
		*) exit 1 ;;
	esac
	capacity="$(cat "$battery/capacity" 2>&1)"
	# Will make a warn variable if discharging and low
	[ "$status" = "🔋" ] && [ "$capacity" -le 25 ] && warn="🔥" && mpv --no-video --no-terminal ./low.power.mp3
	# Prints the info
	printf "%s%s%d%%" "$status" "$warn" "$capacity"; unset warn
done && printf "\\n"
