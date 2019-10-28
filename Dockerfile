FROM ubuntu:18.04



#DangerZone
########################################################
# Essential packages for remote debugging and login in
########################################################

RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    apt-utils gcc g++ openssh-server cmake build-essential gdb gdbserver rsync vim

RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

# 22 for ssh server. 7777 for gdb server.
EXPOSE 22 7777

RUN useradd -ms /bin/bash debugger
RUN echo 'debugger:pwd' | chpasswd

########################################################
# Add custom packages and development environment here
########################################################
#/DangerZone


RUN apt update
RUN apt install -y build-essential wget cmake m4 libgmp-dev file


RUN cd ~ && \
    wget https://www.shoup.net/ntl/ntl-11.3.2.tar.gz && \
    tar xf ntl-11.3.2.tar.gz && \
    cd ntl-11.3.2/src && \
    ./configure SHARED=on NTL_GMP_LIP=on NTL_THREADS=on NTL_THREAD_BOOST=on && \
    make -j4 && \
    make install

#COPY ./ /root/HElib/
RUN apt install -y git
RUN cd ~ && git clone https://github.com/homenc/HElib.git

RUN cd ~ && \
    mkdir -p HElib/build && \
    cd HElib/build && \
    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED=ON -DENABLE_THREADS=ON .. && \
    make -j4 && \
    make install && \
    ldconfig

########################################################
# DangerZone
########################################################

CMD ["/usr/sbin/sshd", "-D"]

RUN apt-get update &&\
    apt-get install python3.6  # &&\
    apt-get install python3-tk

#/DangerZone
