#include "hashmap.h"

#define ZERO 0
#define ONE 1
#define FAIL 0
#define SUCCESS 1
#define NEGATIVE -1
#define HALF 0.5

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  if (func == NULL)
    {
      return NULL;
    }
  hashmap *new_map = (hashmap *) malloc (sizeof (hashmap));
  if (new_map == NULL)
    {
      return NULL;
    }
  vector **new_buckets = (vector **) calloc (HASH_MAP_INITIAL_CAP,
                                             sizeof(vector *));
  if (new_buckets == NULL)
    {
      free (new_map);
      return NULL;
    }
  new_map->size = ZERO;
  new_map->capacity = HASH_MAP_INITIAL_CAP;
  new_map->buckets = new_buckets;
  new_map->hash_func = func;
  return new_map;
}
/**
 * calculates the hash for specific key
 * @param hash_map
 * @param key
 * @return hashed key
 */
size_t get_hash (const hashmap *hash_map, const_keyT key)
{
  return (hash_map->hash_func (key)) & (hash_map->capacity - ONE);
}

/**
 * swaps buckets for specific pair during resize of capacity
 * @param hash_map
 * @param data pair
 * @param new_buckets the new bucket
 * @return 1 upon success 0 upon failure
 */
int bucket_swap (hashmap *hash_map, const pair *data, vector **new_buckets)
{
  size_t ind = get_hash (hash_map, data->key);
  int new = ZERO;
  if (new_buckets[ind] == NULL)
    {
      vector *new_vec = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (new_vec == NULL)
        {
          return FAIL;
        }
      new = ONE;
      new_buckets[ind] = new_vec;
    }
  if (vector_push_back (new_buckets[ind], data) == ZERO)
    {
      if (new == ONE)
        {
          vector_free (&new_buckets[ind]);
          new_buckets[ind] = NULL;
        }
      return FAIL;
    }
  return SUCCESS;
}
/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  if (p_hash_map == NULL || *p_hash_map == NULL)
    {
      return;
    }
  for (size_t i = ZERO; i < (*p_hash_map)->capacity; i++)
    {
      if ((*p_hash_map)->buckets[i] == NULL)
        {
          continue;
        }
      vector_free (&(*p_hash_map)->buckets[i]);
      (*p_hash_map)->buckets[i] = NULL;
    }
  free ((*p_hash_map)->buckets);
  free ((*p_hash_map));
  (*p_hash_map) = NULL;
}
/**
 * frees buckets
 * @param bucket vector pointer to pointer
 * @param hash_map
 */
void free_buckets (vector **bucket, hashmap *hash_map)
{
  for (size_t i = ZERO; i < hash_map->capacity; i++)
    {
      if (bucket[i] != NULL)
        {
          vector_free (&bucket[i]);
        }
    }
  free (bucket);
}
/**
 * resizes buckets when capacity changes
 * @param hash_map
 * @param size old size of map
 * @return new buckets for hashmap
 */
vector **resize_hashmap (hashmap *hash_map, size_t size)
{
  if (hash_map == NULL)
    {
      return NULL;
    }
  vector **new_buckets = (vector **) calloc (hash_map->capacity,
                                             sizeof (vector *));
  if (new_buckets == NULL)
    {
      return NULL;
    }
  for (size_t i = ZERO; i < size; i++)
    {
      if (hash_map->buckets[i] == NULL)
        {
          continue;
        }
      for (size_t j = ZERO; j < hash_map->buckets[i]->size; j++)
        {
          if (bucket_swap (hash_map, (const pair *) vector_at
              (hash_map->buckets[i], j), new_buckets) == FAIL)
            {
              free_buckets (new_buckets, hash_map);
              return NULL;
            }
        }
    }
  return new_buckets;
}

/**
 * gets location of key in vector
 * @param key
 * @param temp pointer to pointer of vectors
 * @param ind index of vector
 * @return index of key in vector
 */
int get_location (const void *key, vector **temp, size_t ind)
{
  if (temp[ind] == NULL)
    {
      return NEGATIVE;
    }
  for (size_t i = ZERO; i < temp[ind]->size; i++)
    {
      pair *data = vector_at ((const vector *) temp[ind], (size_t) i);
      if (data == NULL)
        {
          return NEGATIVE;
        }
      if (data->key_cmp (data->key, key) == ONE)
        {
          return i;
        }
    }
  return NEGATIVE;
}

/**
 * reverts condensed bucket after resize
 * @param hash_map
 * @param temp
 * @param new_bucket_size 1 upon resize 0 upon no resize
 * @return 0
 */
int revert_condensed_bucket (hashmap *hash_map, vector **temp, int
new_bucket_size)
{
  if (new_bucket_size == ONE)
    {
      free_buckets (temp, hash_map);
      hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
    }
  hash_map->size++;
  return FAIL;
}

/**
 * checks inputs of insert function
 * @param hash_map
 * @param in_pair pair being inserted
 * @return new pair upon success
 */
int check_hashmap_insert_inputs (hashmap *hash_map, const pair *in_pair)
{
  // check inputs
  if (hash_map == NULL || in_pair == NULL || in_pair->key == NULL ||
      in_pair->value == NULL)
    {
      return FAIL;
    }
  // check if key already in map
  if (hashmap_at ((const hashmap *) hash_map, (const_keyT) in_pair->key) !=
      NULL)
    {
      return FAIL;
    }
  return SUCCESS;
}

