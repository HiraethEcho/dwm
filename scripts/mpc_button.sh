#!/bin/sh
case "$1" in
    1) mpc toggle  ;;
    2) st ncmpcpp ;;
    3) mpc next ;;
    4) mpc volume +1  ;;
    5) mpc volume -1 ;;
esac
