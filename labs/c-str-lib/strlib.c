

int mystrlen(char *str)
{
    int len = 0;
    while (*str != '\0')
    {
        len++;
        str++;
    }
    return len;
}

char *mystradd(char *origin, char *addition)
{
    char *result = origin + mystrlen(origin);
    while (*addition != '\0')
    {
        *result = *addition;
        result++;
        addition++;
    }
    *result = '\0';
    return origin;
}
 
int mystrfind(char *origin, char *substr)
{
    int len = 0;
    int len2 = 0;
    int verif = mystrlen(substr);
    int tmp = 0;

    while (origin[len] != '\0')
    {
        tmp = len;
        while (origin[len] == substr[len2] && origin[len] != '\0')
        {
            len++;
            len2++;
        }
        if (len2 == verif)
        {
            return tmp;
        }
        len++;
        len2 = 0;
    }
    return -1;
}
