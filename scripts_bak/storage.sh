#!/bin/sh
#
df_check_location='/home'
df_output=$(df -h $df_check_location | tail -n 1)
STOUSED=$(echo $df_output | awk '{print $3}')
STOTOT=$(echo $df_output | awk '{print $2}')

printf "\x0e"
printf " 󰋊 %s/%s "   "$STOUSED" "$STOTOT" 
