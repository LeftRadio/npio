### Introduction

This is a NanoPi boards based GPIO manipulation package written on C with Python3 wrapper with some improvements and installable from PyPI package. In this package tryed an attempt to maximize the preservation of API compatibility with original package - WiringNP.


### APT
#### Common requirements
    sudo apt update
    sudo apt install python3-dev

### PyPI
#### Installation
    sudo pip3 install npio

### GIT
#### Download
    git clone https://github.com/leftradio/npio

#### Installation
    cd npio
    python3 setup.py buils sdist bdist
    sudo python3 setup.py install

    (For correct work after installing copy yours board 'sys_info' to '/etc/sys_info' path)
