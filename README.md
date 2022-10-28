# ZeroJVM
This is a CS263 course project repository with implementation of simple JVM designed to be fit to Flipper Zero device.

## Preparation
1. `git submodule update --init --recursive`
2. `cd unleashed-firmware && ./fbt && cd ..`
3. `make clean`
4. `make all`

## Additional hints
### Libs that you might find useful to install
1. gcc-arm-none-eabi
2. libstdc++-arm-none-eabi-newlib

## TODO list:
1. Make sure code compiles on target VM's and devices
2. Add head files for local debug and device testing
3. Explore mini_jvm and other analogs, make comparison list
4. Read trought Java documentation at least two chapters per week.
