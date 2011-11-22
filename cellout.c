#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BATTERY_INFO_PATH "/sys/class/power_supply/"
#define BUFFER_SIZE 64

int main(int argc, char *argv[]) {
    char status_filename[BUFFER_SIZE],
         charge_now_filename[BUFFER_SIZE],
         charge_full_filename[BUFFER_SIZE],
         charge_now[BUFFER_SIZE],
         charge_full[BUFFER_SIZE];
    FILE *f_status,
         *f_charge_now,
         *f_charge_full;
    
    if ( argc != 2 ) {
        fputs("Usage: cellout battery\n", stderr);
        exit(1);
    }

    strcpy(status_filename, BATTERY_INFO_PATH);
    strcpy(charge_now_filename, BATTERY_INFO_PATH);
    strcpy(charge_full_filename, BATTERY_INFO_PATH);
    strncat(status_filename, argv[1], BUFFER_SIZE - 32);
    strncat(charge_now_filename, argv[1], BUFFER_SIZE - 36);
    strncat(charge_full_filename, argv[1], BUFFER_SIZE - 37);
    strcat(status_filename, "/status\0");
    strcat(charge_now_filename, "/charge_now\0");
    strcat(charge_full_filename, "/charge_full\0");
    
    f_status = fopen(status_filename, "r");
    f_charge_now = fopen(charge_now_filename, "r");
    f_charge_full = fopen(charge_full_filename, "r");

    if (!f_status) {
        fprintf(stderr, "Failed to open %s\n", status_filename);
        exit(2);
    }
    if (!f_charge_now) {
        fprintf(stderr, "Failed to open %s\n", charge_now_filename);
        exit(2);
    }
    if (!f_charge_full) {
        fprintf(stderr, "Failed to open %s\n", charge_full_filename);
        exit(2);
    }

    fgets(charge_now, BUFFER_SIZE, f_charge_now);
    charge_now[strlen(charge_now)-1] = '\0';

    fgets(charge_full, BUFFER_SIZE, f_charge_full);
    charge_full[strlen(charge_full)-1] = '\0';

    printf("%d%c\n",
           atoi(charge_now) * 100 / atoi(charge_full), fgetc(f_status));

    fclose(f_status);
    fclose(f_charge_now);
    fclose(f_charge_full);

    return 0;
}
