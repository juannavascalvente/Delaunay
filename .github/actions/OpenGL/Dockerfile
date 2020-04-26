FROM ubuntu
RUN apt-get -y update
RUN apt-get -y install apt-utils freeglut3-dev mesa-utils
RUN sudo apt-get install libgtest-dev
RUN cd /usr/src/gtest
RUN sudo cmake CMakeLists.txt
RUN sudo make
RUN sudo cp *.a /usr/lib