#include "files.h"

#include <stdlib.h>
#include <string.h>

const char* read_file(const char *path, long *f_size){
	FILE *f = fopen(path, TEXT_READ_FILE);
	long file_size = 0;
	char *file_content;
	if(f == NULL){
		printf("Error: fail to open the file: %s\n", path);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	file_size = ftell(f);
	fseek(f, 0L , SEEK_SET);

	if(file_size == 1){
		printf("Error: no content on file: %s\n", path);
		exit(1);
	}

	file_content = (char *) malloc(sizeof(char)*(file_size+1));


	if(file_content == NULL){
		printf("Error: fail to allocate memory for file content!\n");
		exit(1);
	}

	fread(file_content, 1, file_size, f);

	fclose(f);

	file_content[file_size] = '\0';

	*f_size = file_size;
	return file_content;
}

const void *b_read_file(const char *path, long *bf_size){
	FILE *f = fopen(path, "rb");
	long file_size = 0;
	const void *file_content;
	if(f == NULL){
		printf("Error: fail to open the file: %s\n", path);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	file_size = ftell(f);
	fseek(f, 0L , SEEK_SET);

	if(file_size == 1){
		printf("Error: no content on file: %s\n", path);
		exit(1);
	}

	fseek(f, 0, SEEK_SET);

	file_content = (char *) malloc(sizeof(char)*(file_size+1));

	if(file_content == NULL){
		printf("Error: fail to allocate memory for file content!\n");
		exit(1);
	}

	fread((void*)file_content, 1, file_size, f);

	fclose(f);

	*bf_size = file_size;
	return file_content;
}

bool b_write_file(const char *path, const void *content, size_t size, size_t content_size){
	FILE *f;

	f = fopen(path, BINARY_WRITE_FILE);

	if(f == NULL){
		printf("Error: fail to alloc memory for write file: %s\n", path);
		exit(1);
	}

	if(fwrite(content, size, content_size, f) != content_size){
		printf("Error: fail to write file: %s\n", path);
		return false;
	}

	fclose(f);
	return true;
}
