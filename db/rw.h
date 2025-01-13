#include <sqlite3.h>
#include <stdio.h>

#define CONNECTION_IDENTIFIER_MAX_LENGTH 70
struct connection_result_table {
    unsigned char identifier[CONNECTION_IDENTIFIER_MAX_LENGTH];
    int success;
    int failed;
};

int identifier_exists(sqlite3 *db, const char *identifier);
int increment_failed(sqlite3 *db, const char *identifier);
int increment_success(sqlite3 *db, const char *identifier);
int add_or_update_row(sqlite3 *db, const char *identifier, int success, int failed);
int read_rows_with_identifier(sqlite3 *db, const char *identifier, struct connection_result_table* result);
int db_create_table(sqlite3* db, const char* sql);
int db_open(const char* name, sqlite3** db_);
int db_close(sqlite3* db_);
