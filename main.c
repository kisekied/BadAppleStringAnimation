#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef PATH
#define PATH "badapple.txt"
#endif

#ifndef DELIM
#define DELIM "nekomark"
#endif

#ifndef TAIL_CUT//there is an extra "\r" or "\n" at the end of each frame, this param cuts certain number of characters off from each frame
#define TAIL_CUT 3
#endif

#define EQUALS(x, y) (strcmp(x, y) == 0)

int get_framesize(int fd);//declare the function after main to avoid warning

int main(int argc, char const *argv[])
{
	size_t delay = 50000;
	int fd;
	int framesize;
	int i;

	for(int i = 1; i < argc; i++) {
		if(EQUALS(argv[i], "-d"))
			delay = strtol(argv[++i], NULL, 0);
		else if(EQUALS(argv[i], "-h") || EQUALS(argv[i], "--help")) {
			printf(
			"badapple\n"
			"\n"
			"Usage:\tbadapple <-d delay in us> <-h,--help>\n"
			"\n"
			"-h,--help\tShow this help page\n"
			"\n"
			"-d\t\tSet delay between frames, default: 50000us\n"
			"\n"
			"Author: kisekied <https://github.com/kisekied>\n"
			);
			exit(EXIT_FAILURE);
		}
	}

	fd = open(PATH, O_RDONLY);
	if(fd < 0) {
		printf("error: open txt file failed\n");
		return EXIT_FAILURE;
	}

	framesize = get_framesize(fd);
	if(framesize < 0) {
		close(fd);
		return EXIT_FAILURE;
	}

	char* framebuf = malloc(framesize + 1);//leave a byte empty to recognize the end of string

	for(i = 0; i <= TAIL_CUT; i++)
		framebuf[framesize - i] = '\0';

	while(read(fd, framebuf, framesize - TAIL_CUT) > 0) {
		system("clear");
		puts(framebuf);
		lseek(fd, strlen(DELIM) + TAIL_CUT + 3, SEEK_CUR);
		usleep(delay);
	}

	free(framebuf);
	close(fd);
	return EXIT_SUCCESS;
}

/*
This function is for getting the size of a frame, not including "/nnekomark/r/n"
*/
int get_framesize(int fd)
{
	int pagesize = getpagesize();
	char* scanbuf = malloc(pagesize);
	off_t i;
	off_t last_lineend;
	off_t seek = 0;
	ssize_t bytes_get;

	if(pagesize < strlen(DELIM)) {
		printf("warning: one page is too small to buffer scan data\n");
		pagesize = strlen(DELIM);
	}

	bytes_get = read(fd, scanbuf, pagesize);
	if(bytes_get < 0) {
		printf("error: read txt file failed\n");
		return -1;
	}

	while(bytes_get) {
		last_lineend = -1;//if no "\n" is found in scanbuf, we need this value set instead of the value in last loop, set to -1 to increase speed in case pagesize = strlen(DELIM)

		if(strncmp(DELIM, scanbuf, strlen(DELIM)) == 0) {//if nekomark is at the beginning of the scanbuf, we also need to return the result
			printf("frame size: %u\n", seek - 1);
			free(scanbuf);
			lseek(fd, 0, SEEK_SET);//seek offset to the beginning before exiting function
			return seek - 1;//seek-1 bytes before first "\nnekomark"
		}

		for(i = 0; i < bytes_get; i++) {
			if(scanbuf[i] == '\n') {
				last_lineend = i;
				if(bytes_get - i - 1 >= strlen(DELIM)) {//make sure we won't access memory that is not allocated
					if(strncmp(DELIM, scanbuf + i + 1, strlen(DELIM)) == 0) {
						printf("frame size: %u\n", seek + i);
						free(scanbuf);
						lseek(fd, 0, SEEK_SET);//seek offset to the beginning before exiting function
						return seek + i;//seek+i bytes before first "\nnekomark"
					}
				}
			}
		}

		if(bytes_get < pagesize)//if the file isn't end with "/n" and we dont find any DELIM, the loop won't stop, so break if nothing to read
			break;

		if(bytes_get - last_lineend - 1 < strlen(DELIM)) {
			seek = seek + 1 + last_lineend;
			lseek(fd, seek, SEEK_SET);//seek to the character after last "\n" in scanbuf if there is no space for another strncmp after last "/n"
		} else {
			seek = seek + pagesize;//we need to use seek for return value, so if we didn't do lseek, we need to update it
		}

		bytes_get = read(fd, scanbuf, pagesize);
		if(bytes_get < 0) {
			printf("error: read txt file failed\n");
			return -1;
		}
	}

	printf("error: no %s found in file\n", DELIM);
	free(scanbuf);
	return -1;
}
