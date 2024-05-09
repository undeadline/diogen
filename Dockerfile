FROM debian:trixie-slim

ENV DEBIAN_FRONTEND noninteractive

# Update OS packages
RUN apt-get update -y && apt-get install -y \
    build-essential

# Don't show git message about not existing parent branch
# RUN git config --global advice.detachedHead false

RUN mkdir -p /tmp/diogen

WORKDIR /tmp/diogen

RUN useradd -G www-data,root -u 1000 -d /home/sammy sammy
RUN mkdir -p /home/sammy \
    && chown -R sammy:sammy /home/sammy