### Features ###

QShaderEdit allows you to edit shaders and tweak shader parameters while previewing the results in real-time.

Currently it supports the following shading languages:

  * ARB programs.
  * GLSL shaders.
  * CgFX effects.

### Installation ###

QShaderEdit uses the cmake build system, but also comes with standard configure and make scripts. To build and install it, just run:

`$ ./configure && make && make install`

On non unix platforms you will have to use cmake directly. For more details see the detailed BuildInstructions.

### Requirements ###

You will need at least:

  * [Qt 4.2](http://trolltech.com/developer/downloads/qt)
  * [GLEW 1.3](http://glew.sourceforge.net)
  * [cmake 2.4](http://www.cmake.org)

Optionally, if the [Cg Toolkit](http://developer.nvidia.com/object/cg_toolkit.html) 1.4 or above is found in the system, support for CgFx effects will be available.

### Screenshots ###

![![](http://qshaderedit.googlecode.com/svn/wiki/julia_small.png)](http://qshaderedit.googlecode.com/svn/wiki/julia.png)
![![](http://qshaderedit.googlecode.com/svn/wiki/raytracer_small.png)](http://qshaderedit.googlecode.com/svn/wiki/raytracer.png)
![![](http://qshaderedit.googlecode.com/svn/wiki/relief_small.png)](http://qshaderedit.googlecode.com/svn/wiki/relief.png)

