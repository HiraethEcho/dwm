#!/bin/sh
LIGHT=$(xbacklight -get)

printf " %s%%" "$LIGHT"
