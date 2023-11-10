FROM debian:stable-slim

ARG PHP_VERSION="8.2.8" \
    M4_VERSION="1.4.19" \
    AUTOCONF_VERSION="2.71" \
    AUTOMAKE_VERSION="1.16.5" \
    LIBTOOL_VERSION="2.4.7" \
    RE2C_VERSION="3.1" \
    BISON_VERSION="3.8" \
    PKG_CONFIG_VERSION="0.29.2" \
    CLI_CONFIGURE_OPTIONS=" \
        --disable-cgi \
        --enable-zts \
        --disable-short-tags \
        --with-openssl \
        --with-kerberos \
        --with-system-ciphers \
        --without-sqlite3 \
        --with-zlib \
        --enable-bcmath \
        --enable-calendar \
        --with-curl \
        --enable-exif \ 
        --with-ffi \
        --enable-gd \
        --with-gmp \
        --enable-intl \
        --enable-mbstring \
        --enable-pcntl \
        --without-pdo-sqlite \
        --with-pdo-pgsql \
        --with-pgsql \
        --enable-shmop \
        --enable-soap \
        --enable-sockets \
        --with-sodium \
        --with-password-argon2 \
        --with-xsl \
        --with-zip"

ENV DEBIAN_FRONTEND noninteractive

# Update OS packages
RUN apt-get update -y && apt-get install -y \
    build-essential \
    git \
    curl \
    python3 \
    libxml2 \
    libssl-dev \
    libkrb5-dev \
    zlib1g-dev \
    libcurl4-gnutls-dev \
    libffi-dev \
    libpng-dev \
    libgmp-dev \
    libonig-dev \
    libpq-dev \
    libsodium-dev \
    libargon2-dev \
    libxslt-dev \
    libzip-dev

# Don't show git message about not existing parent branch
RUN git config --global advice.detachedHead false

# We need to create a folders for sources files
RUN mkdir -p /tmp/m4 /tmp/autoconf /tmp/automake /tmp/libtool /tmp/re2c /tmp/bison /tmp/php /opt/php /var/www /tmp/pkg-config

# We install m4, because autoconf depends from it
WORKDIR /tmp/m4
RUN curl https://ftp.gnu.org/gnu/m4/m4-${M4_VERSION}.tar.gz --output m4.tar.gz
RUN tar -xzf m4.tar.gz 
RUN cd m4-${M4_VERSION} && ./configure && make && make install

# We install autoconf 2.64+ version for compile php >= 7.2.0
WORKDIR /tmp/autoconf
RUN curl https://ftp.gnu.org/gnu/autoconf/autoconf-${AUTOCONF_VERSION}.tar.gz --output autoconf.tar.gz
RUN tar -xzf autoconf.tar.gz 
RUN cd autoconf-${AUTOCONF_VERSION} && ./configure && make && make install

# We install automake 1.4+ version
WORKDIR /tmp/automake
RUN curl https://ftp.gnu.org/gnu/automake/automake-${AUTOMAKE_VERSION}.tar.gz --output automake.tar.gz
RUN tar -xzf automake.tar.gz 
RUN cd automake-${AUTOMAKE_VERSION} && ./configure && make && make install

# We install libtool 1.4.x+ (except 1.4.2) version
WORKDIR /tmp/libtool
RUN curl https://ftp.gnu.org/gnu/libtool/libtool-${LIBTOOL_VERSION}.tar.gz --output libtool.tar.gz
RUN tar -xzf libtool.tar.gz 
RUN cd libtool-${LIBTOOL_VERSION} && ./configure && make && make install

# We install re2c 0.13.4+ version
WORKDIR /tmp/re2c
RUN git clone --depth=1 --branch=${RE2C_VERSION} https://github.com/skvadrik/re2c.git .
RUN autoreconf -i -W all && ./configure && make && make install

# We install bison > 3.0 version
WORKDIR /tmp/bison
RUN curl https://ftp.gnu.org/gnu/bison/bison-${BISON_VERSION}.tar.xz --output bison.tar.xz
RUN tar -xf bison.tar.xz
RUN cd bison-${BISON_VERSION} && ./configure && make && make install

# We install pkg-config
WORKDIR /tmp/pkg-config
RUN git clone --depth=1 --branch=pkg-config-${PKG_CONFIG_VERSION} https://gitlab.freedesktop.org/pkg-config/pkg-config.git .
RUN autoreconf -i -W all && ./configure --with-internal-glib && make && make install && PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig

# We clone a php repository}
WORKDIR /tmp/php
RUN git clone --depth=1 --branch=php-${PHP_VERSION} https://github.com/php/php-src.git .

# We configure php with prefix location and run make, make install
# RUN ./buildconf --force \
#     && ./configure --prefix=/opt/php ${CLI_CONFIGURE_OPTIONS} \
#     && make \
#     && make install \
#     && ln -s /opt/php/bin/php /usr/bin

# RUN useradd -G www-data,root -u 1000 -d /home/sammy sammy
# RUN mkdir -p /home/sammy \
#     && chown -R sammy:sammy /home/sammy \
#     && chown -R sammy:sammy /var/www