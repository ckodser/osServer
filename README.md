# osServer Project


## Compiling Project
Use `make`

+ `make install` to add my_server to your $PATH. after compiling, you can run server with command `my_server`.
+ `make TYPE=Normal/multiThread/multiProc PLATFORM=unix/windows` compiles code for given type and platform.
+ `make clean` removes executable files.


Default platform is unix and default time is Normal (e.g. `make` creates executable file with normal type on unix).
