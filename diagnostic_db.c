/*
 * Реализация модуля работы с базой данных диагностических программ
 * Автор: Битюкова М.С., группа бИСТ231
 * Версия: 2.1
 */

#include "diagnostic_db.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Внутренние вспомогательные функции */

/* Сравнивает объекты по имени */
static int compareByName(const void* a, const void* b) {
    const DiagnosticProgram* pa = (const DiagnosticProgram*)a;
    const DiagnosticProgram* pb = (const DiagnosticProgram*)b;
    return strcmp(pa->name, pb->name);
}

/* Сравнивает объекты по году выпуска */
static int compareByYear(const void* a, const void* b) {
    const DiagnosticProgram* pa = (const DiagnosticProgram*)a;
    const DiagnosticProgram* pb = (const DiagnosticProgram*)b;
    return pa->releaseYear - pb->releaseYear;
}

/*
   Получает значение поля из строки внешнего файла формата csv
*/
void getField(char* line, int num, char delim, char* result, size_t result_size, int* is_cumul_res_ok) {
    char* start = line;
    char* end;
    int field = 0;
    while (field < num - 1) {
        end = strchr(start, delim);
        if (!end) {
            *is_cumul_res_ok = 0;
            return;
        }
        start = end + 1;
        field++;
    }
    end = strchr(start, delim);
    if (end) {
        size_t len = end - start;
        if (len < result_size) {
            strncpy(result, start, len);
            result[len] = '\0';
        }
        else {
            *is_cumul_res_ok = 0;
        }
    }
    else {
        strncpy(result, start, result_size - 1);
        result[result_size - 1] = '\0';
    }
}

/* Основные функции */

/*
   Ручной ввод данных новой программы
   program - указатель на массив структур данных всех программ
*/
static int inputProgram(DiagnosticProgram* program) {
    if (!program) return 0;

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        continue;
    }
    
    printf("Название: ");
    char nameBuffer[MAX_NAME_LEN];
    if (fgets(nameBuffer, sizeof(nameBuffer), stdin) == NULL) return 0;
    nameBuffer[strcspn(nameBuffer, "\n")] = '\0'; // Удаление символа новой строки
    strncpy(program->name, nameBuffer, sizeof(program->name) - 1);
    program->name[sizeof(program->name) - 1] = '\0';

    printf("Производитель: ");
    char manufacturerBuffer[MAX_MANUFACTURER_LEN];
    if (fgets(manufacturerBuffer, sizeof(manufacturerBuffer), stdin) == NULL) return 0;
    manufacturerBuffer[strcspn(manufacturerBuffer, "\n")] = '\0'; // Удаление символа новой строки
    strncpy(program->manufacturer, manufacturerBuffer, sizeof(program->manufacturer) - 1);
    program->manufacturer[sizeof(program->manufacturer) - 1] = '\0';


    printf("Версия (например 2.5): ");
    char versionBuffer[MAX_VERSION_LEN];
    if (fgets(versionBuffer, sizeof(versionBuffer), stdin) == NULL) return 0;
    versionBuffer[strcspn(versionBuffer, "\n")] = '\0'; // Удаление символа новой строки
    strncpy(program->version, versionBuffer, sizeof(program->version) - 1);
    program->version[sizeof(program->version) - 1] = '\0';

    printf("Год выпуска: ");
    char yearBuffer[6];
    if (fgets(yearBuffer, sizeof(yearBuffer), stdin) == NULL) return 0;
    yearBuffer[strcspn(yearBuffer, "\n")] = '\0'; // Удаление символа новой строки
    program->releaseYear = atoi(yearBuffer);

    printf("Тип лицензии (1-freeware/0-commercial): ");
    char licTypeBuffer[3];
    if (fgets(licTypeBuffer, sizeof(licTypeBuffer), stdin) == NULL) return 0;
    licTypeBuffer[strcspn(licTypeBuffer, "\n")] = '\0'; // Удаление символа новой строки
    program->isFreeware = atoi(licTypeBuffer);

    printf("Поддерживаемые ОС: ");
    char osBuffer[MAX_OS_LEN];
    if (fgets(osBuffer, sizeof(osBuffer), stdin) == NULL) return 0;
    osBuffer[strcspn(osBuffer, "\n")] = '\0'; // Удаление символа новой строки
    strncpy(program->supportedOS, osBuffer, sizeof(program->supportedOS) - 1);
    program->supportedOS[sizeof(program->supportedOS) - 1] = '\0';

    printf("Описание: ");
    char descBuffer[MAX_DESC_LEN];
    if (fgets(descBuffer, sizeof(descBuffer), stdin) == NULL) return 0;
    descBuffer[strcspn(descBuffer, "\n")] = '\0'; // Удаление символа новой строки
    strncpy(program->description, descBuffer, sizeof(program->description) - 1);
    program->description[sizeof(program->description) - 1] = '\0';

    return 1;
}

