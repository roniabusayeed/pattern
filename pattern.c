#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


/**
 * Reads an entire line from stream returning the address of
 * the buffer containing the text. The buffer is null-terminated
 * and includes the newline character if one was found.
 * Returns NULL on failure to read a line (including end of file
 * condition).
 * The buffer is heap allocated and the caller function is expected
 * to free the memory.
 */
char* readline(FILE* stream);

/**
 * Returns index of pattern in line; -1 if none.
 */
int strindex(const char* line, const char* pattern);


int main(const int argc, const char* argv[])
{
	// Ensure proper usage
	if (argc != 3) 
	{
		printf("Usage: %s <file> <pattern string>\n", argv[0]);
		return 1;
	}

	// Remember filename
	const char* infile = argv[1];

	// Remember pattern
	const char* pattern = argv[2];

	// Open file for reading
	FILE* inptr = fopen(infile, "r");
	if (! inptr)
	{
		printf("Couldn't open file\n");
		return 1;
	}
	
	// Iterate over each of the file and print it if it holds the pattern.
	char* line;
	while ((line = readline(inptr)) != NULL)
	{
		if (strindex(line, pattern) != -1)
			printf("%s", line);

		// Free buffer
		free(line);
	}

	// Close files
	fclose(inptr);

	// Success
	return 0;
}


char* readline(FILE* stream)
{
	const size_t init_capacity = 10;
	char* line = malloc(sizeof(char) * init_capacity);
	// Guard against insufficient memory.
	if (!line)
	{
		printf("Insufficient memory\n");
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
		return line;
	}
	
	// Characters are not in buffer. So, free memory and return NULL.	
	free(line);
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

