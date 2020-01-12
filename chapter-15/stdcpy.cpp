#include <iostream>
#include <stdio.h>

const unsigned int BUF_SIZE = 3;

using namespace std;

int main(int argc, char const *argv[])
{
	FILE *fp1;
	FILE *fp2;
	char buf[BUF_SIZE];

	fp1 = fopen("news.txt", "r");
	fp2 = fopen("cpy.txt", "w");

	while(fgets(buf, sizeof(BUF_SIZE), fp1) != NULL)
		fputs(buf, fp2);

	fclose(fp1);
	fclose(fp2);
	return 0;
}