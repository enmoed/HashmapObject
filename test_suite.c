#include "test_suite.h"
#include "test_pairs.h"
#include "hash_funcs.h"
#include <stdio.h>


#define ONE 1
#define ZERO 0
#define CHAR 1
#define INT 2
#define STRING 4
#define EMPLOYEE 5
#define TWO 2
#define NEGATIVE -1
/**
 * Function for creating new employee key string value, pair
 * @param key employee
 * @param value string
 * @return new pair
 */
pair *getPair_employee_string (const_keyT key, const_valueT value)
{
  return pair_alloc (key, value, (pair_key_cpy) employee_key_cpy,
                     (pair_value_cpy) string_value_cpy,
                     (pair_key_cmp) employee_key_cmp,
                     (pair_value_cmp) string_value_cmp,
                     employee_key_free, string_value_free);
}
/**
 * Function for creating new string key char value, pair
 * @param key string
 * @param value char
 * @return new pair
 */
pair *getPair_string_char (const_keyT key, const_valueT value)
{
  return pair_alloc (key, value, (pair_key_cpy) string_key_cpy,
                     (pair_value_cpy) char_value_cpy,
                     (pair_key_cmp) string_key_cmp,
                     (pair_value_cmp) char_value_cmp,
                     string_key_free, char_value_free);
}
/**
 * Function for creating new int key string value, pair
 * @param key int
 * @param value string
 * @return new pair
 */
pair *getPair_int_string (const_keyT key, const_valueT value)
{
  return pair_alloc (key, value, (pair_key_cpy) int_key_cpy,
                     (pair_value_cpy) string_value_cpy,
                     (pair_key_cmp) int_key_cmp,
                     (pair_value_cmp) string_value_cmp,
                     int_key_free, string_value_free);
}
/**
 * Function for creating new char key int value, pair
 * @param key char
 * @param value int
 * @return new pair
 */
pair *getPair_char_int (const_keyT key, const_valueT value)
{
  return pair_alloc (key, value, (pair_key_cpy) char_key_cpy,
                     (pair_value_cpy) int_value_cpy,
                     (pair_key_cmp) char_key_cmp,
                     (pair_value_cmp) int_value_cmp,
                     char_key_free, int_value_free);
}
/**
 * Function for creating new pairs
 * @param key the key
 * @param value the value
 * @param str_key type of key
 * @param str_val type of value
 * @return new pair
 */
pair *create_pair(const_keyT key, const_valueT value, int str_key,
                  int str_val)
{
  if (str_key == CHAR && str_val == INT)
    {
      return getPair_char_int (key, value);
    }
  if (str_key == INT && str_val == STRING)
    {
      return getPair_int_string (key, value);
    }
  if (str_key == EMPLOYEE && str_val == STRING)
    {
      return getPair_employee_string (key, value);
    }
  if (str_key == STRING && str_val == CHAR)
    {
      return getPair_string_char (key, value);
    }
  return NULL;
}
/**
 * loads hash map with char keys
 * @param hash_map
 */
void loading_init_char_int (hashmap *hash_map)
{
  int num = 12;
  for (int i = ONE; hash_map->size < (HASH_MAP_MAX_LOAD_FACTOR
                                      * HASH_MAP_INITIAL_CAP); i++)
    {
      char current = (char)('b' + i);
      pair *new_pair = create_pair(&current,
                                 (const_valueT) &num,
                                 CHAR, INT);
      assert(hashmap_insert(hash_map, new_pair)==ONE);
      assert(hashmap_insert(hash_map, new_pair)==ZERO);
      pair_free((void **) &new_pair);
      assert((int) hash_map->size == i);
      assert(hash_map->capacity == HASH_MAP_INITIAL_CAP);
      assert(hash_map->hash_func == hash_char);
    }
}
/**
 * tests bad inputs for insert function
 * @param hash_map
 */
void check_insert_inputs (hashmap *hash_map)
{
  int twelve = 12;
  char current = (char)('a'+ 30);
  pair *new_pair = create_pair (&current, &twelve, CHAR, INT);
  assert(hashmap_insert (NULL, new_pair) == ZERO);
  new_pair->key_free(&new_pair->key);
  assert(hashmap_insert (hash_map, new_pair) == ZERO);
  assert(hashmap_insert (NULL, new_pair) == ZERO);
  new_pair->value_free(&new_pair->value);
  assert(hashmap_insert (hash_map, new_pair) == ZERO);
  assert(hashmap_insert (NULL, new_pair) == ZERO);
  assert(hash_map->size == HASH_MAP_MAX_LOAD_FACTOR * HASH_MAP_INITIAL_CAP);
  assert(hash_map->capacity == HASH_MAP_INITIAL_CAP);
  assert(hash_map->hash_func == hash_char);
  pair_free((void **) &new_pair);
}
/**
 * inserts another round of keys after resize
 * @param hash_map
 */
