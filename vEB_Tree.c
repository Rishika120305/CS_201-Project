//Thoutam Rishika 2022csb11308
//Gachula Varsha  2022csb1082

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 20
int preferenceSize[500];
int mytree_universe_size;//setting size of vEB tree.
typedef struct student_data {
  int roll_number;
  char name[MAX_NAME_LENGTH];
  int* preference;// college preference data
  int college_allotted;
} student_data;

typedef struct college_data {
  int seats;
  char name[MAX_NAME_LENGTH];
} college_data;

typedef struct vebtree {
  int u;
  int min;
  student_data* min_student;
  int max;
  student_data* max_student;
  struct vebtree* summary;
  struct vebtree** cluster;
} vebtree;

typedef struct {
    char* data;
    int length;
}string;

vebtree* create_vebtree(int size) {
  vebtree* tree = (vebtree*)malloc(sizeof(vebtree));
  tree->u = size;
  tree->min = -1;
  tree->max = -1;
  tree->min_student = NULL;
  tree->max_student = NULL;
  // base size 2 vebtree
  if (size <= 2) {
    tree->summary = NULL;
    tree->cluster = NULL;
  } else {
    int lower_root_u = pow(2, floor((log2(size)) / 2));
    int upper_root_u = pow(2, ceil((log2(size)) / 2));
    /* summary will point to one veb tree with universe size upper_root_u. */
    tree->summary = create_vebtree(upper_root_u);
     /* cluster will point to (upper_root_u) number of veb trees with universe size lower_root_u. */
    tree->cluster = (vebtree**)malloc(upper_root_u * sizeof(vebtree*));
    if (tree->cluster == NULL) {
            free(tree->summary);
            free(tree);
            return NULL; // Handle memory allocation error
        }
    for (int i = 0; i < upper_root_u; i++) {
      tree->cluster[i] = create_vebtree(lower_root_u);
    }
  }

  return tree;
}
// FUNCTIONS
// 1. Function to return cluster number in which k is present.
int high(int x, int big_u) 
{
    int lower_root_u = pow(2, floor((log2(big_u)) / 2));
    return floor((x/lower_root_u));
}
// 2. Function to return position of x in cluster
int low(int x, int big_u) 
{
    int lower_root_u = pow(2, floor((log2(big_u)) / 2));
    return (x%lower_root_u);
}
// 3. Function to return the index from cluster number & index.
int idx(int x, int y, int big_u) 
{
    int lower_root_u = pow(2, floor((log2(big_u)) / 2));
    return (x*lower_root_u + y);
}
// 4. VEB Minimum
int veb_minimum(vebtree *V) 
{ 
  return V->min; 
}
// 5. VEB Maximum
int veb_maximum(vebtree *V)
{
  return V->max;
}
void insert(vebtree *V, int k, student_data* person1) 
{
    if (V->min == -1) 
    {
        V->min = k;
        V->min_student = person1;
        V->max = k;
        V->max_student = person1;
    }
    else
    {
        if (k < V->min) 
        {
            int temp = k;
            k = V->min;
            V->min = temp;
            student_data* tempstudent = person1;
            person1 = V->min_student;
            V->min_student = tempstudent;
        }
        if(V->u > 2) 
        {
            if (veb_minimum(V->cluster[high(k, V->u)]) == -1) 
            {
                insert(V->summary, high(k, V->u), person1);
                V->cluster[high(k, V->u)]->min = low(k, V->u);
                V->cluster[high(k, V->u)]->max = low(k, V->u);
                V->cluster[high(k, V->u)]->min_student = person1;
                V->cluster[high(k, V->u)]->max_student = person1;
            }
            else
            {
                insert(V->cluster[high(k, V->u)], low(k, V->u), person1);
            }
        }
        if (k > V->max){
            V->max = k;
            V->max_student = person1;
        }
    }
}
// 7. Is element 'x' present in veb tree? If yes, what data does it have
int veb_members_check(vebtree *A, int x) 
{
    if (x == A->min) 
    {
        printf("Student Name: %s\n", A->min_student->name);
        printf("Student Roll Number: %d\n", A->min_student->roll_number);
        if ((A->min_student->college_allotted) == -1)
        {
            printf("No college allotted.\n");
        }
        else printf("Code of College and Course Allotted: %d.\n", A->min_student->college_allotted);
        return 1;
    } 
    else if (x == A->max)
    { 
        printf("Student Name: %s\n", A->max_student->name);
        printf("Student Roll Number: %d\n", A->max_student->roll_number);
        if ((A->max_student->college_allotted) == -1)
        {
            printf("No college allotted.\n");
        }
        else printf("Code of College and Course Allotted: %d.\n", A->max_student->college_allotted);
        return 1;
    }
    else if (A->u == 2) 
    {
        return 0;
    }
    else 
    {
        return veb_members_check(A->cluster[high(x, A->u)], low(x, A->u));
    }
}
// 8. Successor of element 'x' in vEB tree.
int veb_tree_successor(vebtree *V, int x)
{
  if (V->u==2)
  {
    if (x==0 && V->max==1)
    {
      return 1;
    }
    else return -1;
  }
  else if ((V->min!=-1) && (x<V->min))
  {
    return V->min;
  }
  else
  {
    int max_in_cluster=veb_maximum(V->cluster[high(x,V->u)]);
    if (max_in_cluster!=-1 && low(x, V->u)<max_in_cluster)
    {
      int offset=veb_tree_successor(V->cluster[high(x,V->u)], low(x, V->u));
      return idx(high(x, V->u), offset, V->u);
    }
    else
    {
      int successor_cluster=veb_tree_successor(V->summary, high(x, V->u));
      if (successor_cluster==-1)
      {
        return -1;
      }
      else
      {
        int offset=veb_minimum(V->cluster[successor_cluster]);
        return idx(successor_cluster, offset, V->u);
      }
    }
  }
}
// 9. Reading until comma from the file
char* reader(FILE* inputFile)
{
    char* data = NULL;
    int dataSize = 0;
    int c;

    while ((c = fgetc(inputFile)) != EOF && c != ',' && c != '\n') {
        data = (char*)realloc(data, dataSize + 2);
        data[dataSize] = (char)c;
        dataSize++;
    }

    if (dataSize > 0) {
        data = (char*)realloc(data, dataSize + 1);
        data[dataSize] = '\0';  // Null-terminate the string
    }

    return data;
}
// 10. Read the whole remaining line from file
void readline(FILE* input)
{
    int c;
    while ((c = fgetc(input)) != EOF && c != '\n') {
        // Read and discard characters until a newline or EOF is encountered
    }
    if (c == EOF && ferror(input)) {
        perror("Error while reading the file");
        exit(1);
    }
}
// 11. Convert string to an integer
int stringtoint(char s[])
{
    int answer = 0;
    int temp = 0;
    for (int i = 0; i<strlen(s); i++)
    {
        temp = s[i] - '0';
        answer = answer * 10 + temp;
    }
    return answer;
}
// 12. Print student data
void printer_student(FILE* outputfile,student_data* person, struct college_data C[])
{
  fprintf(outputfile,"Student Name: %s\n", person->name);
  fprintf(outputfile,"Student Roll No.: %d\n", person->roll_number);
  fprintf(outputfile,"College Allotted: ");
  if (person->college_allotted != -1) {
    fprintf(outputfile,"%s\n", C[person->college_allotted].name);
  } else {
    printf(outputfile,"None.\n");
  }
  return;
}
// 13. Function that gives a student pointer of the index entered.
student_data* veb_tree_student_pointer(vebtree *A, int x)
{ 
    if (x == A->min) 
    {
      return A->min_student;
    } 
    else if (x == A->max)
    { 
      return A->max_student;
    }
    else if (A->u == 2) 
    {
      return NULL;
    }
    else 
    {
      return veb_tree_student_pointer(A->cluster[high(x, A->u)], low(x, A->u));
    }
}
// 14. Function that allots colleges to students.
void college_allotter(student_data* S, struct college_data C[])
{
  int number_of_preferences=preferenceSize[S->roll_number];
  for (int i=0; i<number_of_preferences; i++)
  { 
    int collegecode=(S->preference)[i];
    if ((C[collegecode].seats)!=0)
    {
      S->college_allotted=collegecode;
      (C[collegecode].seats)--;
      return;
    }
    else{
      S->college_allotted=-1;
    }
  }
  return;
}
// End of Functions
int main() {
    FILE* inputfile;
    FILE* college_listfile;
    FILE* outputfile;
    char buffer[255];
    inputfile = fopen("sample-student-data.txt", "r");
    outputfile = fopen("output.txt", "a");
    if (inputfile == NULL) 
    {
        printf("File containing input cannot be opened.\n");
        exit(0);
    }
    //max marks
    char* read=(char*)malloc(100*sizeof(char));
    read = reader(inputfile); // read "Maximum Marks"
    read = reader(inputfile); // read the integer value
    int max_marks = stringtoint(read);
    fscanf(inputfile, "%s", buffer); // read the commas
    fseek(inputfile, 2, SEEK_CUR); // seek to ignore \n
    //max students input
    read = reader(inputfile); //read "Total Students"
    read = reader(inputfile); //read the integer value
    mytree_universe_size = stringtoint(read);
    fscanf(inputfile, "%s", buffer); //read the commas
    fseek(inputfile, 2, SEEK_CUR); // seek to ignore \n
    //make marks array
    vebtree* Score[max_marks + 1];
    for (int i = 0; i < max_marks + 1; i++) 
    {
        Score[i] = create_vebtree(mytree_universe_size);
    }

    //each student input
    for (int j = 0; j < mytree_universe_size; j++) 
    {
      readline(inputfile);
      student_data* temp_student = (student_data*)malloc(sizeof(student_data));
      // student marks
      read = reader(inputfile);//read the integer value
      int person_marks = stringtoint(read);
      // student name
      read = reader(inputfile);
      for (int i = 0; i < 20; i++) 
      {
        temp_student->name[i] = read[i];
      }
      // student roll number
      read = reader(inputfile);
      temp_student->roll_number = stringtoint(read);
      temp_student->preference = NULL;
      // student college choice data
      int c;
      preferenceSize[j]=0;
      while ((read = reader(inputfile)) != NULL)
      {
        preferenceSize[j]++;
        int* newPreference = (int*)realloc(temp_student->preference, preferenceSize[j] * sizeof(int));
        int in=stringtoint(read);
        temp_student->preference = newPreference;
        temp_student->preference[preferenceSize[j] - 1] = in;
      }
      // student college allotted
      temp_student->college_allotted = -1;
      //append student to respective veb tree
      insert(Score[person_marks], temp_student->roll_number, temp_student);
    }
    printf("Insertion to vEB trees complete.\n");
    // COLLEGE LIST INPUT
    college_listfile=fopen("sample-college-data.txt", "r");
    if (college_listfile==NULL)
    {
        printf("File containing colleges cannot be opened.\n");
        exit(0);
    }
    read=reader(college_listfile); //read "Total Courses"
    read=reader(college_listfile); //read the integer value
    int number_of_courses=stringtoint(read);
    fscanf(college_listfile, "%s", buffer); //read the commas
    fseek(college_listfile, 2, SEEK_CUR); // seek to ignore \n
    // initialize all colleges to a pointer.
    struct college_data colleges[number_of_courses+1];
    // store data of all colleges
    for (int i=1; i<number_of_courses+1; i++) 
    { 
      readline(college_listfile);
      read=reader(college_listfile); //read the integer value
      int use_key=stringtoint(read);
      // college + course name
      read=reader(college_listfile);
        for (int i=0; i<20; i++)
        {
          colleges[use_key].name[i]=read[i];
        }
      // college seats
      read=reader(college_listfile);
      colleges[use_key].seats=stringtoint(read);
      //readline(college_listfile);
    }
    // use successor logic to find out next rank and allot college based on preference.
    int previous_roll=0;
    for (int i=max_marks; i>=0;i--)
    { 
      int next_roll=veb_tree_successor(Score[i], previous_roll);
      while (next_roll!=-1)
      {
        student_data* human=veb_tree_student_pointer(Score[i],next_roll);
        college_allotter(human, colleges);
        previous_roll=next_roll;
        next_roll=veb_tree_successor(Score[i], previous_roll);
        printer_student(outputfile,human, colleges);
      }
      previous_roll=0;
    }
  fclose(outputfile);
    // output the allotted colleges.
    printf("Successful execution!\n");
    return 0;
}
