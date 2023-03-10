#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"

#define MAX_LENGTH 15

/////////////////////////////// STRUCTURI ADITIONALE //////////////////////////////

typedef struct node {
    char* name;
    struct node* next;
} TNode, *TSortedList;
typedef struct QueueNode{
	char* elem;
	struct QueueNode *next;
}QueueNode;
typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

///////////////////////////// FUNCTII ADITIONALE CERINTA 1 ////////////////////////


/* Gaseste angajatul in ierarhie dupa nume */
Tree searchName (Tree tree, char *manager_name){
    Tree tmp = tree, x = NULL;
    int i;

    if(tree == NULL) return NULL;
    if( strcmp(tree->name, manager_name) == 0 ) return tree;

    for(i = 0; i < tree->direct_employees_no && x == NULL; i++){
        x = searchName(*((tmp->team) + i), manager_name);
    }

    return x;
}

/* Aloca spatiu si creeaza un angajat nou, stiind numele si manager-ul */
Tree CreateEmployee(char *name, Tree manager){
    Tree newEmployee;
    newEmployee = (Tree)malloc(sizeof(TreeNode));
    newEmployee->name = name;
    newEmployee->manager = manager;
    newEmployee->direct_employees_no = 0;
    newEmployee->team = malloc(MAX_LENGTH * sizeof(Tree));
    return newEmployee;
}

/* Returneaza pozitia unui angajat intr-o echipa
 * sau pozitia pe care ar trebui sa stea in acea echipa 
 */
int PositionInTeam(Tree m, int nrMemTeam, char *employeeName){
    int pos = 0;
    if(m->direct_employees_no == 0) return pos;
    while (pos < nrMemTeam && strcmp(employeeName, (*((m->team)+pos))->name) > 0 ){
        pos++;
    }
    return pos;   
}

/* Insereaza angajatul in echipa manager-ului pe pozitia determinata alfabetic */
Tree insertInTeam(Tree m, Tree emp, int pos){
    int i = m->direct_employees_no;
    m->direct_employees_no++;

    while(i != pos) {
        (*((m->team)+i)) = (*((m->team)+i-1));
        i--;
    }
    (*((m->team)+pos)) = emp;
    return m;
}

/* Elimina un angajat din echipa manager-ului fara promovarea echipei angajatului eliminat */
Tree eliminateFromTeamLeaf(Tree manager, int pos){
    int i = pos;
    while(i < manager->direct_employees_no - 1){
        (*((manager->team) + i)) = (*((manager->team) + i + 1));
        i++;
    }
    (*((manager->team) + i)) = NULL;
    manager->direct_employees_no--;
    return manager;
}

/* Elimina un angajat din echipa managerului, cu mutarea echipei 
 * angajatului eliminat in echipa managerului
 */
Tree eliminateFromTeamNode(Tree manager, int pos){
    Tree tmp = *((manager->team)+pos);
    int i = 0, poz;
    manager = eliminateFromTeamLeaf(manager, pos);
    for(i = 0; i < tmp->direct_employees_no; i++){
        poz = PositionInTeam(manager, manager->direct_employees_no, (*((tmp->team) + i))->name);
        manager = insertInTeam(manager, *((tmp->team) + i), poz);
        (*((tmp->team) + i))->manager = manager;
    }
    return manager;
}

/* Elibereaza memoria alocata unui arbore de angajati */
void freeTree(Tree tree){
    int i;
    if(tree != NULL) {
        for(i = 0; i < tree->direct_employees_no; i++){
            destroy_tree(*(tree->team + i));
        }
        free(tree->team);
        free(tree);
    }
}


/////////////////////////////////// REZOLVARE CERINTA 1 /////////////////////////////


