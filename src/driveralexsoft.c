#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <modbus.h>

#include <sqlite3.h>

#define ADDRESS_START   1450
#define ADDRESS_END     1500

int main(void)
{
    char *err_msg = 0;
    printf("SQLite3 version: %s\n", sqlite3_libversion()); 

    sqlite3 *db;
    sqlite3_stmt *res;

    int rcd = sqlite3_open("driverdev.db", &db);

    if(rcd != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    

    rcd = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);

    if(rcd != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    } 

    rcd = sqlite3_step(res);

    if(rcd == SQLITE_ROW) 
    {
        printf("%s\n", sqlite3_column_text(res, 0));
    }

    char *sql = "CREATE TABLE TimeRef(Idscrap INT, TableTime TEXT);"
                "INSERT INTO TimeRef VALUES(0, '3/9/2020  1:00:00 AM');"
                "INSERT INTO TimeRef VALUES(0, '3/9/2020  1:00:00 AM');";

    rcd = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rcd != SQLITE_OK )
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return -1;
    }


    sqlite3_finalize(res);
    sqlite3_close(db);

    

    modbus_t *ctx;
    int rc;
    int nb;
    uint16_t *tab_rp_registers;

    /* TCP Connection */
    ctx = modbus_new_tcp("10.0.40.1", 502);
    modbus_set_debug(ctx, FALSE);
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    /* Allocate and initialize memory spaces */
    nb = ADDRESS_END - ADDRESS_START;
    if(nb>125)
    {
        printf("Your demanding to many address (Max = 125)\n");
    }
    
    tab_rp_registers = (uint16_t *) malloc(nb * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb * sizeof(uint16_t));


    printf("Before\n");
    rc = modbus_read_registers(ctx, ADDRESS_START, nb, tab_rp_registers);
    if (rc != nb)
    {
        printf("ERROR modbus_read_registers (%d)\n", rc);
        printf("Address = %d, nb = %d\n", ADDRESS_START, nb);
    }
    int c = sizeof(tab_rp_registers);
    printf("Value de c=%d\n", c);
    for(int i = 0;i<nb+2;++i )
    {
        printf("Ici :tab_rp_registers[%d] = %d\n",(i+ADDRESS_START),tab_rp_registers[i]);
    }
 
    printf("%d Return value\n", rc);

    free(tab_rp_registers);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    
    while(1)
    {
        sleep(1000);
        printf("loop");
    }
    printf("bye");
    
    return 0;
}

