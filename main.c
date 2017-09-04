#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define MAX_RECORDS 99
#define MIN_RECORDS 1
#define PI_RAD 3.14159
#define PI_DEG 180
#define MC_LAT 144.963123
#define MC_LNG -37.810592
#define MAX_DOW_LEN 9
#define DAYS_IN_WEEK 7


struct Location {
    double lat;
    double lng;
};

typedef enum {
    Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
} daysOfTheWeek;

struct DateTimes {
    int hour;
    int day;
    int month;
    int year;
    daysOfTheWeek dayOfWeek;
};

struct Record {
    int ID;
    struct Location location;
    struct DateTimes dateTime;
};

char *getDaysOfTheWeekStr(daysOfTheWeek day);

double dist(struct Location *p1, struct Location *p2);

double squared(double x);

double toRadian(double x);

void printRecord(struct Record *record, struct Location *referencePoint);

void stagePrint(int n);

void readRecords(struct Record *records);

int main() {
    struct Location MelbourneCentral = {
            .lat=MC_LAT,
            .lng=MC_LNG
    };

    struct Record records[MAX_RECORDS];
    readRecords(records);
    stagePrint(1);
    printRecord(&records[0], &MelbourneCentral);
    return 0;
}

void readRecords(struct Record *records) {
    int i = 0;
    char dayOfWeek[MAX_DOW_LEN + 1]; /* +1 for null byte */
    while (scanf("%d %lf %lf %d/%d/%d %d %s",
                 &records[i].ID,
                 &records[i].location.lat,
                 &records[i].location.lng,
                 &records[i].dateTime.day,
                 &records[i].dateTime.month,
                 &records[i].dateTime.year,
                 &records[i].dateTime.hour,
                 dayOfWeek
    ) == 8) {
        if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Sunday))) { records[i].dateTime.dayOfWeek = Sunday; }
        else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Monday))) { records[i].dateTime.dayOfWeek = Monday; }
        else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Tuesday))) { records[i].dateTime.dayOfWeek = Tuesday; }
        else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Wednesday))) { records[i].dateTime.dayOfWeek = Wednesday; }
        else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Thursday))) { records[i].dateTime.dayOfWeek = Thursday; }
        else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Friday))) { records[i].dateTime.dayOfWeek = Friday; }
        else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Saturday))) { records[i].dateTime.dayOfWeek = Saturday; }
        i++;
    }

}

double dist(struct Location *p1, struct Location *p2) {
    printf("Location P1: <%lf, %lf>\n", p1->lat, p1->lng);
    printf("Location P2: <%lf, %lf>\n", p2->lat, p2->lng);
    double chord_length =
            squared(sin(toRadian(p2->lat - p1->lat) / 2.0)) +
            cos(toRadian(p1->lat)) * cos(toRadian(p2->lat)) *
            squared(sin(toRadian(p2->lng - p1->lng) / 2.0));
    double angle_distance = 2.0 * atan2(sqrt(chord_length), sqrt(1.0 - chord_length));
    return 6371.0 * angle_distance;
}

double toRadian(double x) {
    return x * PI_RAD / PI_DEG;
}

double squared(double x) {
    return x * x;
}

void printRecord(struct Record *record, struct Location *referencePoint) {
    printf("Accident: #%d\n", record->ID);
    printf("Location: <%lf, %lf>\n", record->location.lat, record->location.lng);
    printf("Date: %d/%d/%d\n", record->dateTime.day, record->dateTime.month, record->dateTime.year);
    printf("Time: %d\n", record->dateTime.hour);
    printf("Distance to reference: %2.2lf\n", dist(&record->location, referencePoint));
}

void stagePrint(int n) {
    if (n != 1) { printf("\n"); }
    printf("Stage %d\n==========\n", n);
}

char *getDaysOfTheWeekStr(daysOfTheWeek day) {
    switch (day) {
        case Sunday:
            return "Sunday";
        case Monday:
            return "Monday";
        case Tuesday:
            return "Tuesday";
        case Wednesday:
            return "Wednesday";
        case Thursday:
            return "Thursday";
        case Friday:
            return "Friday";
        case Saturday:
            return "Saturday";
    }
    assert(0 && "Error: Invalid day");
}
