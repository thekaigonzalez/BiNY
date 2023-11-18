#include "biny.h"

void array_free_int(void* ptr) {
  printf("free called with %d\n", (int)ptr);
}

main(void) {
  Biny_Object * obj = biny_create_object (BINY_LIST);

  biny_disable_object_free (obj);
  
  biny_push_object (obj, 1);
  biny_push_object (obj, 2);
  biny_push_object (obj, 3);
  biny_push_object (obj, 4);

  printf("%d\n", obj->ptrs[0]);
  printf("%d\n", obj->ptrs[1]);
  printf("%d\n", obj->ptrs[2]);
  printf("%d\n", obj->ptrs[3]);

  biny_destroy_object (obj);
}
