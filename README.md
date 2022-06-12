# osServer Project


## Compiling Project
Use `make`

+ `sudo make install` to add my_server to your $PATH. after compiling, you can run server with command `my_server`.
+ `sudo make TYPE=Normal/multiThread/multiProc PLATFORM=unix/windows` compiles code for given type and platform.
+ `sudo make clean` removes executable files.


Default platform is unix and default time is Normal (e.g. `make` creates executable file with normal type on unix).