/*
   Записывает данные всех программ во внешний файл формата csv
   programs - указатель на массив структур данных программ,
   count - количество программ,
   filename - имя внешнего файла
*/
int writeDataToFile(DiagnosticProgram* programs, int count, const char* filename) {
    if (!programs || count <= 0 || !filename) return -1;

    FILE* file = fopen(filename, "w");
    if (!file) return -1;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%s|%d|%d|%s|%s\n",
            programs[i].name,
            programs[i].manufacturer,
            programs[i].version,
            programs[i].releaseYear,
            programs[i].isFreeware,
            programs[i].supportedOS,
            programs[i].description);
    }

    fclose(file);
    return 0;
}

/*
   Загружает данные программ из внешнего файла формата csv
   programs - указатель на массив структур данных программ,
   count - количество загруженных программ,
   filename - имя внешнего файла
*/
int readDataFromFile(DiagnosticProgram** programs, int* count, const char* filename) {

    if (!programs || !count || !filename) return -1;

    char delim = '|';
    FILE* file = fopen(filename, "r");
    if (!file) return -1;

    #define fields_count 7
    char line[MAX_LENGTH * fields_count];
    int lines = 0;
    char ch;
    size_t len = 0;

    while(!feof(file) && lines < MAX_PROGRAMS_COUNT)
    {
      ch = fgetc(file);
      if(ch == '\n')
      {
        lines++;
      }
    }
    
    if (lines == 0) {
        fclose(file);
        return -1;
    }

    *programs = malloc(lines * sizeof(DiagnosticProgram));
    if (!*programs) {
        fclose(file);
        return -1;
    }

    rewind(file);
    for (int i = 0; i < lines; i++) {
        char releaseYear[5];
        char isFreeware[2];
        int is_cumul_res_ok = 1;
        if (fgets(line, sizeof(line), file) == NULL) {
            is_cumul_res_ok = 0;
        }
        else {
            line[strlen(line) - 1] = '\0';
            getField(line, 1, delim, &(*programs)[i].name, sizeof((*programs)[i].name), &is_cumul_res_ok);
            getField(line, 2, delim, &(*programs)[i].manufacturer, sizeof((*programs)[i].manufacturer), &is_cumul_res_ok);
            getField(line, 3, delim, &(*programs)[i].version, sizeof(&(*programs)[i].version), &is_cumul_res_ok);
            getField(line, 4, delim, releaseYear, sizeof(releaseYear), &is_cumul_res_ok);
            if (is_cumul_res_ok) {
                (*programs)[i].releaseYear = atoi(releaseYear);
            }
            getField(line, 5, delim, isFreeware, sizeof(isFreeware), &is_cumul_res_ok);
            if (is_cumul_res_ok) {
                (*programs)[i].isFreeware = atoi(isFreeware);
            }
            getField(line, 6, delim, &(*programs)[i].supportedOS, sizeof((*programs)[i].supportedOS), &is_cumul_res_ok);
            getField(line, 7, delim, &(*programs)[i].description, sizeof((*programs)[i].description), &is_cumul_res_ok);
        }

        if (!is_cumul_res_ok) {
            fclose(file);
            free(*programs);
            *programs = NULL;
            return -1;
        }
    }

    *count = lines;
    fclose(file);
    return 0;
}

