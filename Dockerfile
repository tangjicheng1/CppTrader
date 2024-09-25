FROM rockylinux:8.9 AS base

ENV TZ=Asia/Tokyo

RUN dnf install -y gcc-toolset-13 && dnf clean all
RUN source /opt/rh/gcc-toolset-13/enable

RUN dnf install -y git wget vim cmake make libuuid-devel && dnf clean all

ENV PATH /opt/rh/gcc-toolset-13/root/usr/bin:$PATH
ENV LD_LIBRARY_PATH /opt/rh/gcc-toolset-13/root/usr/lib64:$LD_LIBRARY_PATH
ENV MANPATH /opt/rh/gcc-toolset-13/root/usr/share/man:$MANPATH

WORKDIR /app
COPY . .
RUN rm -rf ./build && mkdir -p build && cd build && cmake .. && make -j6
CMD [ "/bin/bash" ]