#!/bin/sh
#
df_check_location='/home'
free_output=$(free -h | grep Mem)
cache=/tmp/cpubarscache
stats=$(awk '/cpu[0-9]+/ {printf "%d %d %d\n", substr($1,4), ($2 + $3 + $4 + $5), $5 }' /proc/stat)
[ ! -f $cache ] && echo "$stats" > "$cache"
old=$(cat "$cache")

CPU=$(top -bn1 | grep Cpu | awk '{print $2}')%
MEMUSED=$(echo $free_output | awk '{print $3}')
MEMTOT=$(echo $free_output | awk '{print $2}')
# STOPER=$(echo $df_output | awk '{print $5}')

printf "\x0c"
printf " 󰍛 %s/%s|" "$MEMUSED" "$MEMTOT" 
# printf "󰍛 %s/%s|󰻠 %s|󰋊 %s/%s" "$MEMUSED" "$MEMTOT" "$CPU" "$STOUSED" "$STOTOT" 
#
printf "󰘚 %s "  "$CPU" 
# echo "$stats" | while read -r row; do
# id=${row%% *}
# rest=${row#* }
# total=${rest%% *}
# idle=${rest##* }

# case "$(echo "$old" | awk '{if ($1 == id)
#   printf "%d\n", (1 - (idle - $3)  / (total - $2))*100 /12.5}' \
#     id="$id" total="$total" idle="$idle")" in

#   "0") printf "▁";;
#   "1") printf "▂";;
#   "2") printf "▃";;
#   "3") printf "▄";;
#   "4") printf "▅";;
#   "5") printf "▆";;
#   "6") printf "▇";;
#   "7") printf "█";;
#   "8") printf "█";;
# esac
# done;
# echo "$stats" > "$cache"

