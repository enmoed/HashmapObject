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

#ifndef HASHFUNCS_H_
#define HASHFUNCS_H_

#include <stdlib.h>
#include <string.h>

#define ZERO 0


/**
 * Integers simple hash func.
 */
size_t hash_int(const void *elem){
    size_t hash = *((int *) elem);
    return hash;
}

/**
 * Longs simple hash func.
 */
size_t hash_long(const void *elem){
  size_t hash = (long) elem;
  return hash;
}

/**
 * Chars simple hash func.
 */
size_t hash_char(const void *elem){
    size_t hash = *((unsigned char *) elem);
    return hash;
}

/**
 * String simple hash func.
 */

size_t hash_string(const void *elem){
  char *str = ((char *) elem);
  size_t hash = ZERO;
  for (int i=ZERO; i<(int)strlen(str); i++)
    {
      hash += (size_t) str[i];
    }
  return hash;
}

/**
 * Employee simple hash func.
 */
size_t hash_employee(const void *elem)
{
  Employee *new_employee = ((Employee *) elem);
  return (hash_long((const void *)new_employee->ID) + hash_long((const void *)
  new_employee->salary) + hash_string((const void *) new_employee->name));
}

#endif // HASHFUNCS_H_