void loading_next (hashmap *hash_map)
{
  size_t current_capacity = hash_map->capacity*HASH_MAP_GROWTH_FACTOR;
  size_t original_size = hash_map->size;
  for (int i = ONE; hash_map->size <
                    current_capacity*HASH_MAP_MAX_LOAD_FACTOR; i++)
    {
      char key = (char)('A' + i);
      int val = 12;
      pair *new_pair = create_pair(&key, &val, CHAR, INT);
      assert(hashmap_insert(hash_map, new_pair)==ONE);
      assert(hashmap_insert(hash_map, new_pair)==ZERO);
      assert(hash_map->size == i + original_size);
      assert(hash_map->capacity == current_capacity);
      assert(hash_map->hash_func == hash_char);
      pair_free((void **) &new_pair);
    }
}
/**
 * load last key after resize
 * @param hash_map
 */
void load_last (hashmap *hash_map)
{
  size_t current_capacity = hash_map->capacity*HASH_MAP_GROWTH_FACTOR;
  size_t original_size = hash_map->size;
  char key = (char)('Z');
  int val = 12;
  pair *new_pair = create_pair(&key, &val, CHAR, INT);
  assert(hashmap_insert(hash_map, new_pair)==ONE);
  assert(hash_map->size == ONE + original_size);
  assert(hash_map->capacity == current_capacity);
  assert(hash_map->hash_func == hash_char);
  pair_free((void **) &new_pair);
}

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code
 * 1.
 */
void test_hash_map_insert(void)
{
  hashmap *hash_map = hashmap_alloc(hash_char);
  loading_init_char_int (hash_map);
  check_insert_inputs (hash_map);
  loading_next (hash_map);
  load_last (hash_map);
  hashmap_free(&hash_map);
}
/**
 * tests bad inputs for hashmap at function
 * @param hash_map
 */
void check_at_inputs (hashmap *hash_map)
{
  int key = (int)('a'+30);
  char *value = "abc";
  pair *new_pair = create_pair (&key,
                                &value, INT, STRING);
  assert(hashmap_at (hash_map, new_pair->key) == NULL);
  assert(hashmap_at (NULL, new_pair->key) == NULL);
  assert(hashmap_at (hash_map, NULL) == NULL);
  assert(hashmap_at (NULL, NULL) == NULL);
  assert(hash_map->size == ZERO);
  assert(hash_map->capacity == HASH_MAP_INITIAL_CAP);
  assert(hash_map->hash_func == hash_int);
  pair_free((void **) &new_pair);
}
/**
 * inserts hashmap with int key and string value
 * @param hash_map
 */
void loading_init_int_string (hashmap *hash_map)
{
  for (char i = ONE; hash_map->size < (HASH_MAP_MAX_LOAD_FACTOR
                                       * HASH_MAP_INITIAL_CAP); i++)
    {
      char str[5] = "abc";
      int key = (int)(-8 + i);
      strncat(str, &i, 1);
      pair *in_pair = create_pair(&key, &str, INT,
                                  STRING);
      hashmap_insert(hash_map, in_pair);
      pair_free((void **) &in_pair);
    }
}
/**
 * checks good inputs of hashmap at
 * @param hash_map
 */
void check_at (hashmap *hash_map)
{
  for (char i = ONE; i-ONE < (HASH_MAP_MAX_LOAD_FACTOR
                              * HASH_MAP_INITIAL_CAP); i++)
    {
      int key = (int)(-8 + i);
      char str[5] = "abc";
      assert(strcmp(hashmap_at(hash_map, &key),
                    strncat(str, &i, 1)) == ZERO);
    }
}
/**
 * inserts another round of keys
 * @param hash_map
 */
void loading_next_int_string (hashmap *hash_map)
{
  size_t current_capacity = hash_map->capacity*HASH_MAP_GROWTH_FACTOR;
  for (char i = ONE; hash_map->size < current_capacity*
                                      HASH_MAP_MAX_LOAD_FACTOR; i++)
    {
      char str[5] = "abd";
      int key = (int)(200 + i);
      pair *new_pair = create_pair(&key,
                                   strncat(str, &i, 1), INT,
                                   STRING);
      hashmap_insert(hash_map, new_pair);
      pair_free((void **) &new_pair);
    }
}

/**
 * checks location of keys after resize
 * @param hash_map
 */
