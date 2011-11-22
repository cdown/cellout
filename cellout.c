#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64
#define BATTERY_INFO_PATH "/sys/class/power_supply/"
#define CHARGE_NOW_FILENAME "charge_now"
#define CHARGE_FULL_FILENAME "charge_full"
#define STATUS_FILENAME "status"

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
    FILE *f_charge_full,
         *f_charge_now,
         *f_status;

    if (argc < 2)
    {
        fputs("Usage: cellout [-p] battery\n", stderr);
        exit(1);
    }

    battery_arg = strncmp(argv[1], "-p", 3) ? 1 : 2;

    strcpy(charge_full_filename, BATTERY_INFO_PATH);
    strcpy(charge_now_filename, BATTERY_INFO_PATH);
    strcpy(status_filename, BATTERY_INFO_PATH);
    strncat(charge_full_filename, argv[battery_arg], BUFFER_SIZE -
            sizeof(BATTERY_INFO_PATH) - sizeof(CHARGE_FULL_FILENAME));
    strncat(charge_now_filename, argv[battery_arg], BUFFER_SIZE -
            sizeof(BATTERY_INFO_PATH) - sizeof(CHARGE_NOW_FILENAME));
    strncat(status_filename, argv[battery_arg], BUFFER_SIZE -
            sizeof(BATTERY_INFO_PATH) - sizeof(STATUS_FILENAME));
    strcat(charge_full_filename, "/"CHARGE_FULL_FILENAME);
    strcat(charge_now_filename, "/"CHARGE_NOW_FILENAME);
    strcat(status_filename, "/"STATUS_FILENAME);

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

    /* TODO: atoi() error checking (should be fine since /proc is not directly
     * editable (and thus should prevent stupid stuff), but nevertheless...
     */
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
        printf("%d%c\n", charge_percentage, status);

    /* We don't want to exit on failure to close. We don't write anything, so it
     * should be fine, but we should still bring it to the attention of the
     * user.
     *
     * XXX: EXIT_SUCCESS is returned regardless of whether we successfully close
     * the files or not. I'm not sure whether I really want to return failure
     * for something that (shouldn't) cause any problems on error, but it should
     * be such a rare occurrence that we don't really have to worry about it for
     * now.
     */
    if (fclose(f_status) == EOF)
        perror(status_filename);
    if (fclose(f_charge_now) == EOF)
        perror(charge_now_filename);
    if (fclose(f_charge_full) == EOF)
        perror(charge_full_filename);

    return EXIT_SUCCESS;
}
