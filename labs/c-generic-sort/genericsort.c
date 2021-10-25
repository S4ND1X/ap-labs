#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 100000000
void *lineptr[MAX_LINES];

void quicksort(void *lineptr[], int left, int right,
			   int (*comp)(void *, void *));

void mergesort(void *lineptr[], int left, int right,
			   int (*comp)(void *, void *));

int readlines(FILE *inputFile);

void writelines(FILE *output, int nlines);

int numcmp(char *, char *);

int numcmp(char *s1, char *s2)
{
	double v1 = atof(s1),
		   v2 = atof(s2);

	return (int)(v1 - v2);
}

/*
 * read lines from inputFile and store them in lineptr
 */
int readlines(FILE *inputFile)
{
	char *line;

	int currentLine = 0;

	size_t len = 0;
	size_t read;

	while ((read = getline(&line, &len, inputFile) != -1))
	{
		if (currentLine == MAX_LINES)
		{
			printf("Files is has too many lines");
			exit(0);
		}
		lineptr[currentLine] = malloc(strlen(line));
		strcpy(lineptr[currentLine++], line);
	}

	fclose(inputFile);
	return currentLine;
}

/*
 * write lines from lineptr to output
 */

void writelines(FILE *output, int nlines)
{
	for (int i = 0; i < nlines; i++)
	{
		fprintf(output, "%s", (char *)lineptr[i]);
	}
	fclose(output);
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Error: ./genericsort strings.txt -mergesort -o ms_sorted_strings.txt\n");
		return -1;
	}

	int sorted = 0;

	char ifName[50] = "",
		 ofName[50] = "";

	/*
	Store pointers to the input and output files
	*/
	FILE *inputFile,
		*outputFile;

	if (strcmp(argv[1], "-n") == 0)
	{
		if (argc != 6)
		{
			printf("Error: ./genericsort -n numbers.txt -quicksort -o qs_sorted_numbers.txt\n");
			return -1;
		}
		sorted = 1;
	}
	else if (argc != 5)
	{
		printf("Error: ./genericsort strings.txt -mergesort -o ms_sorted_strings.txt\n");
		return -1;
	}

	strcat(ifName, argv[1 + sorted]); // input file name

	if (strcmp(argv[3 + sorted], "-o") != 0)
	{
		printf("Error: ./genericsort strings.txt -mergesort -o ms_sorted_strings.txt\n");
		return -1;
	}

	strcat(ofName, argv[4 + sorted]);

	if ((inputFile = fopen(ifName, "r")) == NULL)
	{
		printf("Error: file not found\n");
		return 0;
	}

	int currentLine = readlines(inputFile);

	if (strcmp(argv[2 + sorted], "-quicksort") == 0)
	{
		quicksort(lineptr, 0, currentLine - 1, (sorted ? (int (*)(void *, void *))numcmp : (int (*)(void *, void *))strcmp));
	}
	else if (strcmp(argv[2 + sorted], "-mergesort") == 0)
	{
		mergesort(lineptr, 0, currentLine - 1, (sorted ? (int (*)(void *, void *))numcmp : (int (*)(void *, void *))strcmp));
	}
	else
	{
		printf("Error: sort [%s] is not supported\n", argv[2 + sorted]);
		return -1;
	}

	if ((outputFile = fopen(ofName, "w")) == NULL)
	{
		printf("Error: file cannot be created\n");
		return -1;
	}

	writelines(outputFile, currentLine);
	printf("Results file can be found at ./%s\n", ofName);

	return 0;
}