#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#if 0
struct tnode {
    char *str;
    struct tnode *left;
    struct tnode * right;
};


void insert(struct tnode **p, char *value) {

    if(!(*p)) {
        (*p) = (struct tnode *) malloc(sizeof(struct tnode));
        if(!(*p)){
            printf("\nError while memory allocate!\n");
            return;
        }
        (*p)->left = (*p)->right = NULL;
        (*p)->str = strdup(value);
        return;
    }
    if (strcmp(value, (*p)->str) < 0)
        insert(&(*p)->left, value);
    else
        insert(&(*p)->right, value);
    return;
}

void insert(struct tnode_int **p, int value) {

    if(!(*p)) {
        (*p) = (struct tnode *) malloc(sizeof(struct tnode));
        if(!(*p)){
            printf("\nError while memory allocate!\n");
            return;
        }
        (*p)->left = (*p)->right = NULL;
        (*p)->str = strdup(value);
        return;
    }
    if (strcmp(value, (*p)->str) < 0)
        insert(&(*p)->left, value);
    else
        insert(&(*p)->right, value);
    return;
}

void print(struct tnode *root){
    if(root != NULL) {
        print(root->left);
        printf("%s", root->str);
        print(root->right);
    }
}

void array_to_tree(int array[], int n, struct tnode **p ){
    for(int i=0; i < n; i++){
        insert
    }
}

int main() {
    char line[1024];
    struct tnode *root;

    FILE *f;
    setlocale(LC_ALL, "");

    if ((f = fopen("mas.txt", "r") )==NULL) {
        printf("Cannot open file.\n");
        exit (1);
    }

    root = NULL;

    while(!feof(f)) {
        if (fgets(line, 1024, f))
            insert(&root, line);
    }
    fclose(f);
    print(root);

    return 0;
}

#endif

#if 1
struct tnode_int {
    int data;
    struct tnode_int *left;
    struct tnode_int *right;
};

void insert_int(struct tnode_int **p, int value) {

    if(!(*p)) {
        (*p) = (struct tnode_int *) malloc(sizeof(struct tnode_int));
        if(!(*p)){
            printf("\nError while memory allocate!\n");
            return;
        }
        (*p)->left = (*p)->right = NULL;
        (*p)->data = value;
        return;
    }
    if (value < (*p)->data)
        insert_int(&(*p)->left, value);
    else
        insert_int(&(*p)->right, value);
    return;
}

void tree_to_array(struct tnode_int **p, int a[])
  {
    static int idx=0;
    if ((*p)==NULL) return;
    tree_to_array(&((*p)->left), a);       // обход левого поддерева
    a[idx++] = (*p)->data;
    tree_to_array(&(*p)->right, a);      // обход правого поддерева
    free(*p);
  }

void sort_tree(int a[], int n)        // собственно сортировка
{
   struct tnode_int *root;
   int i;

   root = NULL;
   for (i=0; i< n; i++)        // проход массива и заполнение дерева
      insert_int(&root, a[i]);
   tree_to_array(&root, a);             // заполнение массива
}

int main()
{
    int n=0;
    clock_t beg, end;
    char ch;
    double stime;
    srand(time(0));

    for(;;){
        printf("Try to sort?(y/n): ");
        scanf("%c",&ch);
        if(ch == 'n') break;

        printf("Please enter number of elements: ");
        scanf("%d",&n);
        int *a = (int *)malloc(n*sizeof(int));
        if(!a){
            printf("\nError while memory allocate!\n");
            return 0;
        }

        for(int i = 0; i<n; i++){
            a[i] = rand() % 100;
            //  printf("%d  ",a[i]);
        }
        printf("\n");
        beg = clock();
        sort_tree(a, n);
        end = clock();
        stime = (double)(end - beg) / CLOCKS_PER_SEC;
        for(int i = 0; i<n; i++){
            printf("%d  ",a[i]);
        }

        printf("Successful!!!\n");
        //printf("sorted array:\n");
        // for (i=0;i<n;i++) printf("%d  ",a[i]);
        printf("Time = %f\n\n", stime);
        fflush(stdin);
        free(a);
    }
    return 0;
}
#endif


