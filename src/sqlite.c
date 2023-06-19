#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <sqlite3.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

#include "all.h"

void force_init(char *path_to_disk_state) {
  sqlite3 *db;
  char *stmt =
      "DROP TABLE IF EXISTS EMPLOYEES;"
      "CREATE TABLE EMPLOYEES(id INTEGER PRIMARY KEY, salary INT, first TEXT, "
      "last TEXT, code INT, rate INT)";
  int rc =
      sqlite3_open_v2(path_to_disk_state, &db, SQLITE_OPEN_READWRITE, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  char *err_msg = 0;
  rc = sqlite3_exec(db, stmt, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    panic(nullptr);
  }
  sqlite3_close(db);
}

size_t get_count(char *path_to_disk_state) {
  sqlite3 *db;
  size_t count = 0;
  char *stmt_txt = "SELECT COUNT(*) FROM Users";
  sqlite3_stmt *stmt;
  int rc = sqlite3_open_v2(path_to_disk_state, &db, SQLITE_OPEN_READONLY, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "cannot open database: %s", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  sqlite3_prepare_v2(db, stmt_txt, (int)strlen(stmt_txt), &stmt, NULL);
  if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    count = (size_t)sqlite3_column_int(stmt, 0);
  }
  rc = sqlite3_finalize(stmt);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    printf("Error encountered while checking user count!");
    exit(1);
  }
  sqlite3_close(db);
  return count;
}

void load_all(char *path_to_disk_state) {
  sqlite3 *db;
  char *stmt_txt = "SELECT * FROM EMPLOYEES";
  sqlite3_stmt *stmt;

  int rc = sqlite3_open_v2(path_to_disk_state, &db, SQLITE_OPEN_READONLY, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "cannot open database: %s", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  sqlite3_prepare_v2(db, stmt_txt, (int)strlen(stmt_txt), &stmt, NULL);

  size_t max_n = table_len =
      (table_len == 0) ? get_count(path_to_disk_state) : table_len;

  for (size_t i = 0; i < max_n; ++i) {
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
      global_table[i].val->mat = (uint16_t)sqlite3_column_int(stmt, 0);
      global_table[i].val->salary = (uint16_t)sqlite3_column_int(stmt, 1);
      stringcpy(global_table[i].val->namef, sqlite3_column_text(stmt, 2));
      stringcpy(global_table[i].val->namel, sqlite3_column_text(stmt, 3));
      global_table[i].val->region.code = (uint8_t)sqlite3_column_int(stmt, 4);
      global_table[i].val->region.rate = (uint8_t)sqlite3_column_int(stmt, 5);
      global_table[i].key = get_hash(*global_table[i].val);
    } else {
      panic("out of range\n");
    }
  }
  rc = sqlite3_finalize(stmt);
  if (rc != SQLITE_OK) {
    sqlite3_close(db);
    panic("error encountered while loading values from the datastore");
  }
  sqlite3_close(db);
}

void dump_all(char *path_to_disk_state) {
  fputs("clearing the datastore...\n", stderr);
  force_init(path_to_disk_state);
  fputs("serializing all data to disk...\n", stderr);
  sqlite3 *db;
  char *stmt_txt = nullptr;
  // sqlite3_stmt *stmt;
  int rc =
      sqlite3_open_v2(path_to_disk_state, &db, SQLITE_OPEN_READWRITE, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "couldn't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  char *err_msg = nullptr;
  size_t max_n = table_len;

  for (size_t i = 0; i < max_n; ++i) {
    asprintf(&stmt_txt,
             "INSERT INTO EMPLOYEES (salary , first , last , code , rate ), "
             "VALUES (%d, '%s', '%s', %d, %d)",
             global_table[i].val->salary, global_table[i].val->namef->ptr,
             global_table[i].val->namel->ptr, global_table[i].val->region.code,
             global_table[i].val->region.rate);
    rc = sqlite3_exec(db, stmt_txt, 0, 0, &err_msg);
    // rc = sqlite3_finalize(stmt);
    if (rc != SQLITE_OK) {
      fprintf(stderr, "SQL error: %s\n", err_msg);
      sqlite3_close(db);
    }
    free(stmt_txt);
  }

  sqlite3_close(db);
}

void modify_entry(char *path_to_disk_state, EMPLOYEE entry) {}
