#!/usr/bin/env bash
sudo apt update
sudo apt-get install postgresql postgresql-contrib libpqxx-dev

sudo -u postgres createuser test -s
sudo -u postgres createdb test

sudo -u test psql -f commands.txt

g++ -std=c++17 a.cpp -lpqxx -lpq
./a.out
