#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
long calc(int operator, int nValues, int *values)
{

    long result = operator== 3 ? 1 : 0; /* initialize the result to 1 if multiplication, 0 if addition or subtraction */

    /* loop through all values in the array and add/subtract/multiply them */
    for (int i = 0; i < nValues; i++)
    {
        switch (operator)
        {
        case 1:
            result += values[i];
            break;
        case 2:
            result -= values[i];
            break;
        case 3:
            result *= values[i];
            break;
        }
    }
    return result;
}

/* based on the operator, this function will return the appropriate integer 
*/
int operator_is_valid(char *operator)
{

    if (strcmp(operator, "add") == 0)
    {
        return 1;
    }
    else if (strcmp(operator, "sub") == 0)
    {
        return 2;
    }
    else if (strcmp(operator, "mult") == 0)
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

int main(int argc, char *argv[])
{

    /* check if the user has entered the correct number of arguments */
    if (argc < 4)
    {
        printf("Invalid arguments...\n");
        return 1;
    }
    /* get all the values from the command line minus the filename and operator*/
    int values[argc - 2];
    for (int i = 2; i < argc; i++)
    {
        values[i - 2] = atoi(argv[i]);
    }

    /* size of the array of values */
    int nValues = sizeof(values) / sizeof(int);
    /* get the operator from the command line */
    char *operator= argv[1];
    /* get the operator integer */
    int operator_type = operator_is_valid(operator);
    /* check if the operator is valid */
    if (operator_type == 0)
    {
        printf("Invalid operator...\n");
    }
    else
    {
        printf("%d\n", calc(operator_type, nValues, values));
    }
    return 0;
}
