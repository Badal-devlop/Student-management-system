#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAX_STUDENTS 100
#define TEMP_BUF_SIZE 256

typedef struct
{
    int roll;
    char *name;
    char *department;
    int semester;
    float marks;
} Student;

void addStudent(Student students[], int *count);
void displayStudents(Student students[], int count);
void updateStudent(Student students[], int count);
void deleteStudent(Student students[], int *count);
void filterByDepartment(Student students[], int count);
void freeAllStudents(Student students[], int count);

int findStudentIndex(Student students[], int count);
float readValidMarks(void);
int readValidInt(const char *prompt);
char *readDynamicString(const char *prompt);

void mergeSort(Student arr[], int left, int right);
void merge(Student arr[], int left, int mid, int right);

int binarySearch(Student arr[], int size, int roll);

void findMaxMin(Student arr[], int low, int high, int *maxIndex, int *minIndex);
void createRollSortedCopy(Student students[], int count, Student copy[]);
char *readDynamicString(const char *prompt)
{
    char temp[TEMP_BUF_SIZE];

    printf("%s", prompt);

    if (scanf("%255s", temp) != 1)
    {
        printf("Invalid Input\n");
        temp[0] = '\0';
    }

    char *str = (char *)malloc((strlen(temp) + 1) * sizeof(char));

    if (str == NULL)
    {
        printf("Memory Allocation Failed\n");
        exit(1);
    }

    strcpy(str, temp);
    return str;
}
int readValidInt(const char *prompt)
{
    int value;

    while (1)
    {
        printf("%s", prompt);

        if (scanf("%d", &value) == 1)
        {
            return value;
        }

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        printf("Invalid Input. Please Enter a Whole Number.\n");
    }
}

float readValidMarks(void)
{
    float marks;
    int valid = 0;

    do
    {
        printf("Enter Marks (0-100): ");

        if (scanf("%f", &marks) != 1)
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ;

            printf("Invalid Input. Please Enter a Numeric Value.\n");
            continue;
        }

        if (marks < 0 || marks > 100)
        {
            printf("Invalid Marks. Please Enter a Value Between 0 and 100.\n");
        }
        else
        {
            valid = 1;
        }

    } while (!valid);

    return marks;
}

int findStudentIndex(Student students[], int count)
{
    int searchChoice;

    printf("\nSearch By:\n");
    printf("1. Roll Number\n");
    printf("2. Name\n");
    searchChoice = readValidInt("Enter Choice: ");

    if (searchChoice == 1)
    {
        int roll = readValidInt("Enter Roll Number: ");

        for (int i = 0; i < count; i++)
        {
            if (students[i].roll == roll)
                return i;
        }
    }
    else if (searchChoice == 2)
    {
        char name[TEMP_BUF_SIZE];

        printf("Enter Name: ");
        scanf("%255s", name);

        for (int i = 0; i < count; i++)
        {
            if (strcasecmp(students[i].name, name) == 0)
                return i;
        }
    }
    else
    {
        printf("Invalid Choice\n");
    }

    return -1;
}

void addStudent(Student students[], int *count)
{
    if (*count >= MAX_STUDENTS)
    {
        printf("Student List is Full\n");
        return;
    }

    students[*count].roll = readValidInt("\nEnter Roll Number: ");

    students[*count].name = readDynamicString("Enter Name: ");
    students[*count].department = readDynamicString("Enter Department (e.g. CSE, IT): ");

    students[*count].semester = readValidInt("Enter Semester: ");

    students[*count].marks = readValidMarks();

    (*count)++;

    printf("\nStudent Added Successfully\n");
}

void displayStudents(Student students[], int count)
{
    if (count == 0)
    {
        printf("\nNo Records Found\n");
        return;
    }

    printf("\n-------------------------------------------------------\n");
    printf("Roll\tName\t\tDept\tSem\tMarks\n");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d\t%s\t\t%s\t%d\t%.2f\n",
               students[i].roll,
               students[i].name,
               students[i].department,
               students[i].semester,
               students[i].marks);
    }

    printf("-------------------------------------------------------\n");
}

