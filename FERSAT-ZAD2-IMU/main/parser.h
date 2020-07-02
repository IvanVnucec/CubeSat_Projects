#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include<string.h>
#include<stdio.h>

#include"list.h"
#include"function.h"
#include"wrapper.h"

#define MAX_ARG_LEN 20

int parse(const char* str);

#endif // PARSER_H_INCLUDED
