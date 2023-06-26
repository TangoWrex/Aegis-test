#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "handle_data.h"

#define MAX_CHAR 1000

FILE *read_file(int argc, char *argv[])
{
	if (argc < 2) {
		return 0;
	}

	FILE *fp = fopen(argv[1], "r");
	if (NULL == fp) {
		perror("[ERROR] fopen()");
		errno = 0;
	} else if (!is_valid_file(fp)) {
		fclose(fp);
		fp = NULL;
	}
	return fp;
}

bool is_valid_file(FILE * fp)
{
	if (NULL == fp) {
		printf("File Not Found!\n");
		return false;
	}
	bool answer = true;

	int placeholder = ftell(fp);

	fseek(fp, 0, SEEK_END);
	if (0L == ftell(fp)) {
		answer = false;
		fprintf(stderr, "This is a zero byte file\n");
	} else {
		fseek(fp, placeholder, SEEK_SET);
	}
	return answer;
}
