#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definitions

#ifdef __linux__
#include <linux/limits.h>
#elif defined(_WIN32)
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <sys/syslimits.h>
#endif

/// implementation of c23's unreachable()
/// https://en.cppreference.com/w/c/program/unreachable
#ifndef unreachable
#ifdef __GNUC__ // GCC, Clang, ICC

#define unreachable() (__builtin_unreachable())

#elifdef _MSC_VER // MSVC

#define unreachable() (__assume(false))

#else
// Even if no extension is used, undefined behavior is still raised by
// the empty function body and the noreturn attribute.

// The external definition of unreachable_impl must be emitted in a separated TU
// due to the rule for inline functions in C.

[[noreturn]] inline void unreachable_impl() {}
#define unreachable() (unreachable_impl())

#endif
#endif /* ifndef unreachable */

// constants
#define FIRST_NAME_LIM 25
#define LAST_NAME_LIM 50

/// general purpose string type
typedef struct {
  char *ptr;
  size_t len;
  size_t lim;
} string;

/// Employee linked list node type
struct EMPLOYEE {
  uint16_t mat;
  uint32_t salary;
  string *namef;
  string *namel;
  struct {
    uint8_t code;
    uint8_t rate;
  } region;
};

typedef struct EMPLOYEE EMPLOYEE;

/// hash table main type
typedef struct {
  size_t key;
  EMPLOYEE *val;
} hash_table;

/* NOTE: general plumbing functions see "utils.c" */
void check_alloc(void *ptr);
int check_string(char *buffer);
void panic(const char *msg);
string *allocate_string(size_t size);
void destruct_string(string *str);
int cmp(int8_t a, int8_t b);
size_t min(size_t a, size_t b);

/* NOTE: employee base functions see "employee.c" */
EMPLOYEE *create_employee(void);
void destruct_employee(EMPLOYEE *poor_soul);
void show_employee(EMPLOYEE *node);

/* NOTE: disk <=> memory interchange see "disk.c" */
void initlist(const char *path_to_disk_state);
void dumplist(const char *path_to_disk_state);

/* NOTE: search functions see "search.c" */
size_t lev_dist(string *a, string *b);

// vim: ft=c
