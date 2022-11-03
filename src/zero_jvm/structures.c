//
// Created by nectostr on 10/28/22.
//

#include "structures.h"
#include "loader.h"

char s_t[STATICS_TABLE_SIZE];
char * statics_table = s_t;
MapEntry statics_map[STATICS_MAP_SIZE];

unsigned int current_statics_map_index = 0;
unsigned int current_statics_table_offset = 0;

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
        printf("%s %d %d\n", entry->name, entry->type, entry->offset);
        entry++;
    }
    printf("\n");
}


void add_statics_entry(char* name, char* type) {
    /*
        * 1. Copy name as a string to statics_map
        * 2. Find out type and chage offset accordingly
        * 3. Convert types to inner types
        * 4. Update current_statics_map_index

    */


    statics_map[current_statics_map_index].offset = current_statics_table_offset;
    //TODO: change the size to something valid
    char * entry = statics_table + current_statics_table_offset;
    memcpy(&statics_map[current_statics_map_index].name, name, 1);

    //TODO: learn how to know the type of enty
    if (1) {  //if long/double
        current_statics_table_offset += 8;
    } else {
        current_statics_table_offset += 4;
    }

    //TODO: learn how to know the type
    switch (1) { //type
        case 1: //int, float 
            int tmp4 = 0;
            memcpy(entry, &tmp4, WORD_SIZE); 
            statics_map[current_statics_map_index].type = 1;
        case 2: //long double
            long tmp8 = 0;
            memcpy(entry, &tmp8, WORD_SIZE*2); 
            statics_map[current_statics_map_index].type = 2;
        default: //method, ?
            entry = 0;// default or compiled add
            statics_map[current_statics_map_index].type = 0;
    }
    current_statics_map_index++;
}