void updateStudent(Student students[], int count)
{
    if (count == 0)
    {
        printf("No Records Found\n");
        return;
    }

    int index = findStudentIndex(students, count);

    if (index == -1)
    {
        printf("Student Not Found\n");
        return;
    }

    int fieldChoice;

    printf("\nWhat Do You Want to Update?\n");
    printf("1. Name\n");
    printf("2. Roll Number\n");
    printf("3. Department\n");
    printf("4. Semester\n");
    printf("5. Marks\n");
    fieldChoice = readValidInt("Enter Choice: ");

    switch (fieldChoice)
    {
        case 1:
            free(students[index].name);
            students[index].name = readDynamicString("Enter New Name: ");
            break;

        case 2:
            students[index].roll = readValidInt("Enter New Roll Number: ");
            break;

        case 3:
            free(students[index].department);
            students[index].department = readDynamicString("Enter New Department: ");
            break;

        case 4:
            students[index].semester = readValidInt("Enter New Semester: ");
            break;

        case 5:
            students[index].marks = readValidMarks();
            break;

        default:
            printf("Invalid Choice\n");
            return;
    }

    printf("Record Updated Successfully\n");
}

void deleteStudent(Student students[], int *count)
{
    if (*count == 0)
    {
        printf("No Records Found\n");
        return;
    }

    int index = findStudentIndex(students, *count);

    if (index == -1)
    {
        printf("Student Not Found\n");
        return;
    }

    /* free this record's heap strings before it gets overwritten by the shift */
    free(students[index].name);
    free(students[index].department);

    for (int i = index; i < *count - 1; i++)
    {
        students[i] = students[i + 1];
    }

    (*count)--;

    printf("Student Deleted Successfully\n");
}

void filterByDepartment(Student students[], int count)
{
    if (count == 0)
    {
        printf("No Records Found\n");
        return;
    }

    char department[TEMP_BUF_SIZE];

    printf("Enter Department to Filter (e.g. CSE, IT): ");
    scanf("%255s", department);

    int found = 0;

    printf("\n-------------------------------------------------------\n");
    printf("Roll\tName\t\tDept\tSem\tMarks\n");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        if (strcasecmp(students[i].department, department) == 0)
        {
            printf("%d\t%s\t\t%s\t%d\t%.2f\n",
                   students[i].roll,
                   students[i].name,
                   students[i].department,
                   students[i].semester,
                   students[i].marks);

            found = 1;
        }
    }

    printf("-------------------------------------------------------\n");

    if (!found)
    {
        printf("No Students Found in %s Department\n", department);
    }
}

/* Frees the heap-allocated name/department for every live record.
   Call this once, right before freeing the students[] array itself. */
void freeAllStudents(Student students[], int count)
{
    for (int i = 0; i < count; i++)
    {
        free(students[i].name);
        free(students[i].department);
    }
}

void merge(Student arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Student L[MAX_STUDENTS];
    Student R[MAX_STUDENTS];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (L[i].marks >= R[j].marks)
        {
            arr[k++] = L[i++];
        }
        else
        {
            arr[k++] = R[j++];
        }
    }

    while (i < n1)
    {
        arr[k++] = L[i++];
    }

    while (j < n2)
    {
        arr[k++] = R[j++];
    }
}

void mergeSort(Student arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void createRollSortedCopy(Student students[], int count, Student copy[])
{
    for (int i = 0; i < count; i++)
    {
        copy[i] = students[i];
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (copy[j].roll > copy[j + 1].roll)
            {
                Student temp = copy[j];
                copy[j] = copy[j + 1];
                copy[j + 1] = temp;
            }
        }
    }
}

int binarySearch(Student arr[], int size, int roll)
{
    int low = 0;
    int high = size - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (arr[mid].roll == roll)
            return mid;

        if (arr[mid].roll < roll)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return -1;
}

void findMaxMin(Student arr[], int low, int high, int *maxIndex, int *minIndex)
{
    if (low == high)
    {
        *maxIndex = low;
        *minIndex = low;
        return;
    }

    if (high == low + 1)
    {
        if (arr[low].marks > arr[high].marks)
        {
            *maxIndex = low;
            *minIndex = high;
        }
        else
        {
            *maxIndex = high;
            *minIndex = low;
        }

        return;
    }

    int mid = (low + high) / 2;

    int leftMax, leftMin, rightMax, rightMin;

    findMaxMin(arr, low, mid, &leftMax, &leftMin);
    findMaxMin(arr, mid + 1, high, &rightMax, &rightMin);

    *maxIndex = (arr[leftMax].marks > arr[rightMax].marks) ? leftMax : rightMax;
    *minIndex = (arr[leftMin].marks < arr[rightMin].marks) ? leftMin : rightMin;
}

