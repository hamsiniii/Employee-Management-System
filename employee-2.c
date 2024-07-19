#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct employee {
    int id;
    char name[50];
    char password[20];
    int pos;
    int domain;
    int age;
    double salary;
    int works_under_id;
} emp;

typedef struct node {
    emp details;
    struct node *left;
    struct node *right;
} node;

typedef struct narynode {
    emp details;
    struct narynode* children[10];
    int numchild;
} narynode;
narynode* createnarynode(emp employee);
narynode* createorgtree(const char *csvname, int rootid);
void addChild(narynode* parent, narynode* child);
void inord(int works_under_id, node *root, int current_id);
void displayorgtree(narynode* root);
void displayorgtreeLevels(narynode* root, int level);
void createdotfile(FILE *fp, node *head);
void displaysiblings(node *head, int id);
void name_tree(node **head);
void insert(node **head, node *newNode);
int validate(int id, node *head);
void detaildisplay(node *head, int id);
double salarycalc(int pos, int domain);
void init_tree_from_csv(node **head, const char *csvname);
void write_to_csv(emp newEmp, const char *csvname);
void findboss(node *head, int id);
void displayboss(node *head, int pos, int domain, int n,int flag);
void displayTree(node *root) ;
node* findemployeebyid(node *head, int id);
int main() {
    node *head1 = NULL;
    init_tree_from_csv(&head1,"employee_data.csv");
    int ch, ch2, id;
    printf("\n🅳🅷🅱🅵 🆃🅴🅲🅷. 🅴🅼🅿🅻🅾🆈🅴🅴 🅼🅰🅽🅰🅶🅴🅼🅴🅽🆃 🆂🆈🆂🆃🅴🅼\n");
    while (1) {
        printf("\n==Main Menu==");
        printf("\n1. New User\n2. Existing User\n3. Exit\n");
        scanf("%d", &ch);
        switch (ch) {
            default:
                printf("Invalid Choice");
                break;
            case 1:
                name_tree(&head1);
                printf("\nInserted!\nWelcome to DHBF Tech. !\nYou can now validate your ID and access other functionalities\n");
            case 2:
                printf("\nEnter your ID:");
                scanf("%d", &id);
                if (validate(id, head1)) {
                    while (1) {
                        printf("\n==Actions==\n");
                        printf("What would you like to execute?\n1. Display my details and salary\n2. Find my boss\n3. Find my coworkers\n4. Search for an employee\n5. View organizational structure\n6. Exit\n");
                        scanf("%d", &ch2);
                        switch (ch2) {
                            case 1:
                                    detaildisplay(head1, id);
                                    break;
                            case 2 : findboss(head1, id);
                                     break;
                            case 3: displaysiblings(head1, id);
                                    break;
                            case 4:
                                    int searchid;
                                    printf("Enter the employee ID to search: ");
                                    scanf("%d", &searchid);
                                    node *foundEmployee = findemployeebyid(head1, searchid);
                                    if (foundEmployee != NULL) {
                                        printf("Employee found:\n");
                                        printf("ID: %d\n", foundEmployee->details.id);
                                        printf("Name: %s\n", foundEmployee->details.name);
                                         if(foundEmployee->details.pos == 1)
                                            printf("Position: Manager\n");
                                        else if(foundEmployee->details.pos == 2)
                                            printf("Position: Team Leader\n");
                                        else if(foundEmployee->details.pos == 3)
                                            printf("Position: Developer\n");        
                                        if(foundEmployee->details.domain == 1)
                                            printf("Domain: Social Media and Marketing\n");
                                        else if(foundEmployee->details.domain == 2)
                                            printf("Domain: Finance\n");
                                        else if(foundEmployee->details.domain == 3)
                                            printf("Domain: IT\n");  
                                    }
                                    else{
                                        printf("Employee not found.\n");
                                    }
                                    break;
                            case 5: FILE *fp = fopen("employee_tree2.dot", "w");
                                    if (fp == NULL) {
                                        fprintf(stderr, "Error opening file for writing.\n");
                                        return 1;
                                    }
                                    fprintf(fp, "digraph EmployeeTree {\n");
                                    fprintf(fp, "\tCEO [label=\"CEO\"];\n");
                                    createdotfile(fp, head1);
                                    fprintf(fp, "}\n");
                                    fclose(fp);
                                    int rootid = 0; 
                                    narynode* naryRoot = createorgtree("employee_data.csv", rootid);

                                    if (naryRoot != NULL) {
                                        printf("N-ary Tree created from CSV:\n");
                                        displayorgtree(naryRoot);
                                    } else {
                                        printf("Failed to create the N-ary tree.\n");
                                    }
                                    printf("\nGenerate image from employee_tree2.dot for better visualisation!\n "); 
                                    break;
                            case 6 : break;
                            default: printf("invalid input.");                           
                        }
                        if(ch2==6)break;
                        
                    }
                } else
                    {printf("\nYou don't have access to this information\n");
                break;}
                case 3:exit(0);}
    return 0;
}
}
void name_tree(node **head) {
    emp newEmp;
    printf("Enter Employee Details:\n");
    int flag = 1;
    while (flag) {
        printf("ID: ");
        scanf("%d", &newEmp.id);
        node *temp = *head;
        while (temp != NULL) {
            if (temp->details.id == newEmp.id) {
                printf("ID already exists. Please try again.\n");
                break;
            }
            temp = (temp->details.id < newEmp.id) ? temp->right : temp->left;
        }
        if (temp == NULL)
            flag = 0;
    }

    printf("Name: ");
    char c;
    while ((c = getchar()) != '\n' && c != EOF); 
    fgets(newEmp.name, sizeof(newEmp.name), stdin);
    newEmp.name[strcspn(newEmp.name, "\n")] = '\0'; 

    printf("Password: "); 
    scanf("%s", newEmp.password);

    printf("Position (1-Manager, 2-Team Lead, 3-Developer): ");
    scanf("%d", &newEmp.pos);
    printf("Domain (1-Social Media and Marketing, 2-Finance, 3-IT): ");
    scanf("%d", &newEmp.domain);
    printf("Age: ");
    scanf("%d", &newEmp.age);
    printf("Works under (Enter ID of the person you work under, 0 if you are a manager): "); 
    scanf("%d", &newEmp.works_under_id); 
    newEmp.salary = 0;

    node *newNode = (node *)malloc(sizeof(node));
    newNode->details = newEmp;
    newNode->left = NULL;
    newNode->right = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        insert(head, newNode);
    }

    write_to_csv(newEmp, "employee_data.csv");
}


