#! /usr/bin/env bash

read -r -p "Input username: " username

sudo -u "$username" psql --dbname=co-work_db --file=update_db.sql