/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */
Tree hire(Tree tree, char *employee_name, char *manager_name) {
    Tree newEmployee, manager;
    int pos;
    
    if(manager_name == NULL && tree == NULL) {
        return CreateEmployee(employee_name, NULL);
    } else {
        manager = searchName(tree, manager_name);
        newEmployee = CreateEmployee(employee_name, manager);
        pos = PositionInTeam(manager, manager->direct_employees_no, employee_name);
        manager = insertInTeam(manager, newEmployee, pos);
        return tree;
    }
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire(Tree tree, char *employee_name) {
    Tree toBeFired, manager;
    int pos;
    toBeFired = searchName(tree, employee_name);
    if(toBeFired == NULL) return tree;
    if(toBeFired->manager == NULL) return tree;
    manager = toBeFired->manager;
    if(toBeFired->direct_employees_no == 0) {
        if (manager->direct_employees_no == 1){
            manager->direct_employees_no = 0;
            free(toBeFired->team);
            free(toBeFired);
        } else {
            pos = PositionInTeam(manager, manager->direct_employees_no, employee_name);
            manager = eliminateFromTeamLeaf(manager, pos);
            free(toBeFired->team);
            free(toBeFired);
        }
    } else {
        pos = PositionInTeam(manager, manager->direct_employees_no, employee_name);
        manager = eliminateFromTeamNode(manager, pos);
        free(toBeFired->team);
        free(toBeFired);
    }
    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name) {
    Tree toBePromoted, newManager, manager;
    int pos, newPos, i;
    toBePromoted = searchName(tree, employee_name);

    if(toBePromoted == NULL) return tree;
    if(toBePromoted->manager == NULL) return tree;
    if(toBePromoted->manager->manager == NULL) return tree;

    manager = toBePromoted->manager;
    newManager = toBePromoted->manager->manager;
    pos = PositionInTeam(manager, manager->direct_employees_no, toBePromoted->name);
    newPos = PositionInTeam(newManager, newManager->direct_employees_no, toBePromoted->name);

    if (toBePromoted->direct_employees_no == 0) {
        manager = eliminateFromTeamLeaf(manager, pos);
        newManager = insertInTeam(newManager, toBePromoted, newPos);
        toBePromoted->manager = newManager;
        return tree;
    } else {
        manager = eliminateFromTeamNode(manager,pos);
        newManager = insertInTeam(newManager, toBePromoted, newPos);
        toBePromoted->manager = newManager;
        toBePromoted->direct_employees_no = 0;
    }
    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) {
    Tree toBeMoved, newManager, manager;
    int pos, newPos, i;
    toBeMoved = searchName(tree, employee_name);

    if(toBeMoved == NULL) return tree;
    if(toBeMoved->manager == NULL) return tree;
    manager = toBeMoved->manager;
    newManager = searchName(tree,new_manager_name);
    if(toBeMoved->manager == newManager) return tree;

    pos = PositionInTeam(manager, manager->direct_employees_no, toBeMoved->name);
    newPos = PositionInTeam(newManager, newManager->direct_employees_no, toBeMoved->name);

    if (toBeMoved->direct_employees_no == 0) {
        manager = eliminateFromTeamLeaf(manager, pos);
        newManager = insertInTeam(newManager, toBeMoved, newPos);
        toBeMoved->manager = newManager;
        return tree;
    } else {
        manager = eliminateFromTeamNode(manager,pos);
        newManager = insertInTeam(newManager, toBeMoved, newPos);
        toBeMoved->manager = newManager;
        toBeMoved->direct_employees_no = 0;
    }
    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) {

    Tree toBeMoved, newManager, manager;
    int pos, newPos, i;
    toBeMoved = searchName(tree, employee_name);

    if(toBeMoved == NULL) return tree;
    if(toBeMoved->manager == NULL) return tree;
    manager = toBeMoved->manager;
    newManager = searchName(tree, new_manager_name);
    if(toBeMoved->manager == newManager) return tree;

    pos = PositionInTeam(manager, manager->direct_employees_no, toBeMoved->name);
    newPos = PositionInTeam(newManager, newManager->direct_employees_no, toBeMoved->name);

    manager = eliminateFromTeamLeaf(manager, pos);
    newManager = insertInTeam(newManager, toBeMoved, newPos);
    toBeMoved->manager = newManager;

    return tree;
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire_team(Tree tree, char *employee_name) {
    Tree toBeFired = searchName(tree, employee_name);
    Tree manager = toBeFired->manager;
    int pos;
    
    if(manager == NULL) return tree;


    pos = PositionInTeam(manager, manager->direct_employees_no, employee_name);
    manager = eliminateFromTeamLeaf(manager, pos);

    freeTree(toBeFired);

    return tree;
}


///////////////////////////// FUNCTII ADITIONALE CERINTELE 2 SI 3 ////////////////////////

/* Functii pentru lucrul cu lista simple inlantuite ordonate */
TSortedList create(char* newValue) {
    TSortedList list;
    list = (TSortedList)malloc(sizeof(TNode));
    list->name = newValue;
    list->next = NULL;
    return list;
}
TSortedList insert(TSortedList list, char* newValue) {
    TSortedList x, p;
    x = list;
    p = create(newValue);
        if(list == NULL || strcmp(newValue,x->name) < 0) {
            p->next = list;
            return p;
        } else {
            while(x->next != NULL && strcmp(p->name, x->next->name) > 0){
                x = x->next;
                }
            p->next = x->next;
            x->next = p;
        }
    return list;
}
TSortedList freeTSortedList(TSortedList list) {
    TSortedList x, p;
    x = list; p = x;
    while(x != NULL){
        p = x;
        x = x->next;
        free(p);
    }
    list = NULL;
    return list;
}

/* Functii pentru lucrul cu cozi */
Queue* createQueue(void){
	Queue *newQueue;
	newQueue = (Queue*)malloc(sizeof(Queue));
	newQueue->front = NULL;
	newQueue->rear = NULL;
	newQueue->size = 0;
	return newQueue;
} 
void enqueue(Queue *q, char* elem){
	QueueNode *node;
	node = (QueueNode*)malloc(sizeof(QueueNode));
	node->elem = elem;
	node->next = NULL;
	if(q->front == NULL){
		q->front = q->rear = node;
		q->size++;
	}else {
	q->rear->next = node;
	q->rear = node;
	q->size++;
	}
}
char* front(Queue* q){
	return q->front->elem;
}
void dequeue(Queue* q){
	if(q->front == NULL){
		return;
	}
	QueueNode *node;
	node = q->front;
	q->front = q->front->next;
	q->size--;
	if(q->size == 0){
		q->rear = NULL;
	}
	free(node);
}
int isQueueEmpty(Queue *q){

	if(q == NULL){
		return 1;
	}
	if((q->size == 0) && (q->rear == NULL) && (q->front == NULL)){
		return 1;
	}
	return 0;
}
void destroyQueue(Queue *q){
	while(isQueueEmpty(q) == 0) {
		dequeue(q);
	}
	free(q);	
}

/* Adauga recursiv toti angajatii dintr-un arbore
 * intr-o lista ordonata alfabetic.
 */ 
TSortedList getEmployees(TSortedList list,Tree tree) {
    int i;
    if(tree != NULL){
        list = insert(list, tree->name);
        for(i = 0; i < tree->direct_employees_no; i++){
            list = getEmployees(list, *((tree->team) + i));
        }
    }
    return list;
}

/* Afiseaza continutul unei liste in fisier */
void printList(FILE *f, TSortedList list){
    TSortedList tmp = list;
    while(tmp != NULL){
        fprintf(f, "%s ", tmp->name);
        tmp = tmp->next;
    }
}

/* Returneaza o lista a angajatilor cu echipele cele mai mari */
TSortedList getBestEmployees(TSortedList list,Tree root, Tree tree) {
    int i;
    Tree tmp = NULL;
    if(tree != NULL){
        if(list == NULL){
            list = insert(list, tree->name);
        } 
        else if(tree->direct_employees_no == (searchName(root,list->name))->direct_employees_no){
            list = insert(list, tree->name);
        } 
        else if(tree->direct_employees_no > (searchName(root,list->name))->direct_employees_no){
            if(list!=NULL) list = freeTSortedList(list);
            list = insert(list, tree->name);
        }
        for(i=0;i<tree->direct_employees_no;i++){
            list = getBestEmployees(list,root,*((tree->team)+i));
        }
    }
    return list;
}

/* Returneaza o coada cu angajatii de pe urmatorul nivel 
 * Coada primita ca parametru contine angajatii de pe 
 * nivelul curent
 */
Queue* getNextLevel(Queue *q, Tree tree){
    int len, i, j;
    if(q->size == 0){
        enqueue(q, tree->name);
    } else {
        len = q->size;
        for(i = 0; i < len; i++){
            Tree tmp = searchName(tree, front(q));
            for(j = 0; j < tmp->direct_employees_no; j++){
                enqueue(q, (*((tmp->team) + j))->name);
            }
            dequeue(q);
        }
    }
    return q;
}

/* Returneaza o lista sortata cu angajatii din coada primita ca parametru.
 * Scopul este de a sorta continutul cozii
 */
TSortedList sortQueueIntoList(TSortedList list, Queue *q){
    while(q->size!=0){
        list = insert(list, front(q));
        dequeue(q);
    }
    destroyQueue(q);
    return list;
}


/////////////////////////////////// REZOLVARE CERINTELE 2+3 /////////////////////////////


/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    TSortedList list = NULL;
    Tree Employee = searchName(tree, employee_name);
    list = getEmployees(list, Employee);
    printList(f, list);
    list = freeTSortedList(list);
    fprintf(f, "\n");
    return;
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */
void get_employees_by_level(FILE *f, Tree tree, int level) {
    int i;
    TSortedList list = NULL;
    Queue *q = createQueue();
    if(level >= 0){
        for(i = 0; i <= level; i++){
            q = getNextLevel(q, tree);
            if(q->size == 0) break;
        }
    }
    list = sortQueueIntoList(list, q);
    printList(f, list);
    freeTSortedList(list);
    fprintf(f, "\n");
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void get_best_manager(FILE *f, Tree tree) {
    TSortedList list = NULL;
    list = getBestEmployees(list, tree, tree);
    printList(f, list);
    fprintf(f,"\n");
    list = freeTSortedList(list);
    return;
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name) {
    int pos;
    Tree newBoss = searchName(tree, employee_name);
    if(newBoss == NULL) return tree;
    if(newBoss == tree) return tree;
    Tree manager = newBoss->manager;
    if(manager!=NULL) {
        tree = reorganize(tree, manager->name);
    }
    pos = PositionInTeam(manager, manager->direct_employees_no, newBoss->name);
    eliminateFromTeamLeaf(manager, pos);
    pos = PositionInTeam(newBoss, newBoss->direct_employees_no, manager->name);
    insertInTeam(newBoss, manager, pos);
    newBoss->manager = manager->manager;
    manager->manager = newBoss;
    return newBoss;
}


/////////////////////////////////// PARCURGERE SI DISTRUGERE /////////////////////////////


/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal(FILE *f, Tree tree) {
    int i;
    if(tree != NULL) {
        if(tree->manager == NULL && tree->direct_employees_no == 0 ){
            fprintf(f, "%s \n", tree->name);
        } else if(tree->manager == NULL) {
            fprintf(f, "%s ", tree->name);
        } else {
            fprintf(f, "%s-%s ", tree->name, tree->manager->name);
        }
        for(i = 0; i < tree->direct_employees_no; i++) {
            preorder_traversal(f, *((tree->team) + i));
            if(tree->manager == NULL && i == tree->direct_employees_no - 1){
                fprintf(f, "\n");
            }
        }
    }
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) {
    int i;
    if(tree != NULL) {
        for(i = 0; i < tree->direct_employees_no; i++){
            destroy_tree(*(tree->team + i));
        }
        free(tree->team);
        free(tree);
    }
}