void check_next_at (hashmap *hash_map)
{
  size_t current_capacity = hash_map->capacity;
  for (char i = ONE; hash_map->size/TWO + i - ONE <
                     current_capacity*HASH_MAP_MAX_LOAD_FACTOR; i++)
    {
      int key_1= (int)(-8 + i);
      int key_2= (int)(200 + i);
      char stl[5] = "abd";
      char str[5] = "abc";
      assert(strcmp(hashmap_at(hash_map, &key_2),
                    strncat(stl, &i, 1)) == ZERO);
      assert(strcmp(hashmap_at(hash_map, &key_1),
                    strncat(str, &i, 1)) == ZERO);
    }
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at(void)
{
  hashmap *hash_map = hashmap_alloc(hash_int);
  check_at_inputs (hash_map);
  loading_init_int_string(hash_map);
  check_at(hash_map);
  loading_next_int_string(hash_map);
  check_next_at(hash_map);
  hashmap_free(&hash_map);
}
/**
 * checks erase function for bad inputs
 * @param hash_map
 */
void check_erase_inputs (hashmap *hash_map)
{
  Employee *emp = malloc(sizeof (Employee));
  emp->salary = 100000;
  emp->ID = 2198329;
  emp->name =(char**) "John Dow";
  char str[5] = "abc";
  pair *new_pair = create_pair ((const_keyT) emp,
                                (const_valueT) &str, EMPLOYEE, STRING);
  assert(hashmap_erase (hash_map, new_pair->key) == ZERO);
  assert(hashmap_erase (NULL, new_pair->key) == ZERO);
  assert(hashmap_erase (hash_map, NULL) == ZERO);
  assert(hashmap_erase (NULL, NULL) == ZERO);
  assert(hash_map->size == ZERO);
  assert(hash_map->capacity == HASH_MAP_INITIAL_CAP);
  assert(hash_map->hash_func == hash_employee);
  free(emp);
  pair_free((void **) &new_pair);
}
/**
 * inserts employee keys and string values into new hashmap
 * @param hash_map
 */
void loading_init_employee_string (hashmap *hash_map)
{
  for (char i = ONE; hash_map->size < (HASH_MAP_MAX_LOAD_FACTOR
                                       * HASH_MAP_INITIAL_CAP); i++)
    {
      Employee *emp = malloc(sizeof (Employee));
      emp->salary = 100000+i;
      emp->ID = 2198329-i;
      emp->name =(char**) "John Dow";
      char str[5] = "abc";
      strncat(str, &i, 1);
      pair *in_pair = create_pair(emp, str, EMPLOYEE,
                                  STRING);
      hashmap_insert(hash_map, in_pair);
      free(emp);
      pair_free((void **) &in_pair);
    }
}
/**
 * inserts and erases single key
 * @param hash_map
 */
void load_erase(hashmap *hash_map)
{
  Employee *emp = malloc(sizeof (Employee));
  emp->salary = 200000;
  emp->ID = 2198329;
  emp->name =(char**) "John Dow";
  char *value = "asjkjkds";
  pair *pr = create_pair(emp, &value, EMPLOYEE,
                         STRING);
  hashmap_insert(hash_map, pr);
  assert(hashmap_erase(hash_map, pr->key)==ONE);
  assert(hash_map->capacity == HASH_MAP_GROWTH_FACTOR*HASH_MAP_INITIAL_CAP);
  assert(hash_map->size == HASH_MAP_INITIAL_CAP*HASH_MAP_MAX_LOAD_FACTOR);
  free(emp);
  pair_free((void **) &pr);
}
/**
 * erases initial employee keys from map
 * @param hash_map
 * @param round int what round it is working on
 */
void erase_init_employee_string (hashmap *hash_map, int round)
{
  size_t cap = HASH_MAP_INITIAL_CAP;
  if (round == 2)
    {
      cap *= HASH_MAP_GROWTH_FACTOR;
    }
  for (char i = ONE; i-ONE < (HASH_MAP_MAX_LOAD_FACTOR
                              * HASH_MAP_INITIAL_CAP); i++)
    {
      if (i==9 || i==11 || i==12)
        {
          cap/=2;
        }
      if (i==5 && round == 2)
        {
          cap/=2;
        }
      Employee *emp = malloc(sizeof (Employee));
      emp->salary = 100000+i;
      emp->ID = 2198329-i;
      emp->name =(char**) "John Dow";
      char str[5] = "abc";
      pair *pr = create_pair(emp,strncat(str, &i, 1), EMPLOYEE,
                             STRING);
      assert(hashmap_erase(hash_map, pr->key)==ONE);
      assert(hash_map->capacity == cap);
      assert(hash_map->size == (HASH_MAP_MAX_LOAD_FACTOR
                                * HASH_MAP_INITIAL_CAP - i));
      free(emp);
      pair_free((void **) &pr);
    }
}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase(void)
{
  hashmap *hash_map = hashmap_alloc(hash_employee);
  check_erase_inputs(hash_map);
  loading_init_employee_string(hash_map);
  erase_init_employee_string(hash_map, 1);
  loading_init_employee_string(hash_map);
  load_erase(hash_map);
  erase_init_employee_string(hash_map, 2);
  hashmap_free(&hash_map);
}
/**
 * loads hashmap with string keys and char values and checks load factor
 * @param hash_map
 */
void loading_get_load_string_char (hashmap *hash_map, int round)
{
  int cap = HASH_MAP_INITIAL_CAP;
  if (round == 2)
    {
      cap = 2;
    }
  for (char i = ONE; hash_map->size < (HASH_MAP_MAX_LOAD_FACTOR
                                       * HASH_MAP_INITIAL_CAP); i++)
    {
      char str[5] = "abc";
      char val = 'b';
      if (round == 2 && (i == 2 || i == 4 || i == 7))
        {
          cap *=2;
        }
      strncat(str, &i, 1);
      pair *new_pair = create_pair((const_keyT ) &str,
                                   &val, STRING, CHAR);
      hashmap_insert(hash_map, new_pair);
      assert(hashmap_get_load_factor(hash_map) ==
                                               (double)i/cap);
      pair_free((void **) &new_pair);
    }
}
/**
 * erases keys and checks load factor
 * @param hash_map
 * @param round
 */
void erase_get_load (hashmap *hash_map, int round)
{
  int cap = HASH_MAP_INITIAL_CAP;
  if (round == 2)
    {
      cap *= HASH_MAP_GROWTH_FACTOR;
    }
  for (char i = ONE; i-ONE < (HASH_MAP_MAX_LOAD_FACTOR
                              * HASH_MAP_INITIAL_CAP); i++)
    {
      if (i==9 || i==11 || i==12)
        {
          cap/=2;
        }
      if (i==5 && round == 2)
        {
          cap/=2;
        }
      char str[5] = "abc";
      char val = 'b';
      pair *pr = create_pair(strncat(str, &i, 1),&val, STRING,
                             CHAR);
      hashmap_erase(hash_map, pr->key);
      assert(hashmap_get_load_factor(hash_map) == ((HASH_MAP_MAX_LOAD_FACTOR
                                                    * HASH_MAP_INITIAL_CAP)-i)/
                                                  cap);
      pair_free((void **) &pr);
    }
}
/**
 * inserts and erases single element and checks load factor after resize
 * @param hash_map
 */
void load_erase_get_load(hashmap *hash_map)
{
  double size = hash_map->size + 1;
  double capacity = hash_map->capacity*HASH_MAP_GROWTH_FACTOR;
  double loading = size/capacity;
  pair *pr = create_pair((const_keyT ) "asndjs", "b" ,STRING, CHAR);
  hashmap_insert(hash_map, pr);
  assert(hashmap_get_load_factor(hash_map) == loading);
  hashmap_erase(hash_map, pr->key);
  size--;
  loading = size/capacity;
  assert(hashmap_get_load_factor(hash_map) == loading);
  pair_free((void **) &pr);
}
/**
 * This function checks the hashmap_get_load_factor function of the hashmap
 * library. If hashmap_get_load_factor fails at some points, the functions
 * exits with exit code 1.
 */
void test_hash_map_get_load_factor(void)
{
  hashmap *hash_map = hashmap_alloc(hash_string);
  assert(hashmap_get_load_factor(NULL) == NEGATIVE);
  loading_get_load_string_char(hash_map, 1);
  erase_get_load(hash_map, 1);
  loading_get_load_string_char(hash_map, 2);
  load_erase_get_load(hash_map);
  erase_get_load(hash_map, 2);
  hashmap_free(&hash_map);
}
/**
 * check bad inputs of apply if function
 * @param hash_map
 */
void apply_if_input_check (const hashmap *hash_map)
{
  assert(hashmap_apply_if(NULL, is_abc, square_value) == NEGATIVE);
  assert(hashmap_apply_if(hash_map, NULL, square_value) == NEGATIVE);
  assert(hashmap_apply_if(hash_map, is_abc, NULL) == NEGATIVE);
  assert(hashmap_apply_if(NULL, NULL, NULL) == NEGATIVE);
  assert(hashmap_apply_if(NULL, is_abc, NULL) == NEGATIVE);
  assert(hashmap_apply_if(NULL, NULL, square_value) == NEGATIVE);
  assert(hashmap_apply_if(hash_map, NULL, NULL) == NEGATIVE);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with
 * exit code 1.
 */
void test_hash_map_apply_if()
{
  hashmap *hash_map = hashmap_alloc(hash_char);
  loading_init_char_int(hash_map);
  apply_if_input_check(hash_map);
  assert(hashmap_apply_if(hash_map, is_abc, square_value) == 12);
  assert(hashmap_apply_if(hash_map, is_digit, double_value) == 0);
  hashmap_free(&hash_map);
}
