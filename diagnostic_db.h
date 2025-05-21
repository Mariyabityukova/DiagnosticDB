#ifndef DIAGNOSTIC_DB_H
#define DIAGNOSTIC_DB_H

/*
 * Модуль работы с базой данных диагностических программ
 * Автор: Битюкова М.С., группа бИСТ231
 * Версия: 2.0
 */

#include <stdio.h>

 /* Максимальные длины полей */
#define MAX_LENGTH          500
#define MAX_NAME_LEN        100
#define MAX_MANUFACTURER_LEN 100
#define MAX_VERSION_LEN     30
#define MAX_OS_LEN          50
#define MAX_DESC_LEN        500
#define MAX_FILENAME_LEN    256
#define MAX_PROGRAMS_COUNT  100

/*
 * Структура для хранения информации о программе
 */
typedef struct {
    char name[MAX_NAME_LEN];          /* Название программы */
    char manufacturer[MAX_MANUFACTURER_LEN]; /* Производитель */
    char version[MAX_VERSION_LEN];           /* Версия программы */
    int releaseYear;                  /* Год выпуска */
    int isFreeware;                   /* Тип лицензии (1-freeware, 0-commercial) */
    char supportedOS[MAX_OS_LEN];     /* Поддерживаемые ОС */
    char description[MAX_DESC_LEN];   /* Описание программы */
} DiagnosticProgram;

/* Основные функции */
int writeDataToFile(DiagnosticProgram* programs, int count, const char* filename);
int readDataFromFile(DiagnosticProgram** programs, int* count, const char* filename);
int addNewProgram(DiagnosticProgram** programs, int* count);
int editProgram(DiagnosticProgram* programs, int count, int index);
int deleteProgram(DiagnosticProgram* programs, int* count, int index);

/* Функции поиска и сортировки */
int searchPrograms(DiagnosticProgram* programs, int count, DiagnosticProgram key,
    DiagnosticProgram** found, int* foundCount);
int sortProgramsByName(DiagnosticProgram* programs, int count);
int sortProgramsByYear(DiagnosticProgram* programs, int count);

/* Функции отображения */
int displayData(DiagnosticProgram* programs, int count);

#endif /* DIAGNOSTIC_DB_H */