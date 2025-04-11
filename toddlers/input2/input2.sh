#!/usr/bin/env bash

printf "\x00\x0a\x00\xff" | env $'\xde\xad\xbe\xef'=$'\xca\xfe\xba\xbe' ./input2 $(printf "a %.0s" {1..64}) $'\x00' $'\x20\x0a\x0d' "6666" $(printf "a %.0s" {68..99}) 2< <(echo -n -e "\x00\x0a\x02\xff") &

echo -e "\xde\xad\xbe\xef" | nc 0 6666
