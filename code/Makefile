source_code="simpleWebServer.c"
exec_file="my_server"
path_to_exec_file="bin/$(exec_file)"
NEW_PATH := $(shell pwd)/bin:$
SHELL := /bin/bash

ifndef PLATFORM
	TYPE="Normal"
endif

ifndef PLATFORM
	PLATFORM="unix"
endif

all:
	@if [ $(TYPE) = "multiThread" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc -o $(path_to_exec_file) -DMULTITHREAD -DWINDOWS $(source_code); \
	elif [ $(TYPE) = "multiProcess" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc -o $(path_to_exec_file) -DMULTIPROC -DWINDOWS $(source_code); \
	elif [ $(TYPE) = "Normal" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc -o $(path_to_exec_file) -DNORMAL -DWINDOWS $(source_code); \
	elif [ $(TYPE) = "multiThread" ] ; then \
		gcc -o $(path_to_exec_file) -DMULTITHREAD -DUNIX -pthread $(source_code); \
	elif [ $(TYPE) = "multiProcess" ] ; then \
		gcc -o $(path_to_exec_file) -DMULTIPROC -DUNIX $(source_code); \
	else \
		gcc -o $(path_to_exec_file) -DNORMAL -DUNIX $(source_code); \
	fi
	@echo "Server will work $(TYPE) on $(PLATFORM)";

install:
	@echo Installing ...;
	@mkdir -p ./bin;
	@echo Add to PATH;
	@echo export PATH="$(NEW_PATH)"PATH >> ~/.bashrc;
	@source ~/.bashrc;
	@echo Installed;

clean:
	@rm -f $(path_to_exec_file);


