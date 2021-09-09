#include <stdio.h>
#include <stdlib.h>

/*Using basic concepts of Pointers and Structures lecture,
create a program that calculates the month's day from a given year and year's day. Use pointers for the month and month's day variables.
Don't forget to add proper errors handling in your program.
*/



/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday)
{

    switch (*pmonth)
    {
    case 1:
        printf("Jan %d, %d \n", *pday, year);
        break;
    case 2:
        printf("Feb %d, %d \n", *pday, year);
        break;
    case 3:
        printf("Mar %d, %d \n", *pday, year);
        break;
    case 4:
        printf("Apr %d, %d \n", *pday, year);
        break;
    case 5:
        printf("May %d, %d \n", *pday, year);
        break;
    case 6:
        printf("Jun %d, %d \n", *pday, year);
        break;
    case 7:
        printf("Jul %d, %d \n", *pday, year);
        break;
    case 8:
        printf("Aug %d, %d \n", *pday, year);
        break;
    case 9:
        printf("Sep %d  %d \n", *pday, year);
        break;
    case 10:
        printf("Oct %d, %d \n", *pday, year);
        break;
    case 11:
        printf("Nov %d, %d \n", *pday, year);
        break;
    case 12:
        printf("Dec %d, %d \n", *pday, year);
        break;

    default:
        printf("Invalid month");
        break;
    }
}

// Check if the year is a leap year
int isLeapYear(int *year)
{
    if (*year % 400 == 0)
    {
        return 1;
    }
    else if (*year % 100 == 0)
    {
        return 0;
    }
    else if (*year % 4 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
    Get the month from the year's day
*/
int monthFromYearday(int *year, int *yearday)
{

    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dayCount = 0;
    if (isLeapYear(year))
    {
        daysInMonth[1] = 29;
    }
    for (int i = 0; i < 12; i++)
    {
        dayCount += daysInMonth[i];
        if (*yearday <= dayCount)
        {
            return 1 + i;
        }
    }
}

/*
    Get the day from the month
*/

int dayFromMonth(int *month, int *year, int *yearday)
{
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year))
    {
        daysInMonth[1] = 29;
    }

    for (int i = 1; i <= 12; i++)
    {
        daysInMonth[i] = daysInMonth[i - 1] + daysInMonth[i];
    }

    //printf("%d - %d \n", yearday, daysInMonth[month - 1]);
    return abs(*yearday - daysInMonth[*month - 2]);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <year> <yearday>\n", argv[0]);
        return 0;
    }

    int year, yearday;

    year = atoi(argv[1]);    /*convert string to integer*/
    yearday = atoi(argv[2]); /*convert string to integer*/

    if (yearday < 1 || yearday > 366)
    {
        printf("valid <yearday> must be between 1 and 366\n");
        return 0;
    }

    int month = monthFromYearday(&year, &yearday);   /*get the month from the year's day*/
    int day = dayFromMonth(&month, &year, &yearday); /*get the day from the month*/

    month_day(year, yearday, &month, &day); /* print the month and day*/

    return 0;
}
