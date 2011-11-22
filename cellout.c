#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BATTERY_INFO_PATH "/sys/class/power_supply/"
#define BUFFER_SIZE 64

int main(int argc, char *argv[])
{
    char status_filename[BUFFER_SIZE],
         charge_now_filename[BUFFER_SIZE],
         charge_full_filename[BUFFER_SIZE],
         charge_now[BUFFER_SIZE],
         charge_full[BUFFER_SIZE],
         charge_percentage_graphic[5];
    int charge_percentage,
        status,
        battery_arg,
        i;
    FILE *f_status,
         *f_charge_now,
         *f_charge_full;

    if (argc < 2)
    {
        fputs("Usage: cellout [-p] battery\n", stderr);
        exit(1);
    }

    battery_arg = strncmp(argv[1], "-p", 3) ? 1 : 2;

    strcpy(status_filename, BATTERY_INFO_PATH);
    strcpy(charge_now_filename, BATTERY_INFO_PATH);
    strcpy(charge_full_filename, BATTERY_INFO_PATH);
    strncat(status_filename, argv[battery_arg], BUFFER_SIZE - 32);
    strncat(charge_now_filename, argv[battery_arg], BUFFER_SIZE - 36);
    strncat(charge_full_filename, argv[battery_arg], BUFFER_SIZE - 37);
    strcat(status_filename, "/status");
    strcat(charge_now_filename, "/charge_now");
    strcat(charge_full_filename, "/charge_full");

    if ((f_status = fopen(status_filename, "r")) == NULL)
    {
        fprintf(stderr, "Failed to open %s\n", status_filename);
        exit(2);
    }
    if ((f_charge_now = fopen(charge_now_filename, "r")) == NULL)
    {
        fprintf(stderr, "Failed to open %s\n", charge_now_filename);
        exit(2);
    }
    if ((f_charge_full = fopen(charge_full_filename, "r")) == NULL)
    {
        fprintf(stderr, "Failed to open %s\n", charge_full_filename);
        exit(2);
    }

    if (fgets(charge_now, BUFFER_SIZE, f_charge_now) == NULL)
        fprintf(stderr, "Failed to get line from %s\n", charge_now_filename);
    charge_now[strlen(charge_now)-1] = '\0';

    if (fgets(charge_full, BUFFER_SIZE, f_charge_full) == NULL)
        fprintf(stderr, "Failed to get line from %s\n", charge_full_filename);
    charge_full[strlen(charge_full)-1] = '\0';

    if ((status = fgetc(f_status)) == EOF)
        fprintf(stderr, "Failed to get character from %s\n", status_filename);

    charge_percentage = atoi(charge_now) * 100 / atoi(charge_full);

    if (battery_arg == 2)
    {
        charge_percentage_graphic[0] = '\0';
        for (i = 25; i <= 100; i += 25)
        {
            if (charge_percentage >= i)
                strcat(charge_percentage_graphic, "#");
            else
                strcat(charge_percentage_graphic, "-");
        }
        printf("[%s] %d%c\n", charge_percentage_graphic, charge_percentage,
               status);
    }
    else
    {
        printf("%d%c\n", charge_percentage, status);
    }

    if (fclose(f_status) == EOF)
        perror(status_filename);
    if (fclose(f_charge_now) == EOF)
        perror(charge_now_filename);
    if (fclose(f_charge_full) == EOF)
        perror(charge_full_filename);

    return 0;
}
