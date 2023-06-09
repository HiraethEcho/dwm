#!/bin/sh
case "$1" in
    1) exec "st" -e  htop -s PERCENT_CPU ;;
    2) st  htop ;;
    3) exec "$TERMINAL" -e htop -s PERCENT_MEM ;;
    4) exec "$TERMINAL" -e htop -s PERCENT_MEM ;;
esac
