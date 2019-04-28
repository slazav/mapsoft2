#!/bin/bash -fu

function assert(){
  cmd="$1"
  exp="$2"
  res="$($cmd 2>&1)"
  ret="$?"
  rete="${3:-''}"
  if [ "$exp" != "$res" ]; then
    printf "ERROR ($cmd):\n"
    printf "  exp:\n%s\n" "$exp"
    printf "  res:\n%s\n" "$res"
    exit 1
  fi
  if [ "$rete" != "" -a "$ret" != "$rete" ]; then
    printf "ERROR in return code ($cmd):\n"
    printf "  exp:\n%s\n" "$rete"
    printf "  ret:\n%s\n" "$ret"
    exit 1
  fi
}
