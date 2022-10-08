/*
Program that reads data from a binary file into a struct (using bit fields) to display the information in a readable format
Author: Augusto Escudero
*/

#include <stdio.h>
#include <string.h>
#pragma pack(1)

struct DataRecord {
    float latitude;
    float longitude;
    short altitude;
    char name[5];
    //'misc' bit field
    unsigned char id: 2;
    unsigned char category: 2;
    unsigned char engaged: 1;
    unsigned char reserved_1: 3;

    //'time reported' bit field
    unsigned short year: 7;
    unsigned short month: 4;
    unsigned short day: 5;
    
    unsigned short hour: 5;
    unsigned short minute: 6;
    unsigned short reserved_2: 5;

    unsigned short second: 6;
    unsigned short reserved_3: 10;
    
};

void processData(struct DataRecord data) {
        
        char name[6];
        char months[12][10] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        
        for (int i=0; i<5; i++) {
            name[i] = data.name[i];
        }
        name[5]='\0';

        char* category = "ship";
        char* id = "unknown";
        char* month = months[data.month-1];
        char* engagedText = (data.engaged==1)? "engaged": "";

        switch (data.id) {
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

        switch (data.category) {
            case 1:
                category = "ground vehicle";
                break;
            case 2:
                category = "airplane";
                break;
            case 3:
                category = "???";
                break;
        }

        printf("lat:%0.1f lon:%0.2f alt:%d name:%s", data.latitude, data.longitude, data.altitude, name);
        printf(" id:%s cat:%s %s ", id, category, engagedText);
        printf("reported: %d/%s/%02d %02d:%02d:%02d\n", 2000+data.year, month, data.day, data.hour, data.minute, data.second);
}

int main(int argc, char *argv[]) {

    FILE *file;
    struct DataRecord data;

    if (argc==2) {
        file = fopen(argv[1], "rb");
    } else {
        printf("Usage: ./binaryfile2 [filename]");
        return 1;
    }

    if (file == NULL) {
        perror("Error: \n");
        return -1;
    }

    while (fread(&data, sizeof(data), 1, file) == 1) {
        processData(data);
    }

    return 0;
}
