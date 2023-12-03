# TwinsmoothSearcher

Required libraries:
```bash
sudo apt-get install g++ #or clang
sudo apt-get install cmake
sudo apt-get install libgmp-dev
```
Installation + compilation:
```bash
git clone https://github.com/GiacomoBruno/TwinsmoothSearcher.git
cd TwinsmoothSearcher
mkdir build
cd build
cmake ..
make
# to run then use: 
./twinsmooth_searcher
```

Configuration can be set through a file before running the program or in the program using command line interface.
To use the file, simply copy the config.conf file from the root folder of the repo to the folder containing the program (e.g. from TwinsmoothSearcher/config.conf to TwinsmoothSearcher/build/config.conf)

You can modify the config.conf file however you like and each parameter of the config is documented in the config file itself.
