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