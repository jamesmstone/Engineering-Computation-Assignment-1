#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#define MAX_RECORDS 99
#define MIN_RECORDS 1
#define PI_RAD 3.14159
#define PI_DEG 180.0
#define MC_LAT -37.810592
#define MC_LNG 144.963123
#define MAX_DOW_LEN 9
#define DAYS_IN_WEEK 7
#define MIN_DIST 0
#define MAX_DIST 100


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

int readRecords(struct Record *records);

void
printRecordAndDistance(struct Record *record, struct Location *referencePoint);

void
getMostFrequentDay(struct Record *records, int numRecords, daysOfTheWeek *day,
                   int *occurrences);

void
stage1(struct Record *records, struct Location *reference);

void stage2(struct Record *records, int numRecords,
            struct Location *reference);

void stage3(struct Record *records, int numRecords);

int main() {
    struct Location MelbourneCentral = {
            .lat=MC_LAT,
            .lng=MC_LNG
    };
    struct Record records[MAX_RECORDS];
    int numRecords = readRecords(records);
    assert(numRecords <= MAX_RECORDS && numRecords >= MIN_RECORDS);

    stage1(records, &MelbourneCentral);
    stage2(records, numRecords, &MelbourneCentral);
    stage3(records, numRecords);

    return 0;
}

void stage3(struct Record *records, int numRecords) {
    stagePrint(3);
    printf("Number of accidents: %d\n", numRecords);
    daysOfTheWeek mostFrequentDay;
    int occurrences;
    getMostFrequentDay(records, numRecords, &mostFrequentDay, &occurrences);
    printf("Day of week with the most accidents: %s (%d accident(s))\n",
           getDaysOfTheWeekStr(mostFrequentDay), occurrences);
}

void stage2(struct Record *records, int numRecords,
            struct Location *reference) {
    stagePrint(2);
    int i;
    for (i = 0; i < numRecords; i++) {
        printRecordAndDistance(&records[i], reference);
    }
}

void stage1(struct Record *records, struct Location *reference) {
    stagePrint(1);
    printRecord(&records[0], reference);
}

void
getMostFrequentDay(struct Record *records, int numRecords, daysOfTheWeek *day,
                   int *occurrences) {
    int dayCount[DAYS_IN_WEEK] = {0};
    int i;
    *occurrences = 0;
    for (i = 0; i < numRecords; i++) {
        dayCount[records[i].dateTime.dayOfWeek]++;
        if (dayCount[records[i].dateTime.dayOfWeek] > *occurrences) {
            *occurrences = dayCount[records[i].dateTime.dayOfWeek];
            *day = records[i].dateTime.dayOfWeek;
        }
    }
}

void
printRecordAndDistance(struct Record *record, struct Location *referencePoint) {
    double distance = dist(&record->location, referencePoint);
    assert(distance <= MAX_DIST && distance >= MIN_DIST);
    printf("Accident: #%d, distance to reference: %05.02lf ", record->ID,
           distance);
    /* Distance Bar */
    printf("|");
    int i;
    for (i = 0; i < ceil(distance); i++) {
        printf((i + 1) % 10 ? "-" : "+");
    }

    printf("\n");
}

int readRecords(struct Record *records) {
    int i = 0;
    char dayOfWeek[MAX_DOW_LEN + 1]; /* +1 for null byte */
    while (scanf("%d %lf %lf %d/%d/%d %d %s",
                 &records[i].ID,
                 &records[i].location.lng,
                 &records[i].location.lat,
                 &records[i].dateTime.day,
                 &records[i].dateTime.month,
                 &records[i].dateTime.year,
                 &records[i].dateTime.hour,
                 dayOfWeek
    ) == 8) {
        if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Sunday)) == 0) {
            records[i].dateTime.dayOfWeek = Sunday;
        } else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Monday)) == 0) {
            records[i].dateTime.dayOfWeek = Monday;
        } else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Tuesday)) == 0) {
            records[i].dateTime.dayOfWeek = Tuesday;
        } else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Wednesday)) == 0) {
            records[i].dateTime.dayOfWeek = Wednesday;
        } else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Thursday)) == 0) {
            records[i].dateTime.dayOfWeek = Thursday;
        } else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Friday)) == 0) {
            records[i].dateTime.dayOfWeek = Friday;
        } else if (strcmp(dayOfWeek, getDaysOfTheWeekStr(Saturday)) == 0) {
            records[i].dateTime.dayOfWeek = Saturday;
        }
        i++;
    }
    return i;
}

double dist(struct Location *p1, struct Location *p2) {
    double chord_length =
            squared(sin(toRadian(p2->lat - p1->lat) / 2.0)) +
            cos(toRadian(p1->lat)) * cos(toRadian(p2->lat)) *
            squared(sin(toRadian(p2->lng - p1->lng) / 2.0));
    double angle_distance =
            2.0 * atan2(sqrt(chord_length), sqrt(1.0 - chord_length));
    return 6371.0 * angle_distance;
}

double toRadian(double x) {
    return x * (PI_RAD / PI_DEG);
}

double squared(double x) {
    return x * x;
}

void printRecord(struct Record *record, struct Location *referencePoint) {
    printf("Accident: #%d\n", record->ID);
    printf("Location: <%lf, %lf>\n", record->location.lng,
           record->location.lat);
    printf("Date: %d/%d/%d\n", record->dateTime.day, record->dateTime.month,
           record->dateTime.year);
    printf("Time: %02d\n", record->dateTime.hour);
    printf("Distance to reference: %05.02lf\n",
           dist(&record->location, referencePoint));
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
    assert(!"Error: Invalid day");
    return NULL;
}
