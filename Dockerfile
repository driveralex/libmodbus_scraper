FROM debian:10 AS builder

RUN useradd -ms /bin/bash driveralex
USER root
WORKDIR /libmodbus_project

RUN apt-get update && \
    apt-get -y install gcc pkg-config libsqlite3-dev sqlite3 make sudo && \
    rm -rf /var/lib/apt/lists/*

COPY ./libmodbus-3.1.6.tar.gz /libmodbus_project
RUN tar -zxf libmodbus-3.1.6.tar.gz
RUN cd ./libmodbus-3.1.6/ && \
    ./configure --prefix=/usr --disable-dependency-tracking && make && sudo make install
COPY ./src/* ./libmodbus-3.1.6/tests/
#RUN ls
#RUN cd ./libmodbus-3.1.6/tests/ &&\
#    ls
EXPOSE 502:502

RUN gcc ./libmodbus-3.1.6/tests/driveralexsoft.c -lsqlite3 -Wall -o driveralexsoft $(pkg-config --libs --cflags libmodbus)

FROM debian:10
USER root

#RUN apk update && apk add useradd sqlite3 libmodbus;

RUN apt-get update && \
    apt-get -y install sqlite3 libmodbus5 && \
    rm -rf /var/lib/apt/lists/*

RUN useradd -ms /bin/bash driveralex
USER driveralex
WORKDIR /home/driveralex
COPY --from=builder /libmodbus_project/driveralexsoft .
RUN ./driveralexsoft
