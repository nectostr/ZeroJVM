//
// Created by nectostr on 10/28/22.
//

#include "structures.h"

char s_t[STATICS_TABLE_SIZE];
char * statics_table = s_t;
MapEntry statics_map[STATICS_MAP_SIZE];

unsigned int current_statics_map_index = 0;
unsigned int current_statics_table_offset = 0;


void debug_random_init(){
    statics_map[0].name = calloc(1, 2);
    statics_map[0].name = "a";
    statics_map[0].type = 1;
    statics_map[0].offset = 0;
    current_statics_map_index = 1;

    statics_table[0] = 1;
}

void debug_print_statics_table() {
    printf("STATICS_TABLE\n");
    for (int i = 0; i < current_statics_table_offset; i=i+4) {
        printf("%d\n", statics_table[i]);
    }
    printf("\n");
}

void debug_print_statics_map() {
    printf("STATICS_MAP\n");
    for(int i = 0; i < current_statics_map_index; i++) {
        printf("%s %d %d\n", statics_map[i].name, statics_map[i].type, statics_map[i].offset);
    }
    printf("\n");
}



// char * get_constant_pool_entry_name(int index) {
//     struct  ConstantPoolEntry * entry = &constant_table[index];
//     while (entry->tag != 1) {
//         entry = &constant_table[entry->data.ushort];
//     }
//     return (char *) entry->addon;
    
// }


/*
[Z = boolean
[B = byte
[S = short
[I = int
[J = long
[F = float
[D = double
[C = char
[L = any non-primitives(Object)
*/

void add_statics_entry(MFInfo * info) {
    /*
        * 1. Copy name as a string to statics_map
        * 2. Find out type and chage offset accordingly
        * 3. Convert types to inner types
        * 4. Update current_statics_map_index

    */

    statics_map[current_statics_map_index].offset = current_statics_table_offset;
    char * entry = statics_table + current_statics_table_offset;
    int tag = info->name_index;
    statics_map[current_statics_map_index].name =  get_constant_pool_entry_name(tag);

    tag = info->descriptor_index;
    char * type = get_constant_pool_entry_name(tag);
    if ((strcmp(type, "J") == 0) ||
        (strcmp(type, "D") == 0)) {  //if long/double
        current_statics_table_offset += 8;
    } else {
        current_statics_table_offset += 4;
    }

    
    switch (*type) { //type
        case 'Z':
        case 'B':
        case 'S':
        case 'F':
        case 'C':
        case 'I': //int, float  
            unsigned int * tmp4 = (unsigned int *) entry;
            *tmp4 = 0;
            statics_map[current_statics_map_index].type = SF;
            break;
        case 'J':
        case 'D': //long double
            unsigned long * tmp8 = (unsigned long *) entry;
            *tmp8 = 0;
            statics_map[current_statics_map_index].type = SF;
            break;
        case '(': //method, 
         
            *entry = 'C';// TODO: default compiler/interpreter link
            if (strcmp(statics_map[current_statics_map_index].name, "<init>") == 0) {
                statics_map[current_statics_map_index].type = SIM;
            } else {
                statics_map[current_statics_map_index].type = SM;
            }
            break;
        //case ?: //Map, Ref, SIM
    }
    current_statics_map_index++;
}