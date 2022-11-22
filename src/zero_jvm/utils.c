#include <string.h>
#include <stdlib.h>
#include "utils.h"

char *combine_names_with_dot(char *firstname, char *lastname) {
    unsigned long fullmethodnamelen = strlen(firstname) + strlen(lastname) + 2;
    char *fullname = calloc(fullmethodnamelen, sizeof(char));
    strcpy(fullname, firstname);
    fullname[strlen(firstname)] = '.';
    strcpy(&fullname[strlen(firstname) + 1], lastname);
    return fullname;
}