/*
   Добавляет новую программу
   programs - указатель на массив структур данных программ,
   count - итоговое количество программ
*/
int addNewProgram(DiagnosticProgram** programs, int* count) {
    if (!programs || !count || *count >= MAX_PROGRAMS_COUNT) return -1;

    DiagnosticProgram* temp = realloc(*programs, (*count + 1) * sizeof(DiagnosticProgram));
    if (!temp) return -1;

    *programs = temp;
    DiagnosticProgram* newProgram = &(*programs)[*count];

    printf("\nДобавление новой программы:\n");
    if (!inputProgram(newProgram)) {
        return -1;
    }

    (*count)++;
    return 0;
}

/*
   Редактирует данные выбранной программы
   programs - указатель на массив структур данных программ,
   count - общее количество программ,
   index - индекс редактируемой программы
*/
int editProgram(DiagnosticProgram* programs, int count, int index) {
    if (!programs || count <= 0 || index < 0 || index >= count) return -1;

    printf("\nРедактирование программы #%d:\n", index + 1);
    if (!inputProgram(&programs[index])) {
        return -1;
    }

    return 0;
}

/*
   Удаляет выбранную программу
   programs - указатель на массив структур данных программ,
   count - итоговое количество программ,
   index - индекс удаляемой программы
*/
int deleteProgram(DiagnosticProgram* programs, int* count, int index) {
    if (!programs || !count || *count <= 0 || index < 0 || index >= *count) return -1;

    for (int i = index; i < *count - 1; i++) {
        programs[i] = programs[i + 1];
    }

    (*count)--;
    return 0;
}

/*
   Выполняет поиск программ по значению одного из полей данных
   programs - указатель на массив структур данных программ,
   count - общее количество программ,
   key - структура данных, содержащая значение определенного поля для поиска программ,
   found - указатель на массив найденных программ,
   foundCount - количество найденных программ
*/
int searchPrograms(DiagnosticProgram* programs, int count, DiagnosticProgram key,
    DiagnosticProgram** found, int* foundCount) {
    if (!programs || count <= 0 || !found || !foundCount) return -1;

    *found = malloc(count * sizeof(DiagnosticProgram));
    if (!*found) return -1;

    *foundCount = 0;
    for (int i = 0; i < count; i++) {
        if ((key.name[0] && strcmp(programs[i].name, key.name) == 0) ||
            (key.manufacturer[0] && strcmp(programs[i].manufacturer, key.manufacturer) == 0) ||
            (key.releaseYear != 0 && programs[i].releaseYear == key.releaseYear) ||
            (key.version != 0 && programs[i].version == key.version)) {
                (*found)[(*foundCount)++] = programs[i];
        }
    }

    if (*foundCount == 0) {
        free(*found);
        *found = NULL;
        return -1;
    }

    return 0;
}

/*
   Сортирует список программ по имени
   programs - указатель на массив структур данных программ,
   count - общее количество программ
*/
int sortProgramsByName(DiagnosticProgram* programs, int count) {
    if (!programs || count <= 0) return -1;
    qsort(programs, count, sizeof(DiagnosticProgram), compareByName);
    return 0;
}

/*
   Сортирует список программ по году выпуска
   programs - указатель на массив структур данных программ,
   count - общее количество программ
*/
int sortProgramsByYear(DiagnosticProgram* programs, int count) {
    if (!programs || count <= 0) return -1;
    qsort(programs, count, sizeof(DiagnosticProgram), compareByYear);
    return 0;
}

/*
   Выводит на экран список всех программ, имеющихся в памяти
   programs - указатель на массив структур данных программ,
   count - общее количество программ
*/
int displayData(DiagnosticProgram* programs, int count) {
    if (!programs || count <= 0) return -1;

    printf("\n%-20s %-20s %-10s %-5s %-11s %-15s\n",
        "Название", "Производитель", "Версия", "Год", "Тип", "ОС");
    printf("---------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-20s %-20s %-10s %-5d %-11s %-15s\n",
            programs[i].name,
            programs[i].manufacturer,
            programs[i].version,
            programs[i].releaseYear,
            programs[i].isFreeware ? "Freeware" : "Commercial",
            programs[i].supportedOS);
        printf("Описание: %s\n\n", programs[i].description);
    }

    return 0;
}
