#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int calculatesum(char* line){
	const char delim[] = " ";
	int sum = 0;
	char *token = strtok(line, delim);
	while(token != NULL){
		sum += atoi(token);
		token = strtok(NULL, delim);
	}

	return sum;
	

}
void main(int argc, char* argv[]){
	int sum=0;

	char *line = NULL;
	size_t n;

	while(getline(&line, &n, stdin) != -1){
		printf("Random numbers:%s\n", line);
		printf("sum= %d\n",calculatesum(line));
	}

	free(line);
	printf("program finished\n");
	
}
