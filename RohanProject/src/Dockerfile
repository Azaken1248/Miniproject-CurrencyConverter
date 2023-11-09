FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y mingw-w64 g++-mingw-w64 curl libcurl4-openssl-dev

WORKDIR /usr/src/app
COPY . .

CMD ["x86_64-w64-mingw32-g++", "CurrencyConverter.cpp", "-o", "CurrencyConverter.exe", "-lcurl", "-ljsoncpp"]

