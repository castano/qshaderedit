## Windows ##

First, download the GLEW binaries from:

http://glew.sourceforge.net/

and unzip them at:

`qshaderedit/src/glew/...`

### MinGW ###

**TODO**

## Linux ##

### Debian ###

First make sure your system has all the required packages installed:

`# apt-get install libqt4-gui libglew-dev cmake nvidia-cg-toolkit`

Then enter the source folder and:

`$ ./configure && make`

If everything succeeds, now you should have the binary at `./build/src/qshaderedit`, and can also install it as root:

`# make install`


## OSX ##

**TODO**




