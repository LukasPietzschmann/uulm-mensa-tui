# UULM Mensa TUI
A terminal app implemented with C++ for hungry students at Ulm University

This app uses the unofficial API from [Kai Anter](https://github.com/Tanikai/).
If you don't value unoccupied RAM and insist of using a Web-App for literally everything, you can also use [Kai's webpage](https://mensaplan.anter.dev/).

![Screenshot](https://github.com/LukasPietzschmann/uulm-mensa-tui/assets/49213919/818d5e0b-b29d-4fb7-87d3-c190ab47d1b8)


## Commandline Flags
- `-f`/`--filter`: You can specify _meal attributes_ to filter for. If you're, for
  example, vegan, you can specify `-f Vegan` to only get vegan food. Possible options
  are: Vegan, Vegetarisch, Bio, Schwein, Rind.

## Build and install
To build and install you need a working version of CMake >= 3.20 and a compiler that can
build at least C++ 20 code.

If you meet those those requirements, you can follow these steps:
1. Clone the project: `git clone https://github.com/LukasPietzschmann/uulm-mensa-tui.git`
2. Navigate inside the project: `cd uulm-mensa-tui`
3. Create a build folder and cd into it: `mkdir build ; cd build`
4. Setup the build environment: `cmake ..`
5. Build it: `sudo cmake --build . --target install`
