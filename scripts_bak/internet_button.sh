#!/bin/sh
case "$1" in
	# 1) "$TERMINAL" -e nmtui ;;
	1) exec  "nm-connection-editor" ;;
	2) exec st "nmtui" ;;
	3) notify-send "🌐 Internet module" "\- Click to connect
❌: wifi disabled
📡: no wifi connection
📶: wifi connection with quality
❎: no ethernet
🌐: ethernet working
🔒: vpn is active
" ;;
esac
