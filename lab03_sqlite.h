#ifndef LAB03_SQLITE_H
#define LAB03_SQLITE_H

#define DB_FILE "sqlab3.db"
void sqlite_get_data();
void sqlite_update(int compid, char *compname, float price);
void sqlite_delete_cd(int disid, int conid);
void sqlite_delete_ld(int lecid, int disid);

#endif
