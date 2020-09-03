FROM debian:10

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
#RUN cd ./libmodbus-3.1.6/tests/ &&\
#    ls

RUN cd /tmp && \
    touch imafile && \
    ls -a

#CMD [ "./driveralexsoft" ]


