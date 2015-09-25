#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	FILE * fp;
	fp = fopen("badapple.txt", "r");
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
		usleep(50000);
		p = strtok(NULL, delim);
	}



	return 0;
}