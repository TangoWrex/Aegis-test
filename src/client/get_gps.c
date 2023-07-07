#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>  // calloc
#include <stdbool.h> // bool
#include <errno.h>
#include <gps.h>
#include <math.h>

bool get_gps(double * lat, double * lon)
{
    // struct timeval tv;
    struct gps_data_t gps_data;

    if ((gps_open(GPSD_SHARED_MEMORY, NULL, &gps_data)) == -1)
    {
        printf("code: %d, reason: %s\n", errno, gps_errstr(errno));
        return false;
    }

    for (int i = 0; i < 10; i++)
    {
        /* read data */
        if ((gps_read(&gps_data, NULL, 0)) == -1)
        {
            printf("error occured reading gps data. code: %d, reason: %s\n",
                   errno,
                   gps_errstr(errno));
        }
        else
        {
            /* Display data from the GPS receiver. */
            if ((gps_data.status == STATUS_FIX) &&
                (gps_data.fix.mode == MODE_2D || gps_data.fix.mode == MODE_3D) &&
                !isnan(gps_data.fix.latitude) && !isnan(gps_data.fix.longitude))
            {
                *lat = gps_data.fix.latitude;
                *lon = gps_data.fix.longitude;
                // printf("latitude: %f, longitude: %f, speed: %f\n",
                // gps_data.fix.latitude, gps_data.fix.longitude, gps_data.fix.speed);
                return true;
            }
            else
            {
                printf("no GPS data available\n");
                return false;
            }
        }
        sleep(3);
    }

    /* When you are done... */
    gps_close(&gps_data);
    return true;
}

// void send_gps(int socket, double * lat, double * lon)
// {
//     if (!socket || !lat || !lon)
//     {
//         perror("send_gps arguments not recieved\n");
//         return;
//     }
//     // convert double pointer to string
//     char   lat_str[20];
//     char   lon_str[20];
//     char * gps_data = calloc(1, sizeof(char));
//     if (!gps_data)
//     {
//         perror("gps_data calloc failed\n");
//         return;
//     }
//     sprintf(lat_str, "%f", *lat);
//     sprintf(lon_str, "%f", *lon);
//     strcat(gps_data, lat_str);
//     strcat(gps_data, ",");
//     strcat(gps_data, lon_str);
//     // printf("gps_data: %s\n", gps_data);
//     // send gps data
//     if (send(socket, gps_data, strlen(gps_data), 0) < 0)
//     {
//         perror("send failed\n");
//         free(gps_data);
//         return;
//     }
//     free(gps_data);
//     return;

//     // strcpy(mac_addr, mac);
//     // send(socket, mac_addr, strlen(mac_addr), 0);
//     // free(mac_addr);
// }
