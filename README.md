# Segments Hardware


This is the repo of the [Segments](https://github.com/AandersonL/Segments-Android) hardware, it uses an ESP32 to make all bluetooth communications.

The third party code was written by [nkolban](https://github.com/nkolban/esp32-snippets), without that this project would probably never be finished.

See the [video](https://streamable.com/7bslw) that shows how this works in practice.

### Setup
Requirements:

- 1 x Esp32

- 1 x MicroSD adapter

- wires

Setup your SD card using the pinnage schema bellow:

![](https://camo.githubusercontent.com/fe6b89251ae4df2628b1a4c86c57976f22d6d5ba/687474703a2f2f692e696d6775722e636f6d2f34436f584f75522e706e67)


For more help, visit the official [SD](https://github.com/espressif/arduino-esp32/tree/master/libraries/SD) page.



## Compiling

In order to compile, you will need the [ESP-IDF](https://esp-idf.readthedocs.io/en/latest/) to setup the compiler, linker, debugger and other useful tools provided by espressif.

after setting up everything(compiler, env variables), just type:

```
make
```
and
```
make flash monitor
```

This will compile & link the source, then flash into ESP32, read more in the [docs](https://esp-idf.readthedocs.io/en/latest/), everything you need to know is there

If you wanna change something in the compilation/linker proccess, just hit

```
make monitor
```
And configure your own make proccess, there's also a partition table (partition.csv), if you need to reallocate something in the partition, modify there(be careful).

See the [Segments App](https://github.com/AandersonL/Segments-Android).
