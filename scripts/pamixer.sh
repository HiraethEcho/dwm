#!/bin/sh
MUTED=$(pamixer --get-mute)
VOL=$(pamixer --get-volume)

if [ "$MUTED" = true ]; then
    printf "🔇%s%" "$VOL"
else
  if [ "$VOL" -gt 0 ] && [ "$VOL" -le 33 ]; then
      printf "🔈%s%" "$VOL"
    elif [ "$VOL" -gt 33 ] && [ "$VOL" -le 66 ]; then
      printf "🔉%s%" "$VOL"
    elif [ "$VOL" -gt 66 ] && [ "$VOL" -le 100 ]; then
      printf "🔊%s%" "$VOL"
  fi
fi
