# CSViewer
A small ncurses tool to view CSV files in a terminal.

# Build instructions
## Debian/Ubuntu
```
sudo apt install build-essential cmake libncurses5-dev
mkdir bin; cd bin
cmake ..
make
```

# Installation instructions
From inside the `bin` folder, run
```
sudo make install
```
`csviewer` will be installed to /usr/local/bin.
