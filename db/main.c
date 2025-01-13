#include <sqlite3.h>
#include <stdio.h>

int main() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc;

    rc = sqlite3_open("db.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS Results ("
                      "identifier TEXT PRIMARY KEY CHECK (length(identifier) <= 70), "
                      "success INTEGER CHECK (success >= 0), "
                      "failed INTEGER CHECK (failed >= 0));";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Table created successfully\n");
    }

    sqlite3_close(db);

    return 0;
}
