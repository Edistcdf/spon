#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include "rw.h"

int db_close(sqlite3* db_)
{
    sqlite3_close(db_);

    return 0;  
}

int db_open(const char* name, sqlite3** db_)
{
    int rc = sqlite3_open(name, db_);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db_));
        sqlite3_close(*db_);
        return 1;
    }

    return 0;  
}

int db_create_table(sqlite3* db, const char* sql){
    char *err_msg = 0;
    int rc;

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Table created successfully\n");
    }

    return 0;
}

int identifier_exists(sqlite3 *db, const char *identifier) {
    const char *sql = "SELECT 1 FROM Results WHERE identifier = ? LIMIT 1;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;  
    }

    // Prepare stmt
    sqlite3_bind_text(stmt, 1, identifier, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    int exists = (rc == SQLITE_ROW);

    sqlite3_finalize(stmt);

    return exists;
}

int increment_failed(sqlite3 *db, const char *identifier) {
    const char *sql = 
        "UPDATE Results "
        "SET failed = failed + 1 "
        "WHERE identifier = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, identifier, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Incremented failed for identifier '%s'\n", identifier);
    }

    sqlite3_finalize(stmt);
    return rc;
}

int increment_success(sqlite3 *db, const char *identifier) {
    const char *sql = 
        "UPDATE Results "
        "SET success = success + 1 "
        "WHERE identifier = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, identifier, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Incremented success for identifier '%s'\n", identifier);
    }

    sqlite3_finalize(stmt);
    return rc;
}

int add_or_update_row(sqlite3 *db, const char *identifier, int success, int failed) {
    const char *sql = 
        "INSERT INTO Results (identifier, success, failed) VALUES (?, ?, ?) "
        "ON CONFLICT(identifier) DO UPDATE SET success = excluded.success, failed = excluded.failed;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, identifier, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, success);
    sqlite3_bind_int(stmt, 3, failed);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    } else {
        printf("Row added or updated successfully\n");
    }

    sqlite3_finalize(stmt);
    return rc;
}

int read_rows_with_identifier(sqlite3 *db, const char *identifier, struct connection_result_table* result) {
    const char *sql = "SELECT identifier, success, failed FROM Results WHERE identifier = ?";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return rc;
    }

    sqlite3_bind_text(stmt, 1, identifier, -1, SQLITE_STATIC);

    printf("Results matching identifier '%s':\n", identifier);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *id = sqlite3_column_text(stmt, 0);
        memcpy(result->identifier, id, strlen((char *)id));
        int success = sqlite3_column_int(stmt, 1);
        result->success = success;

        int failed = sqlite3_column_int(stmt, 2);
        result->failed = failed;

        printf("Identifier: %s, Success: %d, Failed: %d\n", id, success, failed);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    return SQLITE_OK;
}
