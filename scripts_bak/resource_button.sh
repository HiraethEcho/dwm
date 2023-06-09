#!/bin/sh
case "$1" in
    # 1) st ;;
    2) st  htop ;;
    # 3) exec "st" -e htop -s PERCENT_MEM ;;
esac