/**
 * removes enlarged bucket after attempted resize
 * @param hash_map
 * @param new_pair pair being added
 * @param temp temporary buckets
 * @param new_bucket_size 1 upon resize 0 upon no resize
 */
void revert_enlarged_bucket (hashmap *hash_map, vector **temp,
                             int new_bucket_size)
{
  if (new_bucket_size == ONE)
    {
      if (temp != NULL)
        {
          free_buckets (temp, hash_map);
          temp = NULL;
        }
      hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
    }
  hash_map->size--;
}

/**
 * restores hashmap after attempted insert failure
 * @param hash_map
 * @param new_pair pair that failed to be inserted
 * @param temp
 * @param new_bucket_size 1 upon resize 0 upon no resize
 * @param is_new_vector 1 upon new vector 0 upon no resize
 * @param ind index of vector
 * @return 0
 */
int vector_push_back_fail (hashmap *hash_map, vector **temp, int
new_bucket_size, int is_new_vector, size_t ind)
{
  if (is_new_vector == ONE)
    {
      vector_free (&temp[ind]);
    }
  revert_enlarged_bucket (hash_map, temp, new_bucket_size);
  return FAIL;
}

/**
 * removes old buckets after successful insert
 * @param hash_map
 * @param temp buckets
 * @param new_bucket_size 1 upon resize 0 upon no resize
 * @param num
 * @return 1
 */
int remove_old_bucket (hashmap *hash_map, vector **temp, int
new_bucket_size, double num)
{
  if (new_bucket_size == ONE)
    {
      vector **old = hash_map->buckets;
      hash_map->buckets = temp;
      hash_map->capacity = (size_t) hash_map->capacity * num;
      free_buckets (old, hash_map);
      hash_map->capacity = (size_t) hash_map->capacity / num;
    }
  return SUCCESS;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  int good_inputs = check_hashmap_insert_inputs (hash_map, in_pair);
  if (good_inputs == FAIL)
    {
      return FAIL;
    }
  hash_map->size++;
  vector **temp = hash_map->buckets;
  int new_bucket_size = ZERO;
  int is_new_vector = ZERO;
  if (hashmap_get_load_factor ((const hashmap *) hash_map) >
      HASH_MAP_MAX_LOAD_FACTOR)
    {
      hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
      temp = resize_hashmap (hash_map,
                             hash_map->capacity / HASH_MAP_GROWTH_FACTOR);
      new_bucket_size = ONE;
      if (temp == NULL)
        {
          revert_enlarged_bucket (hash_map, temp, new_bucket_size);
          return FAIL;
        }
    }
  size_t ind = get_hash ((const hashmap *) hash_map,(const_keyT) in_pair->key);
  if (temp[ind] == NULL)
    {
      vector *new_vec = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (new_vec == NULL)
        {
          revert_enlarged_bucket (hash_map, temp, new_bucket_size);
          return FAIL;
        }
      temp[ind] = new_vec;
      is_new_vector = ONE;
    }
  if (vector_push_back (temp[ind], in_pair) == ZERO)
    {
      return vector_push_back_fail (hash_map, temp, new_bucket_size,
                                    is_new_vector, ind);
    }
  return remove_old_bucket (hash_map, temp, new_bucket_size, HALF);
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return NULL;
    }
  size_t ind = get_hash (hash_map, key);
  if (hash_map->buckets[ind] == NULL)
    {
      return NULL;
    }
  for (size_t i = ZERO; i < hash_map->buckets[ind]->size; i++)
    {
      pair *data = vector_at ((const vector *) hash_map->buckets[ind], (size_t)
          i);
      if (data == NULL)
        {
          return NULL;
        }
      if (data->key_cmp (data->key, key) == ONE)
        {
          return data->value;
        }
    }
  return NULL;
}
/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise. (if key not in map,
 * considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return FAIL;
    }
  hash_map->size--;
  vector **temp = hash_map->buckets;
  int new_bucket_size = ZERO;
  if (hashmap_get_load_factor (hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
    {
      new_bucket_size = ONE;
      hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
      temp = resize_hashmap (hash_map,
                             hash_map->capacity * HASH_MAP_GROWTH_FACTOR);
      if (temp == NULL)
        {
          hash_map->size++;
          hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
          return FAIL;
        }
    }
  size_t ind = get_hash ((const hashmap *) hash_map, (const_keyT) key);
  int location = get_location (key, temp, ind);
  if (location == NEGATIVE || vector_erase (temp[ind], (size_t) location) ==
                              FAIL)
    {
      return revert_condensed_bucket (hash_map, temp, new_bucket_size);
    }
  return remove_old_bucket (hash_map, temp, new_bucket_size,
                            HASH_MAP_GROWTH_FACTOR);
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL)
    {
      return NEGATIVE;
    }
  return (double) hash_map->size / hash_map->capacity;
}
/**
 * This function receives a hashmap and 2 functions, the first checks a condition on the keys,
 * and the seconds apply some modification on the values. The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a capital letter (A-Z),
 * and val_t_func multiples the number by 2, hashmap_apply_if will resize_hashmap the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                  valueT_func valT_func)
{
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    {
      return NEGATIVE;
    }
  int count = ZERO;
  for (size_t i = ZERO; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] == NULL)
        {
          continue;
        }
      for (size_t j = ZERO; j < hash_map->buckets[i]->size; j++)
        {
          if (keyT_func (((pair *) vector_at (hash_map->buckets[i], j))->key)
              == ONE)
            {
              valT_func (((pair *) vector_at(hash_map->buckets[i], j))->value);
              count++;
            }
        }
    }
  return count;
}
