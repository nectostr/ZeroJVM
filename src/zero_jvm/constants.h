#pragma once

#define STATICS_MAP_SIZE   64
#define STATICS_TABLE_SIZE 64
#define MAX_THEORETICAL_CLASS_MAP_SIZE 64


//#define MAX_NAME_SIZE      32
#define MAX_TYPE_SIZE       4
#define WORD_SIZE           4

// TODO: add type name
#define MAP_TYPE_SM   0
#define MAP_TYPE_SF   1
#define MAP_TYPE_SIM  2
#define MAP_TYPE_MAP  3
#define MAP_TYPE_REP  4
#define MAP_TYPE_O    5
#define MAP_TYPE_CL   6

#define ACC_PUBLIC        0x0001    // Marked or implicitly public in source.
#define ACC_PROTECTED    0x0004    // Marked protected in source.
#define ACC_PRIVATE    0x0002  //	Marked private in source.
#define ACC_STATIC        0x0008    // Marked or implicitly static in source.
#define ACC_FINAL    0x0010    // Marked or implicitly final in source.
#define ACC_INTERFACE    0x0200    // Was an interface in source.
#define ACC_ABSTRACT    0x0400    // Marked or implicitly abstract in source.
#define ACC_SYNTHETIC    0x1000    // Declared synthetic; not present in the source code.
#define ACC_ANNOTATION    0x2000    // Declared as an annotation type.
#define ACC_ENUM        0x4000    // Declared as an enum type.