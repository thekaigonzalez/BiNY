/*Copyright 2019-2023 Kai D. Gonzalez*/

#ifndef BINY_H
#define BINY_H

// BiNY - Dynamic object management
// biny.h

#include <stdlib.h>

#define BINY_VERSION "0.0.1"
#define BINY_MEMORY_INCREMENT 50

typedef enum _Biny_Type {
  BINY_FLAT = 0,
  BINY_LIST = 1
} Biny_Type;

/// a free function
typedef void (*biny_free_function)(void*);

typedef struct _Biny_Object {
  void* ptr;
  void** ptrs;
  Biny_Type type;

  int size;
  int capacity;

  biny_free_function free;

  int __do_object_free;

} Biny_Object;

Biny_Object *biny_create_object(Biny_Type type);
void* biny_get_object(Biny_Object* obj);
void* biny_object_at(Biny_Object* obj, int index);

void biny_disable_object_free(Biny_Object* obj);
void biny_push_object(Biny_Object* obj, void* ptr);
void biny_set_object(Biny_Object* obj, void* ptr);
void biny_set_free(Biny_Object* obj, biny_free_function free);
void biny_destroy_object(Biny_Object* obj);

#endif
