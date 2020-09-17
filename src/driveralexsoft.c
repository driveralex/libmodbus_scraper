#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <errno.h>



#include <time.h>
#include <sys/time.h>  

#include <modbus.h>

#include <sqlite3.h>

#define ADDRESS_START   1450
#define ADDRESS_END     1500



long long int drivetime(void)
{
    /* Example of timestamp in microsecond. */
    struct timeval timer_usec; 
    long long int timestamp_usec; /* timestamp in microsecond */
    if (!gettimeofday(&timer_usec, NULL)) 
    {
        timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll + (long long int) timer_usec.tv_usec;
    }
    else 
    {
        timestamp_usec = -1;
    }
    // printf("%lld microseconds since epoch\n", timestamp_usec);
    
    
    return timestamp_usec;
}

char * stringcreate(char *a,char *b )
{
    char *tmp = (char * ) malloc(1+ strlen(a)+strlen(b));
    strcpy(tmp,a);
    strcat(tmp,b);
    return tmp;
}

int main(void)
{
    char outputtime[1024];

    sprintf(outputtime,"%lld",drivetime());

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

    char *sql = "CREATE TABLE TimeRef(Idscrap INT, TableTimestamp TEXT);";
                "INSERT INTO TimeRef VALUES(0,'1523115');";
               
    char *timerefinsert = "INSERT INTO TimeRef VALUES(1,'";
    char *endins = "');";
    char *concat1 = (char * ) malloc(1+ strlen(timerefinsert)+strlen(outputtime));
    
    /*strcpy(concat1,timerefinsert);
    strcat(concat1,outputtime);
    printf("\nString 1:\n%s\n",concat1);*/
    
    
    concat1 = stringcreate(timerefinsert,outputtime);

    
    //char *concat2 = (char * ) malloc(1+ strlen(concat1)+strlen(endins));
    char *concat2 ;
    concat2 = stringcreate(stringcreate(timerefinsert,outputtime),endins);

    printf("\nString 2:\n%s\n",concat2);

    /*
    printf("Before");
    printf("%s",concat2);
    printf("After");
    */

    /*Faire une fonction d'ajout des 2 parm vers un tableau ( la requette sql )*/


    /* Creation de la table frogien */ 
    char *newrowscraptable = "CREATE TABLE tab_";
    char *rawtabletimereftemp = (char * ) malloc(1+ strlen(newrowscraptable)+strlen(outputtime));
    strcpy(rawtabletimereftemp,newrowscraptable);
    strcat(rawtabletimereftemp,outputtime);

    char *inside1 = "(Modbusaddr INT, Value INT);";
    char *rawcreatfinal = (char * ) malloc(1+ strlen(rawtabletimereftemp)+strlen(inside1));
    strcpy(rawcreatfinal,rawtabletimereftemp);
    strcat(rawcreatfinal,inside1);
    /*printf("Before");
    printf("%s",rawcreatfinal);
    printf("After");*/


    char *insertforgeintable = "INSERT INTO tab_";
    char *insertforgeintablertmp = (char * ) malloc(1+ strlen(insertforgeintable)+strlen(inside1));
    strcpy(insertforgeintablertmp,insertforgeintable);
    strcat(insertforgeintablertmp,outputtime);

    

    printf("Before");
    printf("%s",insertforgeintablertmp);
    printf("After");
    printf("Beforefunct");
    printf("%s",stringcreate(insertforgeintable,outputtime));
    printf("After");




    rcd = sqlite3_exec(db, rawcreatfinal, 0, 0, &err_msg);
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

