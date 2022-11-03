#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#define STATICS_TABLE_SIZE 64
#define MAX_NAME_SIZE      32
#define MAX_TYPE_SIZE       4
#define STATICS_MAP_SIZE   64

// 1byte array, so +offset would work nicely
extern char statics_table[STATICS_TABLE_SIZE];

// alternative: offset always /4
// uint32_t statics_table[STATICS_TABLE_SIZE/4];

struct MapEntry{
    char name[MAX_NAME_SIZE];
    char type[MAX_TYPE_SIZE];
    unsigned int offset;
};

// Renaming, to simplify usage
typedef struct MapEntry MapEntry;


extern MapEntry statics_map[STATICS_MAP_SIZE];

#endif // STRUCTURES_H