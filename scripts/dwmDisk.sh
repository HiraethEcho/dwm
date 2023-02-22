#!/bin/sh

# Status bar module for disk space
# $1 should be drive mountpoint, otherwise assumed /.

location=${1:-/}

# case $BUTTON in
# 	1) notify-send "💽 硬盘空间情况" "$(df -h --output=target,used,size)" ;;
# 	3) notify-send "💽 硬盘模块" "\- 查看硬盘使用率和总量.
# - 左键点击查看所有硬盘空间情况." ;;
# 	6) "$TERMINAL" -e "$EDITOR" "$0" ;;
# esac

[ -d "$location" ] || exit


# printf "%s:%s\n" "$icon" "$(df -h "$location" | awk ' /[0-9]/ {print $3 "/" $2}')"
printf "%s\n" "$(df -h "$location" | awk ' /[0-9]/ {print $3 "/" $2}')"