void insert(node **head, node *newNode) {
    if (*head == NULL) {
        *head = newNode;
    } else {
        if (newNode->details.id < ((*head)->details.id)) {
            insert(&((*head)->left), newNode);
        } else {
            insert(&((*head)->right), newNode);
        }
    }
}

int validate(int id, node *head) {
    if (head == NULL) {
        return 0;
    } else if (id == head->details.id) {
        char password[20];
        printf("Enter your password: ");
        scanf("%s", password);

        if (strcmp(password, head->details.password) == 0) {
            return 1; 
        } else {
            printf("Incorrect password. Access denied.\n");
            return 0; 
        }
    } else if (id < head->details.id) {
        return validate(id, head->left);
    } else {
        return validate(id, head->right);
    }
}


void detaildisplay(node *head, int id) {
    if (head == NULL) {
        printf("Oops! Something is wrong. Try again later.");
    } else if (id == head->details.id) {
        printf("\nDetails:\n");
        printf("ID: %d \n", head->details.id);
        printf("Name: %s \n", head->details.name);
        if(head->details.pos == 1)
            printf("Position: Manager\n");
        else if(head->details.pos == 2)
            printf("Position: Team Leader\n");
        else if(head->details.pos == 3)
            printf("Position: Developer\n");        
        if(head->details.domain == 1)
            printf("Domain: Social Media and Marketing\n");
        else if(head->details.domain == 2)
            printf("Domain: Finance\n");
        else if(head->details.domain == 3)
            printf("Domain: IT\n");  
        printf("Age: %d \n", head->details.age);
        head->details.salary = salarycalc(head->details.pos, head->details.domain);
        printf("Salary: %.2f \n", head->details.salary);  
    } else if (id < head->details.id) {
        detaildisplay(head->left, id);
    } else {
        detaildisplay(head->right, id);
    }
}

