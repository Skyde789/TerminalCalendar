#include <Date.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


DateData* NewDateData(int day, int month, int year)
{
    struct DateData* data = malloc(sizeof(DateData));

    if (data != NULL)
    {
        data->m_day = day;
        data->m_month = month;
        data->m_year = year;
    }

    return data;
}

int GetDateDataLength(FILE* file)
{
    char row[255];
    int size = -1;

    // Rewind to the start of the file and start counting rows
    rewind(file);
    while (!feof(file))
    {
        size++;
        fgets(row, 255, file);
    }

    return size;
}
void AllocateDateData(DateData* data, int day, int month, int year, char* description)
{
    data->m_day = day;
    data->m_month = month;
    data->m_year = year;
    strcpy(data->m_description, description);
}

void AddDateToFile(FILE* file, DateData* date, char* description)
{
    // delete unnecessary newlines
    description[strcspn(description, "\n")] = 0;
    file = fopen("C:/temp/test2.txt", "a");
    fprintf(file, "%d.%02d.%02d - %s\n", date->m_day, date->m_month, date->m_year, description);
    fclose(file);
    file = fopen("C:/temp/test2.txt", "r");
}

struct DateData* GetDateDatasFromFile(FILE* file, DateData* dates, _Bool reallocate)
{
    char row[255];
    int size = -1;

    system("cls");

    // Rewind to the start of the file and start counting rows
    rewind(file);
    while (!feof(file))
    {
        size++;
        fgets(row, 255, file);
        printf("%s", row);
    }

    printf("\n%d\n", size);


    // Allocate the size of our dates based on the amount of rows
    if (!reallocate)
        dates = (struct DateData*)malloc(size * sizeof(struct DateData));
    else
        dates = (struct DateData*)realloc(dates, size * sizeof(struct DateData));

    // if it worked
    if (dates != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            // rewind back to the start of the file for every loop
            rewind(file);
            int test = 0;

            // loop until we are in the correct row
            while (!feof(file))
            {
                fgets(row, 255, file);

                if (i == test)
                {
                    // printf("breaking at %d ", test);
                    break;
                }

                test++;
            }

            // assign default values for parsing the date from string
            char description[255];
            int year, month, day = 0;
            strcpy(&description, "");

            // parse the values to allocate values
            if (sscanf(row, "%d.%02d.%04d - %[^\n]s", &day, &month, &year, &description) != EOF) {

                AllocateDateData(&dates[i], day, month, year, description);

                printf("\n%d.%02d.%02d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
            }
        }
    }

    return dates;
}

void RedrawTerminal(DateData* dates, DateData* today, FILE* file)
{
    int size = GetDateDataLength(file);

    system("cls");

    // Check every date that is today
    printf("Today:");
    for (int i = 0; i < size; i++)
    {
        if (dates[i].m_day == today->m_day && dates[i].m_month == today->m_month && dates[i].m_year == today->m_year)
        {
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
        }
    }

    // Check every date that is after today
    printf("\n\nUpcoming:");
    for (int i = 0; i < size; i++)
    {
        if (today->m_year > dates[i].m_year)
        {
            continue;
        }
        if (today->m_year < dates[i].m_year)
        {
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
            continue;
        }

        if (today->m_month < dates[i].m_month)
        {
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
            continue;
        }
        if (today->m_day < dates[i].m_day)
        {
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
            continue;
        }
    }

    printf("\n\n[1] Add an event.\n");
    printf("[2] See previous events.\n");
    printf("[3] Exit.\n");
}

void DrawPastDates(DateData* dates, DateData* today, FILE* file)
{
    int size = GetDateDataLength(file);
    system("cls");

    // Check every date that is before today
    printf("Past:");
    for (int i = 0; i < size; i++)
    {
        if (today->m_year > dates[i].m_year)
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
        else if (today->m_month > dates[i].m_month)
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
        else if (today->m_day > dates[i].m_day)
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
    }
    printf("\n\n[1] Back.\n");
    printf("[2] Exit.");
}