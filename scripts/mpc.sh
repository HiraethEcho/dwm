#!/bin/sh
if ps -C mpd > /dev/null; then
    ARTIST=$(mpc current -f %artist%)
    TRACK=$(mpc current -f %title%)
    POSITION=$(mpc status | grep "%)" | awk '{ print $3 }' | awk -F/ '{ print $1 }')
    DURATION=$(mpc current -f %time%)
    STATUS=$(mpc status | sed -n 2p | awk '{print $1;}')
    SHUFFLE=$(mpc status | tail -n 1 | awk '{print $6}')

        if [ "$STATUS" = "[playing]" ]; then
            STATUS="▶"
        else
            STATUS="⏸"
        fi

        if [ "$SHUFFLE" = "on" ]; then
            SHUFFLE=" 🔀"
        else
            SHUFFLE=""
        fi
printf "\x0c"
    printf " %s %s-%s %s/%s %s "  "$STATUS" "$ARTIST" "$TRACK" "$POSITION" "$DURATION" "$SHUFFLE"
    # printf " %s %s "  "$STATUS"  "$TRACK" 
fi
