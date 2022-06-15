# osServer Project

## How to use system
+ clone the project.
+ go to code/ directory.
+ run `sudo make install` (on unix).
+ in order to compile the project see the related [section](##compile-source-code).
+ exectuable file is now located on bin/ directory.
+ you can run executable file directly or just by command `my_server` in terminal (unix only).
  * when server is up, it uses `\etc\simpleWebServer\server.conf` as config file. see the related [section](##config_file). 

## Compile source code
Use `make` in `code/` directory. **Makefile** configuration:

+ `sudo make install`
  * to add my_server to your $PATH. after compiling, you can run server with command `my_server` (unix only).
  * to add config file to `/etc/simpleWebServer/server.cong`
  * to add the address of content files to `server.conf`
+ `sudo make TYPE=Normal/multiThread/multiProc PLATFORM=unix/windows` compiles code for given type and platform.
  * for example, `sudo make TYPE=multiThread PLATFORM=unix` builds source code to handle requests with multi threads on unix.
+ `sudo make clean` removes executable files.

default platform is unix and default time is Normal (e.g. `make` creates executable file with normal type on unix).

## Config File

to config server use `server.conf` (before installation).

you can also reinstall after changing `server.conf`

config file content:
+ `PROTNUM` indicates port number 
+ `BUFSIZE` indicates buffer size that store incoming requests. 
+ `StartingPageName` indicates address of first page of website. 
+ `LogFile` indicates address of log file that store all logs. 
+ `ErrorHTML` indicates address of a page that will be showed if requested page doesn't exists. 
+ `MAXPROCORTHREAD` indicates maximum number of additional threads or procces.
+ lines starting with `#` will be ignored.

## About system
This project, simply runs a web server (http). When a request (i.e. http request) is received, based on the settings, this server creates a new process or a new thread to handle the request. when a request is handled, corresponding thread and process is terminated. All the messages in connections and the log of requests are collected in each thread/ process, and is sent to the main thread/ process. Finally, when server is terminated, all those logs will be stored in log file.

At this point, this projects runs well on **Unix** in all Normal, multiThread, and multiProcess settings.
On **Windows**, it works in Normal, and multiThread (without image) settings.

You can see `simpleWebServer.c` in code/ directory for more details. the code has comments which describe the bahavior of functions.
