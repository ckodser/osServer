source_code="simpleWebServer.c"
exec_file="my_server"

path_to_exec_file="bin/$(exec_file)"
path_to_content_files_unix := $(shell pwd)/cont/
path_to_content_files_windows := $(shell pwd)/WindowsCont/
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
		gcc $(source_code) -o $(path_to_exec_file) -DMULTITHREAD -DWINDOWS -lws2_32; \
	elif [ $(TYPE) = "multiProcess" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc $(source_code) -o $(path_to_exec_file) -DMULTIPROC -DWINDOWS -lws2_32; \
	elif [ $(TYPE) = "Normal" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc $(source_code) -o $(path_to_exec_file) -DNORMAL -DWINDOWS -lws2_32; \
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
	@echo Add conf to /etc/simpleWebServer;
	@mkdir -p /etc/simpleWebServer/
	@rm -f /etc/simpleWebServer/*
	@cp -r server.conf WindowsServer.conf /etc/simpleWebServer
	@echo Add content address to conf
	@echo WebContentLocation = $(path_to_content_files_unix) >> /etc/simpleWebServer/server.conf
	@echo WebContentLocation = $(path_to_content_files_windows) >> /etc/simpleWebServer/WindowsServer.conf
	@echo Done
clean:
	@rm -f $(path_to_exec_file);


