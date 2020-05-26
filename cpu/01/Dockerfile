From daocloud.io/library/ubuntu:18.04
USER root
COPY ./sources.list /etc/apt/sources.list
RUN apt update && apt install gcc  -y && apt install g++  -y && apt install cmake -y && apt install zlib1g-dev -y

EXPOSE 4000

WORKDIR /app

COPY *.* /app/
RUN cd /app && tar -xvf libevent-2.1.8-stable.tar.gz && cd libevent-2.1.8-stable && ./configure && make -j4 && make install
RUN echo include /usr/local/lib >> /etc/ld.so.conf
RUN ldconfig
RUN cd /app && cmake . && make -j4
CMD ["/app/bin/perfromTest"]

