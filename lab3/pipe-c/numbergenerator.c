#include <stdio.h>
#include <stdlib.h>
void main(){
	srand((unsigned) time(0));
	for(int i=0;i<10;i++)
		printf("%d ", rand()%100);
}
