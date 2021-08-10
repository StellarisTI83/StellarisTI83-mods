#ifndef KEYS_H
#define KEYS_H

enum _keys {
    ky_none, ky_struct, ky_name, ky_ship_size
};

typedef enum _keys keys;

enum _value_type {
    type_error, type_struct, type_struct_end, type_string, type_int
};

typedef enum _value_type value_type;

#endif