#ifndef DIAGNOSTIC_DB_H
#define DIAGNOSTIC_DB_H

/*
 * ������ ������ � ����� ������ ��������������� ��������
 * �����: �������� �.�., ������ ����231
 * ������: 2.0
 */

#include <stdio.h>

 /* ������������ ����� ����� */
#define MAX_LENGTH          500
#define MAX_NAME_LEN        100
#define MAX_MANUFACTURER_LEN 100
#define MAX_VERSION_LEN     30
#define MAX_OS_LEN          50
#define MAX_DESC_LEN        500
#define MAX_FILENAME_LEN    256
#define MAX_PROGRAMS_COUNT  100

/*
 * ��������� ��� �������� ���������� � ���������
 */
typedef struct {
    char name[MAX_NAME_LEN];          /* �������� ��������� */
    char manufacturer[MAX_MANUFACTURER_LEN]; /* ������������� */
    char version[MAX_VERSION_LEN];           /* ������ ��������� */
    int releaseYear;                  /* ��� ������� */
    int isFreeware;                   /* ��� �������� (1-freeware, 0-commercial) */
    char supportedOS[MAX_OS_LEN];     /* �������������� �� */
    char description[MAX_DESC_LEN];   /* �������� ��������� */
} DiagnosticProgram;

/* �������� ������� */
int writeDataToFile(DiagnosticProgram* programs, int count, const char* filename);
int readDataFromFile(DiagnosticProgram** programs, int* count, const char* filename);
int addNewProgram(DiagnosticProgram** programs, int* count);
int editProgram(DiagnosticProgram* programs, int count, int index);
int deleteProgram(DiagnosticProgram* programs, int* count, int index);

/* ������� ������ � ���������� */
int searchPrograms(DiagnosticProgram* programs, int count, DiagnosticProgram key,
    DiagnosticProgram** found, int* foundCount);
int sortProgramsByName(DiagnosticProgram* programs, int count);
int sortProgramsByYear(DiagnosticProgram* programs, int count);

/* ������� ����������� */
int displayData(DiagnosticProgram* programs, int count);

#endif /* DIAGNOSTIC_DB_H */