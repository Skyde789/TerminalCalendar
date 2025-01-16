#include <Date.h>

void ReadInput(enum InputType, char*, int*);

int main()
{
    // Try to open the file, if it doesnt exist create it
    FILE* filepointer;
    filepointer = fopen("C:/temp/test2.txt", "r");
    if (!filepointer)
    {
        filepointer = fopen("C:/temp/test2.txt", "w");
        filepointer = fopen("C:/temp/test2.txt", "r");
    }

    // Get the dates from the txt file
    DateData* dates = GetDateDatasFromFile(filepointer, &dates, 0);
   
    // Get todays time
    time_t t = time(NULL);
    struct tm currentTime = *localtime(&t);
    DateData* today = NewDateData(currentTime.tm_mday, currentTime.tm_mon + 1, currentTime.tm_year + 1900);

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
                if (day < 1)
                    day = 1;

                printf("Enter the month:\n");
                ReadInput(type, NULL, &month);
                if (month > 12)
                    month = 12;
                if (month < 1)
                    month = 1;

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
