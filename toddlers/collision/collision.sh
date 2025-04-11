#!/usr/bin/env bash

./col $(printf '\xc8\xce\xc5\x06%.0s' {1..4} && printf '\xcc\xce\xc5\x06')
