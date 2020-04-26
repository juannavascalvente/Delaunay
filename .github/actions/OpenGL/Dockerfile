FROM ubuntu:latest
RUN apt-get -y update
RUN export TZ=Europe/Madrid
RUN apt-get install -y apt-utils cmake freeglut3-dev mesa-utils libgtest-dev googletest build-essential
RUN cmake --version
WORKDIR /usr/src/gtest
RUN pwd
RUN ls -l
RUN ls -l /usr/src/gtest
RUN cmake CMakeLists.txt
RUN make
RUN cp *.a /usr/lib