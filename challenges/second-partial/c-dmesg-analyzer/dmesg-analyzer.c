#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

//Define repot file name
#define REPORT_FILE "report.txt"

// Define the maximum number of lines that a log can have
#define MAX_LOGS_ON_TABLE 10000

// Log breakdown
#define MAX_LOGS_ON_BREAKDOWN 100
#define MAX_LOG_TYPE_LEN 1000

#define MAX_LOG_DESCRIPTION_LEN 1000

// Define functions skeleton
char *getSubStr(const char *src, int pos, int len);
struct LogType getLogType(char *currentLine);
unsigned long hash(char *str);

void analizeLog(char *logFile, char *report);
void insertInLogsTable(char *name, char *log);
void generateReport();

/*
    Define the struct to store the log
    Type - The type of the log
    Description - The description of the log
*/
struct Log
{
    char *type,
        *description;
};

/*
    Define Struct to store the log breakdown
    Name - The name of the log type
    Logs - The logs of the log type
*/
struct LogBreakdown
{
    char *name,
        logs[MAX_LOGS_ON_BREAKDOWN * MAX_LOG_TYPE_LEN];
};

/* 
 * Define the struct to store the logs breakdown 
*/
struct LogBreakdown *logsTable[MAX_LOGS_ON_TABLE];

/* Auxiliar function
 * Hash function for the log types and not repeated log types, the hash function is the sum of the ASCII values of the characters in the string
 * Hash function was created by the following link: http://www.cse.yorku.ca/~oz/hash.html (Ozan Ozturk)
 */
unsigned long hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash %= MAX_LOGS_ON_TABLE;
}

/*
 * Insert a log in the logs table and check if with the hash function the log type is already in the table
*/
void insertInLogsTable(char *logName, char *logDescription)
{
    // Get the hash value of the log type
    int hashValue = hash(logName);

    while (logsTable[hashValue] != NULL)
    {
        // Get the name from the log type and check if log is on the table
        char *logTypeName = logsTable[hashValue]->name;
        if (strcmp(logTypeName, logName) == 0)
        {
            // If log type already exist, add the new log to the log type
            strcat(logsTable[hashValue]->logs, "\t");
            strcat(logsTable[hashValue]->logs, logDescription);
            return;
        }
        ++hashValue;
        hashValue %= MAX_LOGS_ON_TABLE;
    }

    // If log type is not in the table, create a new log type
    struct LogBreakdown *logBreakdown = (struct LogBreakdown *)malloc(sizeof(struct LogBreakdown));
    // Copy the name of the log type
    logBreakdown->name = logName;
    // Add the log type to the table
    strcat(logBreakdown->logs, "\t");
    strcat(logBreakdown->logs, logDescription);

    // insert the log type in the table
    logsTable[hashValue] = logBreakdown;
}

/*
    * Auxiliar function to get the substring from a string
*/
char *getSubStr(const char *src, int pos, int len)
{
    char *dest = (char *)malloc(sizeof(char) * (len + 1));
    strncpy(dest, (src + pos), len);
    return dest;
}

/*
    * Generate report based on the logs table
*/
void generateReport(char *report)
{
    // Create and open the report file
    int reportPtr = open(report, O_CREAT | O_WRONLY, 0600);

    // Check if the report file was created
    if (reportPtr < 0)
    {
        printf("Error: could not create the report\n");
        return;
    }

    // Start to write the report file
    for (int i = 0; i < MAX_LOGS_ON_TABLE; i++)
    {
        // Check if there's a log in tthe table
        if (logsTable[i] != NULL)
        {
            // Declare logs table
            char logs_table[MAX_LOGS_ON_TABLE];
            // Get log attributes
            char *name = logsTable[i]->name;
            char *logs = logsTable[i]->logs;

            // Write the following log breakdown in a string with a size constarin
            snprintf(logs_table, MAX_LOGS_ON_TABLE, "%s: \n%s\n", name, logs);

            // Write the logs table in the report file
            size_t logsTableSize = strlen(logs_table);
            write(reportPtr, logs_table, logsTableSize);
        }
    }

    // Close the report file
    close(reportPtr);
}

/* 
    * Get the log type from the current line
*/
struct Log getCurrentLog(char *currentLine)
{
    //Define variable
    char currentCharacter;
    int startIndex = 0, logLen = 0, logStart = 0, isDelimiter = 0;

    //Traverse the current line
    for (size_t i = 0; i < strlen(currentLine); i++)
    {
        //Get the current character
        currentCharacter = currentLine[i];

        // Divide the current line based on the delimiters
        if (currentCharacter == ']')
        {
            logStart = 1;
        }
        else if (logStart)
        {
            if (currentCharacter != ' ')
            {
                startIndex = i;
                for (size_t j = i; j < strlen(currentLine); j++)
                {
                    currentCharacter = currentLine[j];
                    if (currentCharacter == ':')
                    {
                        isDelimiter = 1;
                    }
                    else if (currentCharacter == ' ' && isDelimiter)
                    {
                        break;
                    }
                    logLen++;
                }
                break;
            }
        }
    }

    struct Log log;
    // If log type is not found, return the log to the general category
    if (isDelimiter == 0)
    {
        log.type = "General";
        log.description = currentLine;
    }
    else
    {
        // Get the log type
        log.type = getSubStr(currentLine, startIndex, logLen - 1);
        // Get the log description
        char *logTime = getSubStr(currentLine, 0, startIndex - 1),
             *logMessage = getSubStr(currentLine, startIndex + logLen, strlen(currentLine));

        // Assign the log memory size
        log.description = (char *)malloc(MAX_LOG_DESCRIPTION_LEN);

        // Add the log time and the log message to the log description
        strcpy(log.description, logTime);
        strcat(log.description, logMessage);
    }

    return log;
}

/*
    * Traverse the file and process the each line into a log struct
*/
void analizeLog(char *logFile, char *report)
{
    printf("Generating Report from: [%s] log file\n", logFile);

    // pointer to the file
    FILE *ptrFile;
    // Current log line
    char *currentLine = NULL;

    // Get the size of the file
    size_t len = 0;
    ssize_t read;

    // Open the file and check if it was opened successfully
    if ((ptrFile = fopen(logFile, "r")) == NULL)
    {
        printf("Error: file not found\n");
        return;
    }

    // Read the file line by line
    while ((read = getline(&currentLine, &len, ptrFile)) != -1)
    {
        //Pass when the line is empty
        if (strlen(currentLine) == 0 || strcasecmp(currentLine, "\n") == 0)
            continue;
        //Get the log type and description
        struct Log currentLog = getCurrentLog(currentLine);
        //Insert the log into the table
        insertInLogsTable(currentLog.type, currentLog.description);
    }

    // Generate report file based on the logs table
    generateReport(report);

    printf("Report is generated at: [%s]\n", report);
}

int main(int argc, char **argv)
{
    // Handle command currentLine arguments
    if (argc < 2)
    {
        printf("Usage:./dmesg-analizer logfile.txt\n");
        return 1;
    }

    // Generate report file
    analizeLog(argv[1], REPORT_FILE);

    return 0;
}