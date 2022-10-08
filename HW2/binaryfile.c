/*
Program that reads data (Five 16-bytes records) from a binary file into a struct to display the information in a readable format.
Author: Augusto Escudero
*/

#include <stdio.h>

struct DataRecord {
    float latitude;
    float longitude;
    short altitude;
    char name[5];
    unsigned char misc;

};

void processMiscField(int misc) {

        int idResult = misc & 0x3;
        int categoryResult = (misc >> 2) & 0x3;
        int engagedResult = (misc >> 4) & 0x1;

        char* category = "ship";
        char* id = "unknown";
        char* engagedText = (engagedResult==1)? "engaged": "";

        switch (idResult) {
            case 1:
                id = "friend";
                break;
            case 2:
                id = "foe";
                break;
            case 3:
                id = "neutral";
                break;
        }

        switch (categoryResult) {
            case 1:
                category = "ground vehicle";
                break;
            case 2:
                category = "airplane";
                break;
        }

        printf(" id:%s cat:%s %s\n", id, category, engagedText);

}

int main(int argc, char *argv[]) {

    FILE *file;
    struct DataRecord data;

    if (argc==2) {
        file = fopen(argv[1], "rb");
    } else {
        printf("Usage: ./binaryfile [filename]");
        return 1;
    }

    if (file == NULL) {
        perror("Error: \n");
        return -1;
    }

    while (fread(&data, sizeof(data), 1, file) == 1) {
        char name[5];

        for (int i=0; i<5; i++) {
            name[i] = data.name[i];
        }

        printf("lat:%0.1f lon:%0.2f alt:%d name:%s", data.latitude, data.longitude, data.altitude, name);
        processMiscField(data.misc);
    }
    
    return 0;
}
