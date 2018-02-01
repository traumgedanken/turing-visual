#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STATE_LEN 10
#define TABLE_LEN 100
#define LINE_LEN 300

struct Command
{
    char currentState[STATE_LEN];
    char nextState[STATE_LEN];
    char direction;
    char currentChar;
    char newChar;
};

struct Table
{
    struct Command *command;
    int length;
};

struct Error
{
    int code;
    int line;
};

struct ErrorArray
{
    struct Error *error;
    int length;
};

bool readLine(char *line, FILE *f);
void fillTableFromFile(Table *table, const char *fileName);
void convertLineToCommand(char *line, Command *command);
void addCommand(Command *command, Table *table);
void addError(Error *error, ErrorArray *errorArray);
void printTable(Table *table);
void printErrorArray(ErrorArray *errorArray);
void freeTable(Table *table);
void freeErrorArray(ErrorArray *errorArray);
void checkFile(char *fileName, ErrorArray *errorArray);
int checkLine(char *line);
int checkState(char *state);
int checkCh(char *ch);
int checkDir(char *dir);

int main(int argc, char **argv)
{
    struct Table table;
    struct ErrorArray errors;
    checkFile(argv[1], &errors);
    if (errors.length == 0)
    {
        fillTableFromFile(&table, argv[1]);
        printTable(&table);
        freeTable(&table);
    }
    else
    {
        printErrorArray(&errors);
        freeErrorArray(&errors);
    }
    return EXIT_SUCCESS;
}

void freeTable(struct Table *table)
{
    if (table->command != NULL)
    {
        free(table->command);
    }
    table->length = 0;
}

bool readLine(char *line, FILE *f)
{
    fgets(line, LINE_LEN, f);
    int length = strlen(line);
    if (line[length - 1] == '\n')
    {
        line[length - 1] = '\0';
    }
    if (length == 1 && line[0] == '\n')
    {
        return false;
    }
    return true;
}

void fillTableFromFile(struct Table *table, const char *fileName)
{
    table->length = 0;
    FILE *f = fopen(fileName, "r");
    while (!feof(f))
    {
        char line[LINE_LEN];
        if (!readLine(line, f))
        {
            continue;
        }
        struct Command current;
        convertLineToCommand(line, &current);
        addCommand(&current, table);
    }
    fclose(f);
}

void convertLineToCommand(char *line, Command *command)
{
    int length = strlen(line);
    int index = 0;
    int read = 0;
    while (line[index] != ',')
    {
        command->currentState[index] = line[read];
        index++;
        read++;
    }
    command->currentState[index] = '\0';
    read++;
    if (line[read] == '-')
    {
        command->currentChar = 0;
    }
    else
    {
        command->currentChar = line[read];
        read++;
    }
    read += 2;
    if (line[read] == ',')
    {
        command->newChar = command->currentChar;
    }
    else
    {
        command->newChar = line[read];
        read++;
    }
    read++;
    if (line[read] == ',')
    {
        command->direction = 'N';
    }
    else
    {
        command->direction = line[read];
        read++;
    }
    read++;
    if (line[read] == '\0')
    {
        strcpy(command->nextState, command->currentState);
    }
    else
    {
        strcpy(command->nextState, line + read);
    }
}

void addCommand(Command *command, Table *table)
{
    if (table->length == 0)
    {
        table->command = (Command *)malloc(sizeof(Command));
    }
    else
    {
        table->command = (Command *)realloc(table->command, sizeof(Command) * (table->length + 1));
    }
    strcpy(table->command[table->length].currentState, command->currentState);
    strcpy(table->command[table->length].nextState, command->nextState);
    table->command[table->length].currentChar = command->currentChar;
    table->command[table->length].newChar = command->newChar;
    table->command[table->length].direction = command->direction;
    table->length++;
}

void addError(Error *error, ErrorArray *errorArray)
{
    if (errorArray->length == 0)
    {
        errorArray->error = (Error *)malloc(sizeof(Error));
    }
    else
    {
        errorArray->error = (Error *)realloc(errorArray->error, sizeof(Error) * (errorArray->length + 1));
    }
    errorArray->error[errorArray->length].line = error->line;
    errorArray->error[errorArray->length].code = error->code;
    errorArray->length++;
}

