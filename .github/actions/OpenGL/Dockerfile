FROM ubuntu
RUN apt-get -y update
RUN apt-get install -y apt-utils cmake freeglut3-dev mesa-utils libgtest-dev
RUN cd /usr/src/gtest
RUN cmake CMakeLists.txt
RUN make
RUN cp *.a /usr/lib