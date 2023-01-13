#include <covenant/vec.h>
#include <covenant/status.h>

#define GROW_FACTOR 1.618
#define SHRINK_THRESHOLD 0.25

Status vec_realloc(Vector*, USize);
Status vec_grow(Vector*);
Status vec_assign(Vector*, USize, UniversalType);
UniversalType vec_get_offset_of(Vector*, USize);
Status vec_move_right(Vector*, USize);
Status vec_move_left(Vector*, USize);