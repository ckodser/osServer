//Be nam _e_ khoda


//#define WINDOWS
//#define UNIX
//#define MULTITHREAD
//#define MULTIPROC
//#define NORMAL
#define DEBUG



#ifdef UNIX
#define CloseHandle close
#define INVALID_HANDLE_VALUE -1
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SOCKADDR struct sockaddr
#define closesocket close
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/sendfile.h>
#endif

#ifdef MULTITHREAD
#ifdef UNIX
#include <pthread.h>
#endif
#endif

#ifdef MULTIPROC
#ifdef UNIX
#include <stdarg.h>
#include <sys/wait.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#include <tchar.h>
#endif
#endif

#ifdef WINDOWS
#include<winsock2.h>
#include<Mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#include <windows.h>
#include <conio.h>
#include <process.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>

#define INNERBUFFSIZE 2000
#define LOGSIZE 5000
#define ALLLOGSSIZE 200000

#ifdef MULTITHREAD
#define MAXMAXPROCTHREAD 200
#endif
#ifdef MULTIPROC
#define MAXMAXPROCTHREAD 200
#endif
#ifdef NORMAL
#define MAXMAXPROCTHREAD 1
#endif

#ifdef UNIX
char config_file_address[]="/etc/simpleWebServer/server.conf";
#endif
#ifdef WINDOWS
char config_file_address[]="WindowsServer.conf";
#endif

#ifdef UNIX
int min(int a,int b){
	if(a<b)return a;
	return b;
}
#endif

int BUFSIZE;
int PORTNUM;
int MAXPROCTHREAD;
char content_location[INNERBUFFSIZE];
char LogFile[INNERBUFFSIZE];
char Error[INNERBUFFSIZE];
char all_logs[ALLLOGSSIZE];
char starting_page[INNERBUFFSIZE];

SOCKET fd[MAXMAXPROCTHREAD];
char* log_ret[MAXMAXPROCTHREAD];
int busy[MAXMAXPROCTHREAD];
int read_log_flag[MAXMAXPROCTHREAD];

#ifdef MULTITHREAD
#ifdef UNIX
pthread_t thread_pool[MAXMAXPROCTHREAD];
#endif
#endif

#ifdef MULTIPROC
int all_pipes[MAXMAXPROCTHREAD][2];
enum { P_READ, P_WRITE };   /* Read, write descriptor of a pipe */
#ifdef UNIX
pid_t all_pids[MAXMAXPROCTHREAD];
#endif
#ifdef WINDOWS

#endif
#endif

SOCKET fd_server;


// This function returns the absolute path of the location.
// In order to do that, it concats the content_location (i.e. absolute path to content folder) and the location.

char* relative_file_address(char *location){
	char *rel_location=malloc(strlen(location)+1+strlen(content_location));
	strcpy(rel_location, content_location);
	strcat(rel_location, location);
	return rel_location;
}


// This function reads the content of a webpage to send it the client.

