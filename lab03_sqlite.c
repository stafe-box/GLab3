#include <sqlite3.h>
#include <stdio.h>
#include "lab03_sqlite.h"
#include <string.h>
#include <stdlib.h>

int callback(void *, int, char **, char **);
void sqlite_get_data()
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql = "SELECT \
	                lecturers.lecturer_id, \
                    lecturers.lecturer_name, \
                    disciplines.discipline_id, \
                    disciplines.discipline_name, \
                    disciplines.lectures, \
                    disciplines.practices, \
                    disciplines.laboratory, \
                    control.control_id, \
                    control.control_name \
                 FROM \
                    lecturers JOIN lecturers_to_disciplines ON \
                    lecturers_to_disciplines.lecturer_id = lecturers.lecturer_id JOIN disciplines ON \
                    lecturers_to_disciplines.discipline_id = disciplines.discipline_id JOIN control_to_disciplines ON \
                    control_to_disciplines.discipline_id = disciplines.discipline_id JOIN control ON \
                    control.control_id = control_to_disciplines.control_id;";
    rc = sqlite3_exec(db, sql, callback, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}

void sqlite_update(int compid, char *compname, float price)
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }

    char *sql_comp_temp = "UPDATE components\
                              SET \
                              name = '%s', \
                              price = %d.%d \
                          WHERE \
                              component_id = %d;";
    char sql_comp[255];
    sprintf(sql_comp, sql_comp_temp, compname, (int)price, ((int)(price * 100)) % 100, compid);
    rc = sqlite3_exec(db, sql_comp, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to update data in components\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}


void sqlite_delete(int lecid, int disid, int conid)
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    printf("*\n");
    char *sql_comp_temp = "DELETE FROM lecturers_to_disciplines\
                              WHERE \
                              lecturer_id = %d AND discipline_id = %d;\
                           DELETE FROM control_to_disciplines\
                              WHERE \
                              control_id = %d AND discipline_id = %d;";

    printf("*\n");
    char sql_comp[511];
    printf("*\n");
    sprintf(sql_comp, sql_comp_temp, lecid, disid, conid, disid);
    printf("*\n");
    rc = sqlite3_exec(db, sql_comp, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to delete data in components\n");
        fprintf(stderr, "SQLite error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
    }
    sqlite3_close(db);
}

int TRY;
int callback_e(void *not_used, int argc, char **argv, char **col_names)
{
    if (argc == 1)
        TRY = atoi(argv[0]);  
    else
        TRY = 0;
    return (TRY);
    
}//можно заколхозить перекрёзсный вызов функций, так как получить данные нынешним способом не выйдет

void sqlite_add(char *shopname, char *compname, float price)//последняя переменная ^^^
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    if (SQLITE_OK != (rc = sqlite3_open(DB_FILE, &db)))
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    char *sql_comp_temp_find_shop = "SELECT shop_id\
                                     FROM shops\
                                     WHERE name = '%s';";
    char sql_comp[255];
    sprintf(sql_comp, sql_comp_temp_find_shop, shopname);
    rc = sqlite3_exec(db, sql_comp, callback_e, NULL, &err_msg);//пытаемя получить индекс магазина
    //если магазина нет возвращется 0, иначе индекс магазина
    if (TRY == 0)//если магазина нет
    {
        char *sql_comp_temp_add_shop = "INSERT INTO shops (name)\
                                        VALUES ('%s');";
        char sql_comp2[255];
        sprintf(sql_comp2, sql_comp_temp_add_shop, shopname);
        rc = sqlite3_exec(db, sql_comp2, NULL, NULL, &err_msg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Failed to update data in components\n");
            fprintf(stderr, "SQLite error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
        }
        sqlite_add(shopname, compname, price);
    }
    else//если магазин есть, а он будет
    {
        char *sql_comp_temp_add_comp = "INSERT INTO components (name, price, shop_id)\
                                        VALUES ('%s', %d.%d, %d);";
        char sql_comp3[255];
        sprintf(sql_comp3, sql_comp_temp_add_comp, compname, (int)price, 
                                        ((int)(price * 100)) % 100, TRY);
        rc = sqlite3_exec(db, sql_comp3, NULL, NULL, &err_msg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Failed to update data in components\n");
            fprintf(stderr, "SQLite error: %s\n", err_msg);
            sqlite3_free(err_msg);
            sqlite3_close(db);
        }
    }
    sqlite3_close(db);
}