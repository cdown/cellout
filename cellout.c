/* (c) Christopher Down 2011
 * See the COPYING file for copyright information.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION 1.02
#define BUFFER_SIZE 64
#define BATTERY_INFO_PATH "/sys/class/power_supply/"
#define CHARGE_NOW_FILENAME "charge_now"
#define CHARGE_FULL_FILENAME "charge_full"
#define STATUS_FILENAME "status"

int main(int argc, char *argv[]) {
    char status_filename[BUFFER_SIZE],
         charge_now_filename[BUFFER_SIZE],
         charge_full_filename[BUFFER_SIZE],
         charge_now[BUFFER_SIZE],
         charge_full[BUFFER_SIZE],
         charge_percentage_graphic[5],
         battery[BUFFER_SIZE];
    int charge_percentage, status, i, show_graphic;
    FILE *f_charge_full, *f_charge_now, *f_status;

    if (argc == 3 && !strncmp(argv[1], "-g", 3)) {
        show_graphic = 1;
        strncpy(battery, argv[2], BUFFER_SIZE - 1);
    } else {
        if (argc == 2) {
            if (!strncmp(argv[1], "-v", 3)) {
                printf("%s %.02f\n", "mac-cel", VERSION);
                return 0;
            }
            strncpy(battery, argv[1], BUFFER_SIZE - 1);
        } else {
            fputs("Usage: cellout [-g] battery\n", stderr);
            return 1;
        }
    }

    strcpy(charge_full_filename, BATTERY_INFO_PATH);
    strcpy(charge_now_filename, BATTERY_INFO_PATH);
    strcpy(status_filename, BATTERY_INFO_PATH);
    strncat(charge_full_filename, battery, BUFFER_SIZE - 1 -
            sizeof(BATTERY_INFO_PATH) - sizeof(CHARGE_FULL_FILENAME));
    strncat(charge_now_filename, battery, BUFFER_SIZE - 1 -
            sizeof(BATTERY_INFO_PATH) - sizeof(CHARGE_NOW_FILENAME));
    strncat(status_filename, battery, BUFFER_SIZE - 1 -
            sizeof(BATTERY_INFO_PATH) - sizeof(STATUS_FILENAME));
    strcat(charge_full_filename, "/"CHARGE_FULL_FILENAME);
    strcat(charge_now_filename, "/"CHARGE_NOW_FILENAME);
    strcat(status_filename, "/"STATUS_FILENAME);

    if ((f_charge_full = fopen(charge_full_filename, "r")) == NULL) {
        fprintf(stderr, "Failed to open %s\n", charge_full_filename);
        return 2;
    }
    if ((f_charge_now = fopen(charge_now_filename, "r")) == NULL) {
        fprintf(stderr, "Failed to open %s\n", charge_now_filename);
        return 2;
    }
    if ((f_status = fopen(status_filename, "r")) == NULL) {
        fprintf(stderr, "Failed to open %s\n", status_filename);
        return 2;
    }


    if (fgets(charge_full, BUFFER_SIZE, f_charge_full) == NULL)
        fprintf(stderr, "Failed to get line from %s\n", charge_full_filename);
    if (fgets(charge_now, BUFFER_SIZE, f_charge_now) == NULL)
        fprintf(stderr, "Failed to get line from %s\n", charge_now_filename);
    if ((status = fgetc(f_status)) == EOF)
        fprintf(stderr, "Failed to get character from %s\n", status_filename);

    charge_full[strlen(charge_full)-1] = '\0';
    charge_now[strlen(charge_now)-1] = '\0';

    /* TODO: atoi() error checking (should be fine since /proc is not directly
     * editable (and thus should prevent stupid stuff), but nevertheless...
     */
    charge_percentage = atoi(charge_now) * 100 / atoi(charge_full);

    if (show_graphic == 1) {
        charge_percentage_graphic[0] = '\0';
        for (i = 25; i <= 100; i += 25) {
            if (charge_percentage >= i)
                strcat(charge_percentage_graphic, "#");
            else
                strcat(charge_percentage_graphic, "-");
        }
        printf("[%s] %d%c\n", charge_percentage_graphic, charge_percentage,
               status);
    } else
        printf("%d%c\n", charge_percentage, status);

    /*
     * We don't want to exit on failure to close. We don't write anything, so it
     * should be fine, but we should still bring it to the attention of the
     * user.
     *
     * XXX: EXIT_SUCCESS is returned regardless of whether we successfully close
     * the files or not. I'm not sure whether I really want to return failure
     * for something that (shouldn't) cause any problems on error, but it should
     * be such a rare occurrence that we don't really have to worry about it for
     * now.
     */
    if (fclose(f_charge_full) == EOF)
        perror(charge_full_filename);
    if (fclose(f_charge_now) == EOF)
        perror(charge_now_filename);
    if (fclose(f_status) == EOF)
        perror(status_filename);

    return EXIT_SUCCESS;
}
