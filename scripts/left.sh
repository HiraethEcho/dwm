#!/bin/sh

df_check_location='/home'
df_output=$(df -h $df_check_location | tail -n 1)
free_output=$(free -h | grep Mem)
cache=/tmp/cpubarscache
stats=$(awk '/cpu[0-9]+/ {printf "%d %d %d\n", substr($1,4), ($2 + $3 + $4 + $5), $5 }' /proc/stat)
[ ! -f $cache ] && echo "$stats" > "$cache"
old=$(cat "$cache")
STOUSED=$(echo $df_output | awk '{print $3}')
STOTOT=$(echo $df_output | awk '{print $2}')

CPU=$(top -bn1 | grep Cpu | awk '{print $2}')%
MEMUSED=$(echo $free_output | awk '{print $3}')
MEMTOT=$(echo $free_output | awk '{print $2}')
STOPER=$(echo $df_output | awk '{print $5}')

printf "]"


#function get_bytes {
#    # Find active network interface
#    #interface=$(ip route get 8.8.8.8 2>/dev/null | grep 'dev \S\+' -o | awk '{print $2}')
#    local interface=$(route | grep '^default' | grep -o '[^ ]*$')
#    local bytes_concat_args=$(grep ${interface:-lo} /proc/net/dev | cut -d ':' -f 2 | awk -v rb="$1" -v tb="$2" '{print rb"="$1, tb"="$9}')
#    eval $bytes_concat_args
#}

## Function which calculates the speed using actual and old byte number.
## Speed is shown in KByte per second when greater or equal than 1 KByte per second.
## This function should be called each second.
## parameters: value, old_value, now, old_time
#function get_velocity {
#    local timediff=$(($3 - $4))
#    local vel_kb=$(echo "1000000000 * ($1 - $2) / 1024 / $timediff" | bc)
#    if test "$vel_kb" -gt 1024; then
#        echo $(echo "scale = 2; $vel_kb / 1024" | bc)MB/s
#    else
#        echo ${vel_kb}KB/s
#    fi
#}

#function dwm_network_speed_record {
#    get_bytes 'received_bytes' 'transmitted_bytes'
#    old_received_bytes=$received_bytes
#    old_transmitted_bytes=$transmitted_bytes

#    old_time=$(date +%s%N)
#}

#function download_speed {
#    get_velocity $received_bytes $old_received_bytes $now $old_time
#}

#function upload_speed {
#    get_velocity $transmitted_bytes $old_transmitted_bytes $now $old_time
#}

## The greater interval ($now - $old_time) is, the be exacter the speed is.
#function dwm_network_speed {
#    get_bytes 'received_bytes' 'transmitted_bytes'
#    now=$(date +%s%N)

#        printf "李 %s %s"  "$(download_speed)" "$(upload_speed)"
#}

#dwm_network_speed_record
#dwm_network_speed


printf " 󰋊 %s/%s  "   "$STOUSED" "$STOTOT" 

printf "󰍛 %s/%s  " "$MEMUSED" "$MEMTOT" 
#
printf "󰘚 %s "  "$CPU" 
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

