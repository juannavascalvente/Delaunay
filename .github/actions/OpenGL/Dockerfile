FROM ubuntu:latest
RUN apt-get -y update
RUN echo 8 | apt-get install -y tzdata
RUN apt-get install -y apt-utils cmake freeglut3-dev mesa-utils libgtest-dev googletest build-essential
RUN cmake --version
WORKDIR /usr/src/gtest
RUN cmake CMakeLists.txt
RUN make
RUN cp lib/*.a /usr/lib
RUN ls -l /usr/lib/*.a