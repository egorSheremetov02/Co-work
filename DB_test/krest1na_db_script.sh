#! /usr/bin/env bash
#sudo apt update
#sudo apt-get install postgresql postgresql-contrib libpqxx-dev

#sudo -u postgres createuser co-work -s -w -r
#sudo -u postgres createdb co-work_db -w
sudo  -u postgres createuser krestino4ka -s -w -r
sudo -u postgres createdb co-db -w

sudo -u krestino4ka psql --dbname=co-db --file=db_create.sql
