//
// Created by nectostr on 10/28/22.
//

#include "structures.h"
#include "loader.h"

char statics_table[STATICS_TABLE_SIZE];
MapEntry statics_map[STATICS_MAP_SIZE];

void debug_print_statics_table() {
    printf("STATICS_TABLE\n");
    unsigned int * entry = (unsigned int *) statics_table;
    while (entry[0] != '\0') {
        printf("%04x\n", *entry);
        entry++;
    }
    printf("\n");
}

void debug_print_statics_map() {
    printf("STATICS_MAP\n");
    MapEntry * entry = statics_map;
    while (entry->name[0] != '\0') {
        printf("%s %s %d\n", entry->name, entry->type, entry->offset);
        entry++;
    }
    printf("\n");
}