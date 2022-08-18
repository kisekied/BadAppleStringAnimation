#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef PATH
#define PATH  "badapple.txt"
#endif

#define EQUALS(x, y) (strcmp(x, y) == 0)

static size_t delay = 50000;

int main(int argc, char const *argv[])
{
  for(int i = 1; i < argc; i++)
  {
    if(EQUALS(argv[i], "-d"))
      delay = strtol(argv[++i], NULL, 0);
    else if(EQUALS(argv[i], "-h") || EQUALS(argv[i], "--help"))
    {
      printf(
          "badapple\n"
          "\n"
          "Usage:\tbadapple <-d delay> <-h,--help>\n"
          "\n"
          "-h,--help\tShow this help page\n"
          "\n"
          "-d\t\tSet delay between frames\n"
          "\n"
          "Author: kisekied <https://github.com/kisekied>\n"
          );
      exit(EXIT_FAILURE);
    }
  }

	FILE * fp = fopen(PATH, "r");
	fseek(fp, 0, SEEK_END);
	int file_size = ftell(fp);
	printf("%d\n", file_size);
	char * tmp;
	fseek(fp, 0, SEEK_SET);
	tmp = (char *)malloc(file_size * sizeof(char));
	fread(tmp, file_size, sizeof(char), fp);
	char * delim = "nekomark";
	char * p = NULL;

	p = strtok(tmp, delim);
	while (p != NULL)
	{
		system("clear");
		printf("%s", p);
		usleep(delay);
		p = strtok(NULL, delim);
	}

	return EXIT_SUCCESS;
}
