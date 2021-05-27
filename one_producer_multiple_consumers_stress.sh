#!/usr/bin/env bash

clients_count=$1

# shellcheck disable=SC2039
#
#cmake .
#
#make -j20

echo $clients_count

# temporary solution (i hope)
/home/egor/HSE/projects/Co-work/first-client-stress.out &

producer_pid=$!

# shellcheck disable=SC1073
# shellcheck disable=SC2039
# shellcheck disable=SC2034

function benchmark() {
  pids=()
  for i in $(seq 1 $1); do
    # temporary solution (i hope)
    /home/egor/HSE/projects/Co-work/client-stress.out &
    pids+=($!)
  done

  # shellcheck disable=SC2068,SC2034,SC2039
  for pid in ${pids[*]}; do
    wait $pid
  done
}

for i in $(seq 1 $2); do
  time benchmark $1
done

kill $producer_pid
