#! /usr/bin/env bash
sudo apt update
sudo apt-get install postgresql postgresql-contrib libpqxx-dev

sudo -u postgres createuser co-work -s -w -r
sudo -u postgres createdb co-work_db -w

sudo -u co-work psql --dbname=co-work_db --file=db_create.sql
