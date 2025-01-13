#!/bin/bash

DUMP_FILE="database_dump.sql"
OUTPUT_DB="new_database.db"

if [ -f "$DUMP_FILE" ]; then
    echo "Recreating database from $DUMP_FILE..."
    sqlite3 $OUTPUT_DB < $DUMP_FILE
    echo "Database recreated: $OUTPUT_DB"
else
    echo "Error: $DUMP_FILE not found!"
    exit 1
fi

