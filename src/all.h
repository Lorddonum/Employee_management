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

/// Employee node type
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

/// search mode
typedef enum {
  exact,
  lev,
  fuzz,
} search_mode;

//---------
// globals
//---------
extern hash_table *global_table;
extern size_t table_len;

/* NOTE: general plumbing functions see "utils.c" */
void check_alloc(void *ptr);
int check_string(char *buffer);
string *allocate_string(size_t size);
void stringdup(string *dest, const unsigned char *src);
void stringcpy(string *dest, string *src);
void getstring(string *arg);
void destruct_string(string *str);
int cmp(int8_t a, int8_t b);
size_t min(size_t a, size_t b);
[[noreturn]] void panic(const char *msg);

/* NOTE: employee && hash table functions see "employee.c" and "table.c" */
EMPLOYEE *create_employee(void);
void destruct_employee(EMPLOYEE *poor_soul);
void add_employee(char *path_to_disk_state, EMPLOYEE *unit);
void copy_employee(EMPLOYEE *dest, EMPLOYEE *src);
void show_employee(EMPLOYEE *node);
void list_employees(void);
size_t get_hash(EMPLOYEE emp);
hash_table *allocate_hashtable(size_t n_elem);
hash_table *reallocate_hashtable(size_t n_elem);

EMPLOYEE *search_by_id(uint16_t key);
void search_wrapper(search_mode set);

/* NOTE: datastore memory intercom see "sqlite.c" */
void force_init(char *path_to_disk_state);
void load_all(char *path_to_disk_state);
void dump_all(char *path_to_disk_state);
void add_entry(char *path_to_disk_state, EMPLOYEE *entry);
size_t get_count(char *path_to_disk_state);

/* NOTE: search functions see "search.c" */
size_t lev_dist(string *a, string *b);

/* NOTE: shell functions see "shell.c" */
int shell_loop(char *path_to_disk_state);

/* NOTE: gui functions see "gui.c" */
void DrawEmployeeData(int numEmployees, int startRow, int numRows);

// vim: ft=c
