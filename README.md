![Demo](https://raw.githubusercontent.com/prodemmi/twiq/main/assets/demo.gif)


# Twiq

**Twiq** is a lightweight C++/Qt application designed to create and save beautiful loading spinner animations with ease.


## Features

- Design modern, customizable multi-spinner animations
- Export animations as GIFs using GIFLIB
- Clean and intuitive UI built with Qt6


## Built With

Twiq leverages several powerful C++ libraries:

- [Qt6](https://www.qt.io/product/qt6) – A modern, cross-platform application development framework.
- [Blend2D](https://blend2d.com/) – A high-performance 2D vector graphics engine.
- [GIFLIB](https://giflib.sourceforge.net/gif_lib.html) – A library for encoding and decoding GIF images.

# Build

```bash
mkdir build && cd build
cmake build ..
make

// run twiq
./twiq
or
twiq.exe
```