int main()
{
    Student *students = (Student *)malloc(MAX_STUDENTS * sizeof(Student));

    if (students == NULL)
    {
        printf("Memory Allocation Failed\n");
        exit(1);
    }

    int count = 0;
    int choice;

    do
    {
        printf("\n===== STUDENT RESULT MANAGEMENT SYSTEM =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Sort By Marks\n");
        printf("4. Filter By Department\n");
        printf("5. Search By Roll Number\n");
        printf("6. Find Topper and Lowest Scorer\n");
        printf("7. Update Student\n");
        printf("8. Delete Student\n");
        printf("9. Exit\n");

        choice = readValidInt("Enter Choice: ");

        switch (choice)
        {
            case 1:
                addStudent(students, &count);
                break;

            case 2:
                displayStudents(students, count);
                break;

            case 3:
            {
                if (count == 0)
                {
                    printf("No Records Found\n");
                    break;
                }

                int sortChoice;

                printf("\n1. Sort Temporarily (View Only)\n");
                printf("2. Sort Permanently (Overwrite Data)\n");
                sortChoice = readValidInt("Enter Choice: ");

                if (sortChoice == 1)
                {
                    Student *tempCopy = (Student *)malloc(count * sizeof(Student));

                    if (tempCopy == NULL)
                    {
                        printf("Memory Allocation Failed\n");
                        exit(1);
                    }

                    for (int i = 0; i < count; i++)
                    {
                        tempCopy[i] = students[i];
                    }

                    mergeSort(tempCopy, 0, count - 1);

                    printf("\nSorted View (Original Data Unchanged):\n");
                    displayStudents(tempCopy, count);

                    /* tempCopy shares name/department pointers with the
                       original array - free only the array, never the
                       strings, or the original records get double-freed. */
                    free(tempCopy);
                }
                else if (sortChoice == 2)
                {
                    mergeSort(students, 0, count - 1);

                    printf("\nSorted and Saved Successfully:\n");
                    displayStudents(students, count);
                }
                else
                {
                    printf("Invalid Choice\n");
                }

                break;
            }

            case 4:
                filterByDepartment(students, count);
                break;

            case 5:
            {
                if (count == 0)
                {
                    printf("No Records Found\n");
                    break;
                }

                int roll = readValidInt("Enter Roll Number: ");

                Student *copy = (Student *)malloc(count * sizeof(Student));

                if (copy == NULL)
                {
                    printf("Memory Allocation Failed\n");
                    exit(1);
                }

                createRollSortedCopy(students, count, copy);

                int index = binarySearch(copy, count, roll);

                if (index != -1)
                {
                    printf("\nStudent Found\n");
                    printf("Roll       : %d\n", copy[index].roll);
                    printf("Name       : %s\n", copy[index].name);
                    printf("Department : %s\n", copy[index].department);
                    printf("Semester   : %d\n", copy[index].semester);
                    printf("Marks      : %.2f\n", copy[index].marks);
                }
                else
                {
                    printf("Student Not Found\n");
                }

                /* same reasoning as tempCopy above: shared pointers, free only the array */
                free(copy);

                break;
            }

            case 6:
            {
                if (count == 0)
                {
                    printf("No Records Found\n");
                    break;
                }

                int maxIndex;
                int minIndex;

                findMaxMin(students, 0, count - 1, &maxIndex, &minIndex);

                printf("\nTopper:\n");
                printf("%s (%.2f)\n", students[maxIndex].name, students[maxIndex].marks);

                printf("\nLowest Scorer:\n");
                printf("%s (%.2f)\n", students[minIndex].name, students[minIndex].marks);

                break;
            }

            case 7:
                updateStudent(students, count);
                break;

            case 8:
                deleteStudent(students, &count);
                break;

            case 9:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid Choice\n");
        }

    } while (choice != 9);

    freeAllStudents(students, count);
    free(students);

    return 0;
}
