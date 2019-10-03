# fanctrl

fanctrl is a utilty for the Raspberry Pi 4B to control a PWM fan's speed based on the current CPU temperature.

## Usage
By default, fanctrl uses GPIO pin 18 to set the speed of the fan. It turns the fan off entirely as long as the CPU temperature is below 60 degrees Celsius and linearly increases it until the CPU temperature reaches 80 degrees Celsius, at which point the fan will run at full power. The fan speed is adjusted every 5 seconds. These defaults can be changed by modifying the values in fanctrl.cpp.

To start, run fanctrl as root. For the first 5 seconds the fan will run at full speed so that you can ensure it is working properly. fanctrl also provides a systemd service to make it easier to use (see the installation section further down).

## Requirements
- WiringPi 2.52 or newer (see "Updating WiringPi" below)
- CMake 3.7 or newer
- Working C++ compiler that supports C++11

## Installing fanctrl
Use
```
  cmake -DCMAKE_BUILD_TYPE=Release -Bbuild -H.
  cmake --build build
  sudo cmake --build build --target install
```

to install fanctrl and the systemd service. If you want to start fanctrl automatically during boot, additionally call
```
  systemctl enable fanctrl
```
and to start it immediately use
```
  systemctl start fanctrl
```

If you don't want to install the systemd service for some reason, pass `-DINSTALL_SYSTEMD_SERVICE=OFF` to the first cmake invocation.

## Updating WiringPi
As of right now (Oct 3rd, 2019) raspbian does not ship WiringPi 2.52 yet. To update, use

```
  wget https://project-downloads.drogon.net/wiringpi-latest.deb
  sudo dpkg -i wiringpi-latest.deb
```
