#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string.h>
#include "loader.h"

#define STATICS_TABLE_SIZE 64
//#define MAX_NAME_SIZE      32
#define MAX_TYPE_SIZE       4
#define STATICS_MAP_SIZE   64
#define WORD_SIZE           4

#define SM  0
#define SF  1
#define Map 2
#define Ref 3
#define o   4


// 1byte array, so +offset would work nicely
extern char * statics_table;

// alternative: offset always /4
// uint32_t statics_table[STATICS_TABLE_SIZE/4];

struct MapEntry{
    char * name;
    unsigned char type;
    unsigned int offset;
};

// Renaming, to simplify usage
typedef struct MapEntry MapEntry;


extern MapEntry statics_map[STATICS_MAP_SIZE];

void debug_random_init();
void debug_print_statics_map();
void debug_print_statics_table();

void add_statics_entry(MFInfo * info);

#endif // STRUCTURES_H