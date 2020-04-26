FROM ubuntu
RUN apt-get -y update
RUN apt-get -y install apt-utils freeglut3-dev mesa-utils install libgtest-dev
RUN cd /usr/src/gtest
RUN cmake CMakeLists.txt
RUN make
RUN cp *.a /usr/lib