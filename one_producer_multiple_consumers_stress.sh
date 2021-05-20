#!/usr/bin/env bash

clients_count=$1

# shellcheck disable=SC2039
pids=()

cmake .

make -j20

# temporary solution (i hope)
/home/egor/HSE/projects/Co-work/first-client-stress.out &

echo $clients_count

producer_pid=$!

# shellcheck disable=SC1073
# shellcheck disable=SC2039
# shellcheck disable=SC2034
for i in $(seq 1 $clients_count); do
  # temporary solution (i hope)
  /home/egor/HSE/projects/Co-work/client-stress.out &
  pids+=($!)
done

# shellcheck disable=SC2068,SC2034,SC2039
for pid in ${pids[*]}; do
  wait $pid
done

kill $producer_pid
