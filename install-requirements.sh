#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "[!] Please run as root"
  exit
fi

apt -y update
apt -y install git python3 python3-distutils uuid-dev build-essential bison flex nasm mtools acpica-tools
apt -y install gcc-10 g++-10