double salarycalc(int pos, int domain) {
    double baseSalary = 50000;
    double posSalary = 0;
    double domainSalary = 0;

    switch (pos) {
        case 1:
            posSalary = 20000;
            break;
        case 2:
            posSalary = 15000;
            break;
        case 3:
            posSalary = 10000;
            break;
    }

    switch (domain) {
        case 1:
            domainSalary = 5000;
            break;
        case 2:
            domainSalary = 4000;
            break;
        case 3:
            domainSalary = 3000;
            break;
    }

    return baseSalary + posSalary + domainSalary;
}

void init_tree_from_csv(node **head, const char *csvname) {
    FILE *csvFile = fopen(csvname, "r");
    if (!csvFile) {
        fprintf(stderr, "Error opening CSV file: %s\n", csvname);
        return;
    }

    emp newEmp;

    while (fscanf(csvFile, "%d,%49[^,],%49[^,],%d,%d,%d,%lf,%d\n", &newEmp.id, newEmp.name, newEmp.password, &newEmp.pos, &newEmp.domain, &newEmp.age, &newEmp.salary, &newEmp.works_under_id) == 8) {

        node *newNode = (node *)malloc(sizeof(node));
        newNode->details = newEmp;
        newNode->left = NULL;
        newNode->right = NULL;

        if (*head == NULL) {
            *head = newNode;
        } else {
            insert(head, newNode);
        }
    }

    fclose(csvFile);
}
void write_to_csv(emp newEmp, const char *csvname) {
    FILE *csvFile = fopen(csvname, "a");
    if (!csvFile) {
        fprintf(stderr, "Error opening CSV file: %s\n", csvname);
        return;
    }

    fprintf(csvFile, "%d,%s,%s,%d,%d,%d,%f,%d\n", newEmp.id, newEmp.name, newEmp.password, newEmp.pos, newEmp.domain, newEmp.age, newEmp.salary,newEmp.works_under_id);

    fclose(csvFile);
}
void displayboss(node *head, int pos, int domain, int n,int flag) {
    if (head == NULL) {
        return;
    }
    if (head->details.domain == domain && head->details.pos < pos) {
        printf("\nSuperior %d in your domain:\n" , n++);
        printf("ID: %d\n", head->details.id);
        printf("Name: %s\n", head->details.name);
        if(head->details.pos == 1)
        printf("Position: Manager\n");
        else if(head->details.pos == 2)
        printf("Position: Team Leader\n");
        else if(head->details.pos == 3)
        printf("Position: Developer\n");
    }
    else{
        if (flag==1) {
            printf("You report to CEO.\n");
            flag++;
        }
    }
    displayboss(head->left, pos, domain, n,flag);
    displayboss(head->right, pos, domain, n,flag);
}
void findboss(node *head, int id) {
    if (head == NULL) {
        printf("Employee not found.\n");
    } else {
        node *current = head;
        while (current != NULL && current->details.id != id) {
            if (id < current->details.id) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (current == NULL) {
            printf("Employee not found.\n");
        } else {
            int n= 1;
            displayboss(head, current->details.pos, current->details.domain , n,1);
        }
    }
}
node* findemployeebyid(node *head, int id) {
    if (head == NULL || head->details.id == id) {
        return head;
    }

    if (id < head->details.id) {
        return findemployeebyid(head->left, id);
    } else {
        return findemployeebyid(head->right, id);
    }
}

void createdotfile(FILE *fp, node *head) {
    if (head == NULL) {
        return;
    }

    const char* colors[] = {"blue", "green", "red"}; 

    if (head->details.pos == 1) {
        fprintf(fp, "\tCEO -> %d [label=\"Manager: %s\", color=\"%s\"];\n", head->details.id, head->details.name, colors[head->details.domain - 1]);
    } else if (head->details.pos == 2) {
        fprintf(fp, "\t%d [label=\"Team Lead: %s\", color=\"%s\"];\n", head->details.id, head->details.name, colors[head->details.domain - 1]);
        fprintf(fp, "\t%d -> %d;\n", head->details.works_under_id, head->details.id);
        }
    else if (head->details.pos == 3) {
        fprintf(fp, "\t%d [label=\"Developer: %s\", color=\"%s\"];\n", head->details.id, head->details.name, colors[head->details.domain - 1]);
        fprintf(fp, "\t%d -> %d;\n", head->details.works_under_id, head->details.id);
    }
    createdotfile(fp, head->left);
    createdotfile(fp, head->right);
}

narynode* createnarynode(emp employee) {
    narynode* newNode = (narynode*)malloc(sizeof(narynode));
    if (newNode != NULL) {
        newNode->details = employee;
        newNode->numchild = 0;
        for (int i = 0; i < 10; ++i) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

void addChild(narynode* parent, narynode* child) {
    if (parent != NULL && child != NULL && parent->numchild < 10) {
        parent->children[parent->numchild++] = child;
    }
}

void displayorgtreeLevels(narynode* root, int level) {
    if (root == NULL) {
        return;
    }

    static int prevLevel = -1;

    if (level != prevLevel) {
        printf("\nLevel %d:\n", level);
        prevLevel = level;
    }

    printf("ID: %d, Name: %s\n", root->details.id, root->details.name);

    for (int i = 0; i < root->numchild; ++i) {
        displayorgtreeLevels(root->children[i], level + 1);
    }
}

void displayorgtree(narynode* root) {
    if (root == NULL) {
        printf("N-ary tree is empty.\n");
    } else {
        printf("Displaying N-ary tree by grouped levels:\n");
        displayorgtreeLevels(root, 0);
    }
}

void displaysiblings(node *head, int id) {
    if (head == NULL) {
        printf("No other user reports to the same person as you. We respect your Hardwork!\n");
        return;
    }

    node *current = head;
    while (current != NULL && current->details.id != id) {
        if (id < current->details.id) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (current == NULL) {
        printf("No other user reports to the same person as you. We respect your Hardwork!\n");
        return;
    }

    printf("\nEmployees who report to the same person:\n");
    inord(current->details.works_under_id, head, id);
}

void inord(int works_under_id, node *root, int current_id) {
    if (root == NULL) {
        return;
    }

    inord(works_under_id, root->left, current_id);

    if (root->details.works_under_id == works_under_id && root->details.id != current_id) {
        printf("ID: %d, Name: %s\n", root->details.id, root->details.name);
    }

    inord(works_under_id, root->right, current_id);
}
narynode* createorgtree(const char *csvname, int rootid) {
    FILE *csvFile = fopen(csvname, "r");
    if (!csvFile) {
        fprintf(stderr, "Error opening CSV file: %s\n", csvname);
        return NULL;
    }

    narynode* rootNode = NULL;
    narynode* employeeNodes[1000];
    int numEmployees = 0;

    emp newEmp;

    while (fscanf(csvFile, "%d,%49[^,],%19[^,],%d,%d,%d,%lf,%d\n", &newEmp.id, newEmp.name, newEmp.password, &newEmp.pos, &newEmp.domain, &newEmp.age, &newEmp.salary, &newEmp.works_under_id) == 8) {
        narynode* newEmployee = createnarynode(newEmp);
        employeeNodes[numEmployees++] = newEmployee;
        if (newEmp.id == rootid) {
            rootNode = newEmployee; 
        }
    }

    fclose(csvFile);

    if (rootNode == NULL) {
        fprintf(stderr, "Root ID not found in CSV file.\n");
        return NULL;
    }

    for (int i = 0; i < numEmployees; ++i) {
        if (employeeNodes[i]->details.id != rootid) {
            int worksUnderId = employeeNodes[i]->details.works_under_id;
            for (int j = 0; j < numEmployees; ++j) {
                if (employeeNodes[j]->details.id == worksUnderId) {
                    addChild(employeeNodes[j], employeeNodes[i]);
                    break;
                }
            }
        }
    }

    return rootNode;
}
