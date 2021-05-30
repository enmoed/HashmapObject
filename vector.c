#include "vector.h"

#define SUCCESS 1
#define FAIL 0
#define ZERO 0
#define ONE 1
#define NEGATIVE -1

/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc(vector_elem_cpy elem_copy_func, vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func)
{
  if (elem_free_func == NULL || elem_copy_func == NULL ||
      elem_cmp_func == NULL)
    {
      return NULL;
    }
  vector *new_vec = (vector*)malloc(sizeof (vector));
  if (new_vec == NULL)
    {
      return NULL;
    }
  void **new_data = (void**)calloc(VECTOR_INITIAL_CAP, sizeof (void*));
  if (new_data == NULL)
    {
      free(new_vec);
      return NULL;
    }
  new_vec->data = new_data;
  new_vec->capacity = VECTOR_INITIAL_CAP;
  new_vec->size = ZERO;
  new_vec->elem_cmp_func = elem_cmp_func;
  new_vec->elem_copy_func = elem_copy_func;
  new_vec->elem_free_func = elem_free_func;
  return new_vec;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free(vector **p_vector)
{
  if (p_vector == NULL || *p_vector == NULL)
    {
      return;
    }
  vector_clear(*p_vector);
  free((*p_vector)->data);
  free((*p_vector));
  (*p_vector) = NULL;
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself, not a copy of it),
 * NULL otherwise.
 */
void *vector_at(const vector *vector, size_t ind)
{
  if (vector == NULL || ind >= vector->size || ind < ZERO)
    {
      return NULL;
    }
  return vector->data[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find(const vector *vector, const void *value)
{
  if (vector == NULL || value == NULL)
    {
      return NEGATIVE;
    }
  for (int i = ZERO; i<vector->size; i++)
    {
      if (vector->elem_cmp_func(value,vector->data+i))
        {
          return i;
        }
    }
  return NEGATIVE;
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back(vector *vector, const void *value)
{
  if (vector == NULL || value == NULL)
    {
      return FAIL;
    }
  vector->data[vector->size] = vector->elem_copy_func(value);
  vector->size++;
  if (vector_get_load_factor(vector)>VECTOR_MAX_LOAD_FACTOR)
    {
      void **temp = (void **) realloc (vector->data,
                                       (vector->capacity
                                        * VECTOR_GROWTH_FACTOR)*sizeof
                                        (void*));
      if (temp == NULL)
        {
          vector->elem_free_func(&(vector->data[vector->size]));
          vector->size--;
          return FAIL;
        }
      vector->data = temp;
      vector->capacity *= VECTOR_GROWTH_FACTOR;
    }
  return SUCCESS;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor(const vector *vector)
{
  if (vector == NULL)
    {
      return NEGATIVE;
    }
  return (double) vector->size/vector->capacity;
}

/**
 * Removes the element at the given index from the vector. alters the indices of the remaining
 * elements so that there are no empty indices in the range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase(vector *vector, size_t ind)
{
  if (vector == NULL || ind < ZERO || ind >= vector->size)
    {
      return FAIL;
    }
  vector->size--;
  if (vector_get_load_factor(vector)<VECTOR_MIN_LOAD_FACTOR)
    {
      void **temp = (void **) realloc (vector->data,
                                       (vector->capacity
                                        / VECTOR_GROWTH_FACTOR)*sizeof
                                        (void*));
      if (temp == NULL)
        {
          vector->size++;
          return FAIL;
        }
      vector->data = temp;
      vector->capacity /= VECTOR_GROWTH_FACTOR;
    }
  vector->elem_free_func(&(vector->data[ind]));
  for (size_t i=ind; i < vector->size; i++)
    {
      vector->data[i] = vector->data[i+ONE];
    }
  vector->data[vector->size] = NULL;
  return SUCCESS;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear(vector *vector)
{
  if (vector == NULL)
    {
      return;
    }
  for (int i=(int) vector->size; i>= ONE; i--)
    {
      vector_erase (vector, i-ONE);
    }
}

