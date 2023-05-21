#!/bin/sh

# Show wifi 📶 and percent strength or 📡 if none.
# Show 🌐 if connected to ethernet or ❎ if none.
# Show 🔒 if a vpn connection is active

printf "\x0f"
    CONNAME=$(nmcli -a | grep 'Wired connection' | awk 'NR==1{print $1}')
    if [ "$CONNAME" = "" ]; then
        CONNAME=$(nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -c 5-)
    fi

if grep -xq 'up' /sys/class/net/w*/operstate 2>/dev/null ; then
	# wifiicon="$(awk '/^\s*w/ { print "📶", int($3 * 100 / 70) }' /proc/net/wireless)"
	wifiicon="$(awk '/^\s*w/ { print "📶" }' /proc/net/wireless)"
elif grep -xq 'down' /sys/class/net/w*/operstate 2>/dev/null ; then
	grep -xq '0x1003' /sys/class/net/w*/flags && wifiicon="📡 " || wifiicon="❌ "
fi

printf " %s:%s%s%s " "$wifiicon" "$(sed "s/down/❎/;s/up/🌐/" /sys/class/net/e*/operstate 2>/dev/null)" "$CONNAME"  "$(sed "s/.*/🔒/" /sys/class/net/tun*/operstate 2>/dev/null)"

# Dependencies: NetworkManager, curl

# dwm_network () {
#     CONNAME=$(nmcli -a | grep 'Wired connection' | awk 'NR==1{print $1}')
#     if [ "$CONNAME" = "" ]; then
#         CONNAME=$(nmcli -t -f active,ssid dev wifi | grep '^yes' | cut -c 5-)
#     fi

#     PRIVATE=$(nmcli -a | grep 'inet4 192' | awk '{print $2}')
#     PUBLIC=$(curl -s https://ipinfo.io/ip)

#         # printf "🌐 %s %s | %s" "$CONNAME" "$PRIVATE" "$PUBLIC"
#         printf "🌐 %s" "$CONNAME"
# }

# dwm_network
