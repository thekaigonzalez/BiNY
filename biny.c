#include "biny.h"

#include <stdio.h>
#include <string.h>

Biny_Object *
biny_create_object (Biny_Type type)
{
  Biny_Object *obj = calloc (sizeof (Biny_Object), 1);

  if (!obj)
    {
      printf ("BiNY: Failed to allocate memory for object\n");
      return NULL;
    }

  obj->type = type;
  obj->size = 0;
  obj->capacity = BINY_MEMORY_INCREMENT;

  switch (obj->type)
    {
    case BINY_LIST:
      obj->ptrs = calloc (obj->capacity, sizeof (void *));

      if (!obj->ptrs) {
        printf ("BiNY: Failed to allocate memory for object (list)\n");
        free (obj);
        return NULL;
      }
      break;
    case BINY_FLAT:
      obj->ptr = calloc (obj->capacity, sizeof (void *));

      if (!obj->ptr) {
        printf ("BiNY: Failed to allocate memory for object (flat)\n");
        free (obj);
        return NULL;
      }

      obj->__do_object_free = 0;
      break;
    default:
      printf ("BiNY: Unknown object type\n");
      break;
    }

  return obj;
}

void *
biny_get_object (Biny_Object *obj)
{
  if (!obj || !obj->ptr || !obj->ptrs)
    {
      printf ("BiNY: Invalid object\n");
      return NULL;
    }

  switch (obj->type)
    {
    case BINY_LIST:
      return *obj->ptrs;
    case BINY_FLAT:
      return obj->ptr;
    }
  return NULL;
}

void *
biny_object_at (Biny_Object *obj, int index)
{
  if (!obj || !obj->ptrs)
    {
      printf ("BiNY: Invalid object\n");
      return NULL;
    }

  if (index < 0 || index >= obj->size)
    {
      printf ("BiNY: Invalid index\n");
      return NULL;
    }

  return obj->ptrs[index];
}

void
biny_disable_object_free (Biny_Object *obj)
{
  if (!obj)
    {
      printf ("BiNY: Invalid object\n");
      return;
    }

  obj->free = NULL;
  obj->__do_object_free = 0;
}

void
biny_push_object (Biny_Object *obj, void *ptr)
{
  if (!obj || !ptr || !obj->ptrs)
    {
      printf ("BiNY: Invalid object\n");
      return;
    }

  switch (obj->type)
    {
    case BINY_LIST:
      {
        if (obj->size >= obj->capacity)
          {
            obj->capacity += BINY_MEMORY_INCREMENT;
            obj->ptrs = realloc (obj->ptrs, obj->capacity * sizeof (void *));
          }

        obj->ptrs[obj->size] = ptr;
        obj->size++;
      }
      break;
    case BINY_FLAT:
      if (obj->size >= obj->capacity)
        {
          obj->capacity += BINY_MEMORY_INCREMENT;
          obj->ptr = realloc (obj->ptr, obj->capacity * sizeof (void *));
        }

      ((void **)obj->ptr)[obj->size] = ptr;

      obj->size++;
      break;
    default:
      printf ("BiNY: Unknown object type\n");
      break;
    }
}

void
biny_set_object (Biny_Object *obj, void *ptr)
{
  if (!obj || !ptr)
    {
      printf ("BiNY: Invalid object\n");
      return;
    }

  if (obj->type == BINY_LIST)
    {
      printf ("BiNY: List object cannot be set\n");
      return;
    }

  memcpy (obj->ptr, &ptr, sizeof (void *));
}

void
biny_set_free (Biny_Object *obj, biny_free_function free)
{
  if (!obj)
    {
      printf ("BiNY: Invalid object\n");
      return;
    }

  obj->free = free;
}

void
biny_destroy_object (Biny_Object *obj)
{
  if (!obj)
    {
      printf ("BiNY: Invalid object\n");
      return;
    }

  switch (obj->type)
    {
    case BINY_LIST:
      {
        for (int i = 0; i < obj->size; i++)
          {
            if (obj->free)
              obj->free (obj->ptrs[i]); // allow free function to access the
                                        // object's memory
            if (obj->__do_object_free == 1)
              free (obj->ptrs[i]);
          }
      }

      free (obj->ptrs);
      free (obj);
      break;
    case BINY_FLAT:
      {
        free (obj->ptr);
        free (obj);
      }
      break;
    }
}
