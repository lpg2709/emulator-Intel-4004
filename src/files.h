#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define TEXT_READ_FILE "r"
#define TEXT_WRITE_FILE "w"
#define TEXT_APPEND_FILE "a"
#define TEXT_READWRITE_FILE "r+"
#define TEXT_RW_CREATE_FILE "w+"
#define TEXT_APPEND_CREATE_FILE "a+"

#define BINARY_READ_FILE "rb"
#define BINARY_WRITE_FILE "wb"
#define BINARY_APPEND_FILE "ab"
#define BINARY_READWRITE_FILE "rb+"
#define BINARY_RW_CREATE_FILE "wb+"
#define BINARY_APPEND_CREATE_FILE "ab+"


const char* read_file(const char *path);
const void* b_read_file(const char *path, long *bf_size);
bool b_write_file(const char *path, const void *content, size_t size, size_t content_size);

#endif
