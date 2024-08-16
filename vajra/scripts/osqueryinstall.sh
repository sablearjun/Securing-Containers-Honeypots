#!/bin/bash

echo " Setting up your system please wait ..."

# Installing OSQuery
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1484120AC4E9F8A1A577AEEE97A80C63C9D8B80B  
# sudo add-apt-repository "deb [arch=amd64] https://osquery-packages.s3.amazonaws.com/xenial xenial main"  
sudo add-apt-repository 'deb [arch=amd64] https://pkg.osquery.io/deb deb main'
sudo apt-get update  
sudo apt-get install osquery

# Curl
sudo apt install curl -y

# Copying OSQuery flags
sudo curl -o /etc/osquery/cert.pem https://gist.githubusercontent.com/sablearjun/65fbf069af759ca865ef2da001d18765/raw/ecbd40e6e0fc56e0cb95dc96fdcf94e6cd5188d7/cert.pem
sudo curl -o /etc/osquery/enrollment_secret https://gist.githubusercontent.com/sablearjun/65fbf069af759ca865ef2da001d18765/raw/ecbd40e6e0fc56e0cb95dc96fdcf94e6cd5188d7/enrollment_secret
sudo curl -o /etc/osquery/osquery.flags https://gist.githubusercontent.com/sablearjun/65fbf069af759ca865ef2da001d18765/raw/ecbd40e6e0fc56e0cb95dc96fdcf94e6cd5188d7/osquery.flags

# OSQuery run script
sudo curl -o /etc/osquery/osq.sh https://gist.githubusercontent.com/sablearjun/65fbf069af759ca865ef2da001d18765/raw/4aa79d0c4785ee6a2e859588701b8aa7e7cdefc4/osq.sh
sudo chmod +x /etc/osquery/osq.sh

# On restart run OSQuery
sudo crontab -l > file; 
sudo echo "@reboot sudo /etc/osquery/osq.sh" >> file;
sudo crontab file;
rm file

echo " ------------- System setup successfull ------------- "