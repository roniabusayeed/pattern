#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


/**
 * Reads an entire line from stream returning the address of
 * the buffer containing the text. The buffer is null-terminated
 * and includes the newline character if one was found.
 * Returns NULL on failure to read a line (including end of file
 * condition). Sets *line_number to line number of the line in stream.
 * The buffer is heap allocated and the caller function is should
 * to free the memory to avoid memory leak.
 */
char* readline(FILE* stream, int* line_number);

/**
 * Returns index of pattern in line; -1 if none.
 */
int strindex(const char* line, const char* pattern);


/**
 * Prints lines file  that contains a pattern
 */
int main(int argc, char* argv[])
{
	// Read optional arguments
	bool except = false;	// Show lines that doesn't contain the pattern.
	bool number = false;	// Show lines preceded by its line number.
	int c;
	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
			case 'x':
				except = true;
				break;
			case 'n':
				number = true;
				break;
			default:
				printf("pattern: illegal option %c\n", c);
				argc = 0;
				break;
			}

	// Ensure proper usage
	if (argc != 2) 
	{
		printf("Usage: %s [-x -n] <file> <pattern string>\n", argv[0]);
		return 1;
	}

	// Remember filename
	const char* infile = argv[0];

	// Remember pattern
	const char* pattern = argv[1];

	// Open file for reading
	FILE* inptr = fopen(infile, "r");
	if (! inptr)
	{
		printf("Couldn't open file\n");
		return 1;
	}
	
	// Iterate over each of the file and print it if it holds the pattern.
	char* line;
	int line_number;
	while ((line = readline(inptr, &line_number)) != NULL)
	{
		bool found = strindex(line, pattern) != -1;
		if (except)
		{
			// Print those lines where pattern is not found.
			if (!found)
			{
				if (number)
					printf("%4d ", line_number);
				printf("%s", line);
			}
		}
		else
		{
			// Print those line where pattern is found.
			if(found)
			{
				if (number)
					printf("%4d ", line_number);
				printf("%s", line);
			}
		}

		// Free buffer
		free(line);
	}

	// Close files
	fclose(inptr);

	// Success
	return 0;
}


char* readline(FILE* stream, int* line_number)
{
	static int s_line_number = 1;

	const size_t init_capacity = 10;
	char* line = malloc(sizeof(char) * init_capacity);
	// Guard against insufficient memory.
	if (!line)
	{
		printf("Insufficient memory\n");
		*line_number = -1;
		return NULL;
	}
	const float increment_factor = 1.25f;	
	size_t capacity = init_capacity;
	size_t size = 0;
	int c;
	while ((c = fgetc(stream)) != EOF)
	{
		// Increase buffer capacity if size exceeds it.
		if (size == capacity)
		{
			capacity = ceil(capacity * increment_factor);
			line = realloc(line, capacity);
			// Guard against insufficient memory.
			if (!line)
			{
				printf("Insufficient memory\n");
				*line_number = -1;
				return NULL;
			}
		}

		line[size++] = c;
		if (c == '\n')
			break;
	}

	// If characters are in buffer return buffer.
	if (size)
	{
		line[size] = 0;
		*line_number = s_line_number++;
		return line;
	}
	
	// Characters are not in buffer. So, free memory and return NULL.	
	free(line);
	*line_number = -1;
	return NULL;	
}

int strindex(const char* line, const char* pattern)
{
	size_t l_line = strlen(line);
	size_t l_pattern = strlen(pattern);

	// If the line is not big enough to have the pattern in it
	// it dones't have it.
	if (l_pattern > l_line)
		return -1;

	// Iterate over the line to find a pattern match.
	for (int i = 0; i < l_line - l_pattern + 1; i++)
	{	
		// Return index of first occurance of found match.	
		if (strncmp(line + i, pattern, l_pattern) == 0)
			return i;
	}

	// Match not found.
	return -1;
}

