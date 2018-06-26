#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>

int getSemitoneOrder(char* sem, int len){
	char lead = sem[0];

	char subbuff[2];
	memcpy( subbuff, &sem[len-1], 1 );
	subbuff[1] = '\0';

	int block = atoi(subbuff);
	
	int order_i = ((lead - 'a') - 2) % 7;
	order_i = order_i < 0 ? order_i + 7 : order_i;

	static int real_order[] = {1, 3, 5, 6, 8, 10, 12};
	int order = real_order[order_i];
	
	if (len>2){
		order++;
	}

	if (block == 0){
		order -= 9;
	}
	
	if (block > 0)
		order += (--block) * 12 + 3;

	return order;
}

int main(){
	char test[3];
	int i = 0;
	while (i++<10){
		printf("\nEnter semitone string...\n");
		scanf("%s", test);
		int a4_index = getSemitoneOrder("a4", 2);
		int index = getSemitoneOrder(test, strlen(test));
		printf("\n Semitone Frequency: %.3fHz\n", pow(2.0f, (index - a4_index)/12.0f) * 440);

	}
	return 0;
}