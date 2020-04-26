FROM ubuntu
RUN apt-get -y update
RUN apt-get -y install apt-utils freeglut3-dev mesa-utils
RUN sudo apt-get install libgtest-dev &&
    cd /usr/src/gtest &&
    sudo cmake CMakeLists.txt &&
    sudo make &&
    sudo cp *.a /usr/lib