void printTable(Table *table)
{
    for (int i = 0; i < table->length; i++)
    {
        printf("%s | %c -> %c | %c | %s\n",
               table->command[i].currentState,
               table->command[i].currentChar,
               table->command[i].newChar,
               table->command[i].direction,
               table->command[i].nextState);
    }
}

void freeErrorArray(ErrorArray *errorArray)
{
    if (errorArray->error != NULL)
    {
        free(errorArray->error);
    }
    errorArray->length = 0;
}

void printErrorArray(ErrorArray *errorArray)
{
    for (int i = 0; i < errorArray->length; i++)
    {
        printf("Line %i: error code[%i]\n",
               errorArray->error[i].line,
               errorArray->error[i].code);
    }
}

void checkFile(char *fileName, ErrorArray *errorArray)
{
    errorArray->length = 0;
    // FILE *f = fopen(fileName, "a");
    // fprintf(f, "\n");
    // fclose(f);
    FILE *f = fopen(fileName, "r");
    int lineNumber = 0;
    while (!feof(f))
    {
        lineNumber++;
        char line[LINE_LEN];
        if (!readLine(line, f))
        {
            continue;
        }
        int code = checkLine(line);
        if (code != 0)
        {
            Error newError = {code, lineNumber};
            addError(&newError, errorArray);
        }
    }
    fclose(f);
}

int checkLine(char *line)
{
    int length = strlen(line);
    char state[LINE_LEN];
    state[0] = '\0';
    int index = 0;
    int read = 0;
    while (line[read] != ',' && read < length)
    {
        state[index] = line[read];
        index++;
        read++;
    }
    state[index] = '\0';
    int check = checkState(state);
    if (check != 0)
    {
        return check;
    }
    read++;
    if (read == length || read - 1 == length)
    {
        return 3;
    }
    index = 0;
    char ch[LINE_LEN];
    ch[0] = '\0';
    while (line[read] != ',' && read < length)
    {
        ch[index] = line[read];
        index++;
        read++;
    }
    ch[index] = '\0';
    check = checkCh(ch);
    if (check != 0)
    {
        return check;
    }
    read++;
    if (read == length || read - 1 == length)
    {
        return 8;
    }
    index = 0;
    char dir[LINE_LEN];
    dir[0] = '\0';
    while (line[read] != ',' && read < length)
    {
        dir[index] = line[read];
        index++;
        read++;
    }
    dir[index] = '\0';
    check = checkDir(dir);
    if (check != 0)
    {
        return check;
    }
    if (read == length)
    {
        return 10;
    }
    read++;
    if (line[read] != '\0')
    {
        state[0] = '\0';
        int index = 0;
        while (read < length)
        {
            state[index] = line[read];
            index++;
            read++;
        }
        state[index] = '\0';
        check = checkState(state);
        if (check != 0)
        {
            return check + 9;
        }
    }
    return 0;
}

int checkState(char *state)
{
    int length = strlen(state);
    if (length == 0)
    {
        return 1;
    }
    for (int i = 0; i < length; i++)
    {
        if (state[i] == ' ' || state[i] == ',')
        {
            return 2;
        }
    }
    return 0;
}

int checkCh(char *ch)
{
    int length = strlen(ch);
    if (length == 0)
    {
        return 4;
    }
    int index = 0;
    while (ch[index] != '-' && index < length)
    {
        index++;
    }
    if (index == length - 1 || ch[index + 1] != '>')
    {
        return 5;
    }
    if (index > 1)
    {
        return 6;
    }
    if (length - index - 2 > 1)
    {
        return 7;
    }
    return 0;
}

int checkDir(char *dir)
{
    int length = strlen(dir);
    if (length == 0)
    {
        return 0;
    }
    if (length > 1 || (dir[0] != 'L' && dir[0] != 'R' && dir[0] != 'N'))
    {
        return 9;
    }
    return 0;
}