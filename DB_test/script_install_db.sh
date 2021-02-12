#! /usr/bin/env bash
sudo apt update
sudo apt-get install postgresql postgresql-contrib libpqxx-dev

sudo -u postgres createuser test -s -w -r
sudo -u postgres createdb test_db -w

sudo -u test psql --dbname=test_db --file=commands.sql

g++ -std=c++17 a.cpp -lpqxx -lpq
./a.out
