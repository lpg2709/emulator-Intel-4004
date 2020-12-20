#include "utils.h"

const char* read_file(const char *path){
	FILE *f = fopen(path, "r");
	int file_size = 0;
	char *file_content;
	if(f == NULL){
		printf("Error: fail to open the file: %s\n", path);
		getchar();
		exit(1);
	}

	while(fgetc(f) != EOF){
		file_size++;
	}

	if(file_size == 1){
		printf("Error: no content on file: %s\n", path);
		getchar();
		exit(1);
	}

	fseek(f, 0, SEEK_SET);

	file_content = (char *) malloc(sizeof(char)*(file_size+1));


	if(file_content == NULL){
		printf("Error: fail to allocate memory for file content!\n");
		getchar();
		exit(1);
	}
	
	fread(file_content, 1, file_size, f);

	fclose(f);

	return file_content;
}


