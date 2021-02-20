#! /usr/bin/env bash
#sudo apt update
#sudo apt-get install postgresql postgresql-contrib libpqxx-dev

#sudo -u postgres createuser co-work -s -w -r
#sudo -u postgres createdb co-work_db -w
sudo  -u postgres createuser egor -s -w -r
sudo -u postgres createdb test_bd1 -w

sudo -u egor psql --dbname=test_bd1 --file=db_create.sql
