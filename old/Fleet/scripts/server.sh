#!/bin/bash

sudo apt update -y
sudo apt upgrade -y
echo '------------ Installing git -------------'
sudo apt-get install git -y
cd ~/Desktop

# git clone https://github.com/adhokshaj-mishra/fleet || exit 1
echo '-------------- Logging in to Github --------------------'
read -p 'Input your github username: ' username
echo 'remote: Support for password authentication was removed on August 13, 2021. Please use a personal access token instead. \n remote: Please see https://github.blog/2020-12-15-token-authentication-requirements-for-git-operations/ for more information.'
read -sp 'Input your github PAT: ' password
while ! git clone "https://github.com/adhokshaj-mishra/fleet"; do
  echo 'Error cloning, please retry. Enter q to skip' >&2;
  read key <&1
        if [ "$key" = q ] ; then
            break
        fi
  read -p 'Input your github username: ' username
  read -sp 'Input your github password: ' password
done



cd fleet
sudo mkdir -p cmake-build-debug
cd ~/Desktop/fleet/cmake-build-debug
sudo apt install curl -y
sudo apt-get install libcurl4-openssl-dev -y
sudo curl -o ~/Desktop/fleet/cmake-build-debug/config.json https://gist.githubusercontent.com/sablearjun/65fbf069af759ca865ef2da001d18765/raw/b84843a25503e170c88b020cf1a911bf2a24f68b/config.json

# Install Cmake
sudo apt-get install cmake -y

# Build essentials
sudo apt-get update && sudo apt-get install build-essential -y

# OpenSSL
sudo apt-get install libssl-dev -y

#JSONCpp
sudo apt-get install libjsoncpp-dev -y

# Boost
sudo apt-get install libboost-all-dev -y

# Postgresql
sudo apt install postgresql-client postgresql libpqxx-dev libssl-dev -y

# Create PSQL User

sudo -u postgres bash -c "psql -c \"CREATE USER sablearjun WITH PASSWORD 'admin';\""

# Create database
sudo -u postgres psql -c 'create database fleet;'

# Create tables
sudo -u postgres psql -d fleet -a -f ~/Desktop/fleet/sql/create_tables.sql

sudo mkdir -p /var/log/fleet

sudo chown -R $USER /var/log/fleet

# Libproc
sudo apt-get install libprocps-dev:i386 libprocps-dev -y
sudo cmake ..
sudo make


# Front End Setup

echo '------------ Setting Up Front End --------------'
sudo apt install python3.8 -y
sudo apt install python3-pip -y
sudo pip3 install -r ~/Desktop/fleet/Front-end/requirements.txt

echo '--------------------- Enter your desired Username and Password to login into Front End Web tool ----------------------'
read -p 'Username : ' uname
read -sp 'Password : ' pass
bearer=`sudo ~/Desktop/fleet/cmake-build-debug/userctl --add --username $uname --secret admin --token qwerty --issuer sablearjun`
echo "$bearer"

bearer_token="inspector[\"Authorization\"] = \"$bearer\""
echo $bearer_token

sudo sed -i "22s/.*/$bearer_token/" ~/Desktop/fleet/Front-end/dashboard/views.py

url="url='https:\/\/127.0.0.1:1235\/'"
echo $url
sudo sed -i "26s/.*/$url/" ~/Desktop/fleet/Front-end/dashboard/views.py

echo "from django.contrib.auth.models import User; User.objects.create_superuser(\"$uname\", 'admin@example.com', \"$pass\")" | python3 ~/Desktop/fleet/Front-end/manage.py shell

echo '---------------- Setup complete --------------------'
echo '--------------- You can login to front end using your given credentials ---------------------'