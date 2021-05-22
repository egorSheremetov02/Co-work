#!/usr/bin/env bash

function request_all_tasks() {
  /home/egor/HSE/projects/Co-work/client-stress.out &
  consumer_pid=$!

  wait $consumer_pid
}

time request_all_tasks
