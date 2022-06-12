source_code="simpleWebServer.c"
exec_file="my_server"

ifndef PLATFORM
	TYPE="Normal"
endif

ifndef PLATFORM
	PLATFORM="unix"
endif

all:
	@if [ $(TYPE) = "multiThread" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc -o $(exec_file) -DMULTITHREAD -DWINDOWS $(source_code); \
	elif [ $(TYPE) = "multiProcess" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc -o $(exec_file) -DMULTIPROC -DWINDOWS $(source_code); \
	elif [ $(TYPE) = "Normal" ] && [ $(PLATFORM) = "windows" ] ; then \
		gcc -o $(exec_file) -DNORMAL -DWINDOWS $(source_code); \
	elif [ $(TYPE) = "multiThread" ] ; then \
		gcc -o $(exec_file) -DMULTITHREAD -DUNIX -pthread $(source_code); \
	elif [ $(TYPE) = "multiProcess" ] ; then \
		gcc -o $(exec_file) -DMULTIPROC -DUNIX $(source_code); \
	else \
		gcc -o $(exec_file) -DNORMAL -DUNIX $(source_code); \
	fi
	@echo "Server will work $(TYPE) on $(PLATFORM)";
clean:
	@rm -f $(exec_file)


