FROM ubuntu

ENV DEBIAN_FRONTEND=noninteractive
ENV PACKAGES_ROOT=/packages

RUN apt-get update && apt-get -y install vim-nox build-essential git cmake cmake-curses-gui nano
RUN git config --global http.sslverify false

RUN git clone --branch v1.2.1 https://github.com/eclipse/paho.mqtt.c.git $PACKAGES_ROOT/paho.mqtt.c \
    && mkdir -p $PACKAGES_ROOT/paho.mqtt.c-build \
    && cd $PACKAGES_ROOT/paho.mqtt.c-build \
    && cmake -DCMAKE_BUILD_TYPE:STRING=PACKAGES_VERSION -DPAHO_ENABLE_TESTING:BOOL=FALSE -DPAHO_BUILD_STATIC:BOOL=TRUE ../paho.mqtt.c/ && make -j4 && make install \
    && rm -rf $PACKAGES_ROOT/paho.mqtt.c-build

WORKDIR /project
CMD [ "/bin/bash" ]