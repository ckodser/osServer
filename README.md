# osServer Project


## Compiling Project
Use `make`

+ `sudo make install` to add my_server to your $PATH. after compiling, you can run server with command `my_server`.
+ `sudo make TYPE=Normal/multiThread/multiProc PLATFORM=unix/windows` compiles code for given type and platform.
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
+ lines starting with `#` will be ignored

