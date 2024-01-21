#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct DateData
{
    char m_description[255];
    int m_day;
    int m_month;
    int m_year;
};

enum InputType {
    string,
    integer,
};

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

struct DateData* NewDateData(int day, int month, int year)
{
    struct DateData* data = malloc(sizeof(struct DateData));

    if (data != NULL)
    {
        data->m_day = day;
        data->m_month = month;
        data->m_year = year;
    }

    return data;
}

void AllocateDateData(struct DateData* data, int day, int month, int year, char* description)
{
    data->m_day = day;
    data->m_month = month;
    data->m_year = year;
    strcpy(data->m_description, description);
}

void AddDateToFile(FILE* file, struct DateData* date, char* description)
{
    // delete unnecessary newlines
    description[strcspn(description, "\n")] = 0;
    file = fopen("C:/temp/test2.txt", "a");
    fprintf(file, "%d.%02d.%02d - %s\n", date->m_day, date->m_month, date->m_year, description);
    fclose(file);
    file = fopen("C:/temp/test2.txt", "r");
}

struct DateData* GetDateDatasFromFile(FILE* file, struct DateData* dates, _Bool reallocate)
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

void RedrawTerminal(struct DateData* dates, struct DateData* today, FILE* file)
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

void DrawPastDates(struct DateData* dates, struct DateData* today, FILE* file) 
{
    int size = GetDateDataLength(file);
    system("cls");

    // Check every date that is before today
    printf("Past:");
    for (int i = 0; i < size; i++)
    {
        if (today->m_day > dates[i].m_day)
        {
            if (today->m_month > dates[i].m_month)
            {
                printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
                continue;
            }
        }
        if (today->m_year > dates[i].m_year)
        {
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
            continue;
        }
        if (today->m_month > dates[i].m_month)
        {
            printf("\n%d.%02d.%04d - %s", dates[i].m_day, dates[i].m_month, dates[i].m_year, dates[i].m_description);
            continue;
        }
        
    }
    printf("\n\n[1] Back.\n");
    printf("[2] Exit.");
}

// Try to read the correct input before continuing
void ReadInput(enum InputType type, char* target, int* numtarget)
{
    char input[255];
    
    while (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\r\n")] = '\0';
        switch (type)
        {
            case string:
                if (sscanf(input, "%[^\n]", target) != 1)
                {
                    printf("\nPlease enter a string\n");
                }
                else
                {
                    return;
                }
            case integer:
                if (sscanf(input, "%d", numtarget) != 1)
                {
                    printf("\nPlease enter a number\n");
                }
                else 
                {
                    return;
                }
        }
    }
}

int main()
{
    // Try to open the file, if doenst exist create it
    FILE* filepointer;
    filepointer = fopen("C:/temp/test2.txt", "r");
    if (!filepointer)
    {
        filepointer = fopen("C:/temp/test2.txt", "w");
        filepointer = fopen("C:/temp/test2.txt", "r");
    }

    // Get the dates from the txt file
    struct DateData* dates = GetDateDatasFromFile(filepointer, &dates, 0);
   
    // Get todays time
    time_t t = time(NULL);
    struct tm currentTime = *localtime(&t);
    struct DateData* today = NewDateData(currentTime.tm_mday, currentTime.tm_mon + 1, currentTime.tm_year + 1900);

    // while loop variables
    char description[255];
    enum InputType type = integer;
    int day = 0;
    int month = 0;
    int year = 0;
    int Input = 0;
    int Input2 = 0;

    while (Input != 3)
    {
        RedrawTerminal(dates, today, filepointer);
        
        ReadInput(type, NULL, &Input);

        switch (Input)
        {
            case 1:
                // Add Date
                system("cls");
                type = string;
                printf("Enter the description for this date:\n");
                ReadInput(type, description, NULL);

                type = integer;
                printf("Enter the day:\n");
                ReadInput(type, NULL, &day);
                if (day > 31)
                    day = 31;

                printf("Enter the month:\n");
                ReadInput(type, NULL, &month);
                if (month > 12)
                    month = 12;

                printf("Enter the year:\n");
                ReadInput(type, NULL, &year);

                system("cls");
                printf("%d.%02d.%04d - %s\nIs this correct?\n\n[1] Yes.\n[2] No.\n", day, month, year, description);
                ReadInput(type, NULL, &Input2);

                switch (Input2)
                {
                    case 1:
                        AddDateToFile(filepointer, NewDateData(day, month, year), description);
                        dates = GetDateDatasFromFile(filepointer, dates, 1);
                        break;
                    default:
                        break;
                }
                break;
            case 2:
                // Show past dates
                type = integer;
                DrawPastDates(dates, today, filepointer);
                ReadInput(type, NULL, &Input2);
                switch (Input2)
                {
                    case 1:
                        break;
                    case 2: 
                        return;
                    default:
                        break;
                }
                break;
            case 3:
                break;
            default:
                break;
        }
    }

    free(dates);
}