// Get from Utopia pointers
#include "internal/_ptrkey.h"

#define EXECUTABLE_PATH_EQUALS_LEN 16

const char*
mdlk_get_executable_path(const char* value_key_pair) {
    return value_key_pair + EXECUTABLE_PATH_EQUALS_LEN;
}