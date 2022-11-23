#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "constants.h"
#include "runtime.h"
#include "utils.h"
#include "frame.h"

#ifndef X86
#include <storage/storage.h>
#endif


extern uint8_t filebytebuffer[8];
extern FILE *filepointer;

void loadfile(const char *classname);

ConstantPoolEntry read_constant_pool_entry(uint8_t tag);

MFInfo read_meth_field_info();

AttributeInfo read_attribute_info();

char *get_constant_pool_entry_name(JavaClass *class, uint32_t index);

void add_statics_entry(JavaClass *class, MFInfo *info);

void add_instance_entry(JavaClass *class, MFInfo *info);

uint32_t find_static_record(char *name, char *signature, uint8_t type);

uint32_t find_instance_offset(JavaClass *class, char *name, char *signature, uint8_t type);

JavaClass *read_class(char *filename);

//void debug_print_statics_map();
//
//void debug_print_statics_table();
//
//void debug_print_rep(JavaClass *class);
//
//void debug_print_map(JavaClass *class);
//
//void debug_print_obj_tmpl(JavaClass *class);