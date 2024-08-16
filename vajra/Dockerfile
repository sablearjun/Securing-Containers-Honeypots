FROM ubuntu:latest

RUN apt update && apt install gcc wget curl libcurl4-openssl-dev cmake build-essential libssl-dev libjsoncpp-dev	libboost-all-dev libpqxx-dev libssl-dev libprocps-dev -y

WORKDIR /app

COPY . .

RUN mkdir -p cmake-build-debug
	
RUN mkdir -p /var/log/vajra
	
WORKDIR /app/cmake-build-debug

RUN curl -o ./config.json https://gist.githubusercontent.com/kni9ht/581883cf1eb5020edd8818b0666a6f67/raw/config.json

RUN cmake .. && make