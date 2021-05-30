#ifndef EMPLOYEE_
#define EMPLOYEE_
/**
 * Employee struct
 */
typedef struct Employee{
    long ID;
    char** name;
    long salary;
} Employee;
#endif //EMPLOYEE_

#ifndef TEST_PAIRS_H
#define TEST_PAIRS_H

#include <stdlib.h>
#include "pair.h"

/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return (*(char *) key_1 == *(char *) key_2);
}

/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT* key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Copies the int key of the pair.
 */
void *int_key_cpy (const_keyT key)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) key);
  return new_int;
}

/**
 * Compares the int key of the pair.
 */
int int_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(int *) key_1 == *(int *) key_2;
}

/**
 * Frees the int key of the pair.
 */
void int_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Copies the string key of the pair.
 */
void *string_key_cpy (const_keyT key)
{
  char **new_string = malloc (sizeof (char*));
  *new_string = *((char **) key);
  return new_string;
}

/**
 * Compares the string key of the pair.
 */
int string_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

/**
 * Frees the string key of the pair.
 */
void string_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}


/**
 * Copies the char value of the pair.
 */
void *char_value_cpy (const_valueT value)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) value);
  return new_char;
}

/**
 * Compares the char value of the pair.
 */
int char_value_cmp (const_valueT value_1, const_valueT value_2)
{
  return *(char *) value_1 == *(char *) value_2;
}

/**
 * Frees the char value of the pair.
 */
void char_value_free (valueT* value)
{
  if (value && *value)
    {
      free (*value);
      *value = NULL;
    }
}

/**
 * Copies the int value of the pair.
 */
void *int_value_cpy (const_valueT value)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int*) value);
  return new_int;
}

/**
 * Compares the int value of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}


/**
 * Copies the string value of the pair.
 */
void *string_value_cpy (const_valueT value)
{
  char **new_string = malloc (sizeof (char*));
  *new_string = *((char **) value);
  return new_string;
}

/**
 * Compares the string value of the pair.
 */
int string_value_cmp (const_valueT value_1, const_valueT value_2)
{
  return *(char **) value_1 == *(char **) value_2;
}

/**
 * Frees the string value of the pair.
 */
void string_value_free (valueT *value)
{
  if (value && *value)
    {
      free (*value);
      *value = NULL;
    }
}

/**
 * Copies the employee key of the pair.
 */
void *employee_key_cpy (const_keyT key)
{
  Employee *new_employee = malloc (sizeof (Employee));
  *new_employee = *((Employee *) key);
  return new_employee;
}

/**
 * Compares the employee key of the pair.
 */
int employee_key_cmp (const_keyT key_1, const_keyT key_2)
{
  Employee *employee_1 = (Employee *)key_1;
  Employee *employee_2 = (Employee *)key_2;
  return (employee_1->ID == employee_2->ID &&
  employee_1->salary == employee_2->salary &&
  string_value_cmp(employee_1->name, employee_2->name));
}

/**
 * Frees the employee key of the pair.
 */
void employee_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a digit, else - 0
 */
int is_digit (const_keyT elem)
{
  char c = *((char *) elem);
  return (c > 47 && c < 58);
}

/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
  *((int *) elem) *= 2;
}

/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a letter, else - 0
 */
int is_abc (const_keyT elem)
{
  char c = *((char *) elem);
  return (c >= 65 && c <= 122);
}

/**
 * returns the value pointed to by the given pointer, squared
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void square_value (valueT elem)
{
  *((int *) elem) *= *((int *) elem);
}

#endif //TEST_PAIRS_H
