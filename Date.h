#ifndef DATE_H
#define DATE_H
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct DateData
{
    char m_description[255];
    int m_day;
    int m_month;
    int m_year;
}DateData;

enum InputType {
    string,
    integer,
};

int GetDateDataLength(FILE* file);
DateData* NewDateData(int day, int month, int year);
void AllocateDateData(struct DateData* data, int day, int month, int year, char* description);

void AddDateToFile(FILE* file, struct DateData* date, char* description);
DateData* GetDateDatasFromFile(FILE* file, DateData* dates, _Bool reallocate);

void RedrawTerminal(DateData* dates, DateData* today, FILE* file);
void DrawPastDates(DateData* dates, DateData* today, FILE* file);

#endif // !DATE.h