char *read_webpage(char *webpage_location)
{
	FILE *f = fopen(webpage_location, "r");
	if (f == NULL)
	{
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *webpage = malloc(fsize + 1);
	fread(webpage, fsize, 1, f);
	fclose(f);
	webpage[fsize] = 0;
	return webpage;
}


// This function reads the config file and initialize variables such as content location, port number, log file, maximum thread/processes, etc.
// It reads from the /etc/simpleWebServer/server.conf

void read_config_file()
{
	FILE * config_file;
	char buff[INNERBUFFSIZE];
	config_file = fopen(config_file_address, "r");
	if (config_file == NULL)
	{
		printf("couldn't open config file\n");
		exit(1);
	}
	while (1)
	{
		memset(buff, 0, INNERBUFFSIZE);
		if (fgets(buff, 255, (FILE*) config_file) == 0)
		{
			break;
		}
		if (buff[0] == '#' || buff[0] == ' ' || buff[0] == '\n')
		{
			continue;
		}
		char line[2][INNERBUFFSIZE];
		int start = strstr(buff, " = ") - buff;
		buff[start] = '\0';
		strcpy(line[0], buff);
		strcpy(line[1], buff + start + 3);
		line[1][strlen(line[1]) - 1] = '\0';
		printf("%s = %s\n", line[0], line[1]);
		///////////////////
		if (strcmp(line[0], "PORTNUM") == 0)
		{
			sscanf(line[1], "%d", &PORTNUM);
		}
		else if (strcmp(line[0], "BUFSIZE") == 0)
		{
			sscanf(line[1], "%d", &BUFSIZE);
		}
		else if (strcmp(line[0], "MAXPROCORTHREAD") == 0)
		{
			sscanf(line[1], "%d", &MAXPROCTHREAD);
			MAXPROCTHREAD = min(MAXPROCTHREAD, MAXMAXPROCTHREAD);
		}
		else if (strcmp(line[0], "WebContentLocation") == 0)
		{
			strcpy(content_location, line[1]);
		}
		else if (strcmp(line[0], "StartingPageName") == 0)
		{
			strcpy(starting_page, line[1]);
		}
		else if (strcmp(line[0], "LogFile") == 0)
		{
			strcpy(LogFile, line[1]);
		}
		else if (strcmp(line[0], "ErrorHTML") == 0)
		{
			strcpy(Error, line[1]);
		}
		else
		{
			printf(" Couldn't find %s as a server parameter\n", line[0]);
		}
		//////////////////
	}
	fclose(config_file);
}


// This function reads the user's information (which is in the message of the client to server).

char *get_user_info(char *buf)
{
	char *start = strstr(buf, "User-Agent:") + 11;
	char *end = strstr(start, "\n");
	int length = (end - start);
	char *user_info = malloc(length + 1);
	memcpy(user_info, start, end - start);
	user_info[length] = '\0';
	return user_info;
}


// This function terminates the process/ thread which is the dedicated to the client to handle its requests.
// It closes the connection (i.e. socket).
// It asks the thread/process to send its log to the main thread/ process.

void handle_request_finished(int fd_ind)
{
	busy[fd_ind] = 0;
	#ifdef WINDOWS
	shutdown(fd[fd_ind], 1);
	#endif //WINDOWS
	#ifdef UNIX
	closesocket(fd[fd_ind]);
	#endif // UNIX
	
	#ifdef MULTIPROC
	#ifdef DEBUG
	printf("%s\n", log_ret[fd_ind]);
	#endif
	write(all_pipes[fd_ind][P_WRITE], log_ret[fd_ind], strlen(log_ret[fd_ind]) + 1);
	#endif
}


//This function handles the requests coming from the client and sends the appropriate html/ picture upon the request.
//At the end, it calls handle_request_finished.

void *handle_request(void *fd_ind_point)
{
	int fd_ind;
	fd_ind = *(int*) fd_ind_point;
	busy[fd_ind] = 1;
	read_log_flag[fd_ind] = 1;
	SOCKET fd_client = fd[fd_ind];
	char *logOfRequest = malloc(LOGSIZE);
	log_ret[fd_ind] = logOfRequest;

	char *buf = malloc(BUFSIZE);
	int fdimg;

	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	int size = 0;
	snprintf(logOfRequest + size, LOGSIZE, "Time = %s", asctime(timeinfo));
	size = strlen(logOfRequest);
	if (fd_client == INVALID_SOCKET)
	{
		snprintf(logOfRequest + size, LOGSIZE, "Connection Failed! Can't Connect to Client \n");
		size = strlen(logOfRequest);
		handle_request_finished(fd_ind);
		return (void*) 0;
	}
	snprintf(logOfRequest + size, LOGSIZE, "Client Connection Accepted\n");
	size = strlen(logOfRequest);
	memset(buf, 0, BUFSIZE);
	#ifdef UNIX
	read(fd_client, buf, BUFSIZE);
	#endif
	#ifdef WINDOWS
	recv(fd_client, buf, BUFSIZ, 0);
	#endif
	#ifdef DEBUG
	printf("BUF ******ind= %d  *******\n%s(***************)\n", fd_ind, buf);
	#endif

	if (strlen(buf) == 0)
	{
		snprintf(logOfRequest + size, LOGSIZE, "Request is empty!\n");
		size = strlen(logOfRequest);
		handle_request_finished(fd_ind);
		return (void*) 0;
	}

	char *user_info = get_user_info(buf);
	snprintf(logOfRequest + size, LOGSIZE, "User = %s\n", user_info);
	size = strlen(logOfRequest);
	char location[INNERBUFFSIZE];
	int start;
	int end;
	start = 5;
	end = strchr(buf + 5, ' ') - buf;
	memcpy(location, buf + start, end - start);
	location[end - start] = '\0';

	if (start == end)
	{
		strcpy(location, starting_page);
		start = 0;
		end = strlen(location);
	}
	#ifdef DEBUG
	printf("location ********%s, %s\n", location, relative_file_address(location));
	#endif

	snprintf(logOfRequest + size, LOGSIZE, "Request = %s\n", location);
	size = strlen(logOfRequest);
	snprintf(logOfRequest + size, LOGSIZE, "try to read file %s\n", location);
	size = strlen(logOfRequest);

	if (!strncmp(location + (end - start - 4), "html", 4))
	{
		char *webpage = read_webpage(relative_file_address(location));
		if (webpage == NULL)
		{
			snprintf(logOfRequest + size, LOGSIZE, "couldn't find html file\n");
			size = strlen(logOfRequest);
			webpage = read_webpage(relative_file_address(Error));
		}
		int n;
		n = strlen(webpage);
		#ifdef UNIX
		write(fd_client, webpage, n - 1);
		#endif
		#ifdef WINDOWS
		send(fd_client, webpage, n - 1, 0);
		#endif

	}
	else
	{
		char *address = relative_file_address(location);

		#ifdef UNIX
		fdimg = open(address, O_RDONLY);
		#endif
		#ifdef WINDOWS
		HANDLE fdimg;
		fdimg = CreateFileA(location, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		#endif

		if (fdimg == INVALID_HANDLE_VALUE)
		{
			snprintf(logOfRequest + size, LOGSIZE, "couldn't find/open file\n");
			size = strlen(logOfRequest);
		}
		else
		{
			#ifdef UNIX
			sendfile(fd_client, fdimg, NULL, 200000);
			#endif
			#ifdef WINDOWS
			DWORD dwNumToSend = 0;
			DWORD nNumberOfBytesPerSend = 0;
			//TransmitFile(fd_client, fdimg, dwNumToSend, nNumberOfBytesPerSend, NULL, NULL, 0);
			#endif
		}
		CloseHandle(fdimg);
	}
	snprintf(logOfRequest + size, LOGSIZE, "Client Connection Closed\n");
	size = strlen(logOfRequest);
	handle_request_finished(fd_ind);
	return (void*) 0;
}


//This function reads the log of the ind_th thread/process.

void read_log(int ind)
{
	if (read_log_flag[ind])
	{
		char *log = log_ret[ind];
		strcat(all_logs, "********************\n");
		strcat(all_logs, log);
		#ifdef DEBUG
		printf("$*$*$*$*%d$*$*$*$*$*$\n%s$*$*$*$*$*$*$*$*$*$\n", ind, log);
		#endif
		read_log_flag[ind] = 0;
	}
}

#ifdef MULTIPROC
char read_buffer[INNERBUFFSIZE];


//This function is necessary while serving with multi processes.
//It reads the log buffer of the running processes.

void check_all_processes()
{
	int i;
	for (i = 0; i < MAXPROCTHREAD; i += 1)
	{
		if (busy[i])
		{
			int len = read(all_pipes[i][P_READ], read_buffer, INNERBUFFSIZE);
			if (len > 0)
			{
				read_buffer[len] = 0;
				read_log_flag[i] = 1;
				log_ret[i] = malloc(INNERBUFFSIZE);
				strcpy(log_ret[i], read_buffer);
				memset(read_buffer, 0, sizeof(read_buffer));
				read_log(i);
				busy[i] = 0;
			}
		}
	}
}
#endif


//This function is the interupt (i.e. Ctrl+C) handler.
//It checks all the processes (when multi-processing is used) to gather all the log information.
//It eventually writes all the logs on the log file.

void IntHandler(int sig)
{
	#ifndef MULTIPROC
	int i;
	for (i = 0; i < MAXPROCTHREAD; i++)
	{
		read_log(i);
	}
	#endif
	#ifdef MULTIPROC
	check_all_processes();
	#endif
	printf("Interupt detected ...\n All Logs\n%s\n", all_logs);
	FILE * logfile;
	char *address = relative_file_address(LogFile);
	printf("\n save logs to: %s\n", address);
	logfile = fopen(address, "a");
	if(logfile == NULL)
	{
		printf("could not open logfile.\n saving logs failed.\n");
	}
	else
	{
		fprintf(logfile, "%s", all_logs);
	}
	closesocket(fd_server);
	exit(0);
}


//Main function.

int main(int argc, char *argv[])
{
	signal(SIGINT, IntHandler);
	read_config_file();

	#ifdef WINDOWS
	WSADATA wsa;
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Initialised.\n");
	#endif

	SOCKET fd_client;

	fd_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd_server == INVALID_SOCKET)
	{
		printf("socket failed with error\n");
		#ifdef WINDOWS
		WSACleanup();
		#endif
		return 1;
	}
	socklen_t sin_len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	sin_len = sizeof(client_addr);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;	// could be inet_addr("127.0.0.1");
	server_addr.sin_port = htons(PORTNUM);

	if (bind(fd_server, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		printf("bind failed with error");
		closesocket(fd_server);
		#ifdef WINDOWS
		WSACleanup();
		#endif
		return 1;
	}

	#ifdef WINDOWS
	int num = 1;
	#endif

	#ifdef UNIX
	int num = 10;
	#endif

	if (listen(fd_server, num) == SOCKET_ERROR)
	{
		printf("listen failed with error");
		closesocket(fd_server);
		#ifdef WINDOWS
		WSACleanup();
		#endif
		return 1;
	}
	#ifdef MULTITHREAD
	int free_thread;
	free_thread = 0;
	#endif
	#ifdef MULTIPROC
	int free_proc;
	free_proc = 0;
	#endif
	int ind;

	// Here when we accept a connection, based on the setting,
	// we dedicate a thread or a process to handle the client.

	while (1)
	{
		fd_client = accept(fd_server, (struct sockaddr *) &client_addr, &sin_len);

		#ifdef MULTITHREAD
		for (free_thread = (free_thread + 1) % MAXPROCTHREAD; busy[free_thread]; free_thread = (free_thread + 1) % MAXPROCTHREAD) {}
		ind = free_thread;
		#endif
		#ifdef NORMAL
		ind = 0;
		#endif
		#ifdef MULTIPROC
		for (; busy[free_proc]; free_proc = (free_proc + 1) % MAXPROCTHREAD)
		{
			check_all_processes();
		}
		ind = free_proc;
		#endif

		fd[ind] = fd_client;

		// Creates a new Thread

		#ifdef MULTITHREAD
		read_log(ind);
		#ifdef UNIX
		if (pthread_create(&thread_pool[ind], NULL, &handle_request, (void*) &ind)!=0)
		{
			printf("couldn't build thread\n");
			closesocket(fd[ind]);
			continue;
		}
		#endif
		#ifdef WINDOWS
		if(_beginthread(&handle_request, 0, (void*) &ind)==-1L)
		{
			printf("couldn't build thread\n");
			closesocket(fd[ind]);
			continue;
		}
		#endif
		#endif

		// This Thread is going to handle the request.

		#ifdef NORMAL
		handle_request((void*) &ind);
		read_log(ind);
		#endif

		// A new process is created by fork()

		#ifdef MULTIPROC
		if (pipe(all_pipes[ind]) != 0)
		{
			printf("couldn't pipe\n");
			return -1;
		}
		busy[ind] = 1;
		all_pids[ind] = fork();
		if (all_pids[ind] < 0)
		{
			printf("couldn't fork\n");
			return -1;
		}
		else if (all_pids[ind] == 0)
		{
			//child
			close(all_pipes[ind][P_READ]);
			handle_request((void*) &ind);
			exit(0);
		}
		else
		{
			//parent
			close(all_pipes[ind][P_WRITE]);
			closesocket(fd[ind]);
		}
		#endif
	}

	return 0;
}
