# osServer Project

## How to use system
+ clone the project.
+ go to code/ directory.
+ run `sudo make install` (on unix).
+ in order to compile the project see the related [section](##compile-source-code).
+ exectuable file is now located on bin/ directory.
+ you can run executable file directly or just by command `my_server` in terminal (unix only).

## Compile source code
Use `make`

Makefile configuration:

+ `sudo make install` to add my_server to your $PATH. after compiling, you can run server with command `my_server` (unix only).
+ `sudo make TYPE=Normal/multiThread/multiProc PLATFORM=unix/windows` compiles code for given type and platform.
  * for example, `sudo make TYPE=multiThread PLATFORM=unix` builds source code to handle requests with multi threads on unix.
+ `sudo make clean` removes executable files.

Default platform is unix and default time is Normal (e.g. `make` creates executable file with normal type on unix).

## Config File

to config server use server.conf 

+ `PROTNUM` indicate port number 
+ `BUFSIZE` indicate buffer size that store incoming requests. 
+ `StartingPageName` indicate address of first page of website. 
+ `LogFile` indicate address of log file that store all logs. 
+ `ErrorHTML` indicate address of a page that will be showed if requested page doesn't exists. 
+ `MAXPROCORTHREAD` indicate maximum number of additional threads or procces.
+ lines starting with `#` will be ignored.

