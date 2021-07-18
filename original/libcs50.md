# libcs50
[CS50's library](https://cs50.readthedocs.io/libraries/cs50/c) is required to compile the original code.

Make sure to have it installed following the instructions below.

### Ubuntu
``` bash
$ curl -s https://packagecloud.io/install/repositories/cs50/repo/script.deb.sh | sudo bash
$ sudo apt install libcs50
```

### Fedora
``` bash
$ curl -s https://packagecloud.io/install/repositories/cs50/repo/script.rpm.sh | sudo bash
$ dnf install libcs50
```

### Arch Linux
The library is available in the AUR:
``` bash
$ yay libcs50
```
### From Source (Linux and Mac)

1. Download the latest release from https://github.com/cs50/libcs50/releases
1. Extract `libcs50-*.*`
1. `cd libcs50-*`
1. `sudo make install`

By default, it is installed to `/usr/local`. If you'd like to change the installation location, run
`sudo DESTDIR=/path/to/install make install` as desired.
