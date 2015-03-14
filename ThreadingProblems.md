#summary Threading problems in QShaderEdit

# Introduction #

Shader compilation usually takes hundreds of milliseconds. To enable smooth shader editing, while the shaders are being compiled, QShaderEdit uses a separate thread that compiles the shaders in the background. However, this causes several problems.


# Threading problems under X11 #

One of the problem is that Xlib is not inherently thread safe. A simple workaround is to call `XInitThreads()` at the beginning of the application before any other Xlib call is made.

```
#if defined(Q_WS_X11)
	XInitThreads();
#endif
```

However, in my experience this generally causes lockups in the X server, even when the application is single threaded. This is a typical stack trace:

```
(gdb) info threads
* 1 Thread 0xb64626e0 (LWP 3805)  0xffffe410 in ?? ()
(gdb) bt
#0  0xffffe410 in ?? ()
#1  0xbfc21d98 in ?? ()
#2  0x00000002 in ?? ()
#3  0xb6f6de1e in __lll_mutex_lock_wait () from /lib/i686/cmov/libpthread.so.0
#4  0xb6f69b68 in _L_mutex_lock_86 () from /lib/i686/cmov/libpthread.so.0
#5  0xb6f6958d in pthread_mutex_lock () from /lib/i686/cmov/libpthread.so.0
```

If the call to `XInitThreads()` is omitted everything works fine, but if multiple threads are used, Xlib produces the following errors:

```
Xlib: unexpected async reply (sequence 0x214b)!
Xlib: sequence lost (0x10000 > 0x4011) in reply type 0x0!
X Error: 0 0
  Extension:    245 (Uknown extension)
  Minor opcode: 223 (Unknown request)
  Resource id:  0x0
```


# OpenGL problems and Qt #

Another issue is that OpenGL is not thread safe either. So, OpenGL calls need to be protected by some other access mechanism, for example using a mutex. It's possible to guard every OpenGL call to guarantee serial processing. However, Qt also does some OpenGL calls in response to user events (for example when resizing windows).

In Mac OSX when using Objective-C there's a mechanism that allows you to lock the current context using `CGLLockContext` and `CGLUnlockContext`, so that two threads cannot use it at the same time. However, the same mechanism does not exist in Qt.

In theory it's possible to have multiple threads using separate contexts concurrently. However, when these two contexts share resources (using `wglShareLists()` for example) it's not very clear whether this is still valid. The [apple guidelines](http://developer.apple.com/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_threading/chapter_12_section_3.html) recommend that, and in practice, this solution seems to work fine as long as rendering is stopped while the worker thread creates the resources.

In some rare cases I've seen texture corruption, but I've been running the OSX OpenGL debugger with the "Break on thread error" option enabled, and no error was ever displayed, so I suspect it was a driver problem.

I need to investigate the issue in more detail, but I think it would be good to add a context locking mechanism to Qt, to make this problems easier to avoid.


# References #

http://tronche.com/gui/x/xlib/display/XInitThreads.html

http://doc.trolltech.com/qq/qq06-glimpsing.html

http://developer.apple.com/documentation/GraphicsImaging/Conceptual/OpenGL-MacProgGuide/opengl_threading/chapter_12_section_1.html