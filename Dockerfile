FROM debian:trixie-slim

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && apt-get install -y \
    build-essential

RUN mkdir -p /tmp/diogen

WORKDIR /tmp/diogen

RUN useradd -G www-data,root -u 1000 -d /home/sammy sammy
RUN mkdir -p /home/sammy \
    && chown -R sammy:sammy /home/sammy