/// Postovani,
/// Molim Vas obratite paznju na korektnost unosa.
/// Kod se verovatno moze uprostiti i jos vise poboljsati,
/// ali je cilj bio u tome da program bude funkcionalan i da
/// uradi trazeni posao.
/// Ovaj program radi sve zadate stavke.
/// Data su upustva za korisnika za sve stavke.
/// S postovanjem,
/// Uros Beric
///
/// P.S.
/// Primer jednog korektnog unosa je: A+B*log(abs((-C)-A),exp(U/V))

/// 0^0 = 1

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#define MAXN 1001

struct tree{   /// stablo
    int data;
    int depth;   /// zapravo predstavlja najduzi put do nekog potomka, za root je maxDepth koja ima ulogu u ispisu
    int level;   /// nivo
    int type;    /// da li je cvor koren, levi ili desni sin ; u racunaju ima ulogu za postorder obilazak ; postorder obilazak
    int diff;    /// za diferenciranje cuva podatak da li cvor sadrzi zadatku promenljivu
    struct tree *left, *right;
};

struct stack{  /// stek za podatke tipa char
    int data;
    struct stack *next;
};

struct stackDouble{
    double data;
    struct stackDouble *next;
};

struct stackTree{   /// stek za podatke tipa struct tree
    struct tree *data;
    struct stackTree *next;
};

struct queue{   /// za racunanje dubine po level-orderu
    struct tree *data;
    struct queue *next;
};

struct defineQueue{
    struct queue *front, *rear;
};

struct tree* getNodeTree(int data) {

    struct tree* node=(struct tree*)malloc(sizeof(struct tree));
    node->data=data;
    node->depth=0;
    node->diff=0;
    node->left=NULL;
    node->right=NULL;
    return node;

}

struct defineQueue* createQueue() {

    struct defineQueue *q=(struct defineQueue*)malloc(sizeof(struct defineQueue));
    q->front=q->rear=NULL;
    return q;

}

int max(int a, int b) {
    return a > b ? a : b;
}

void insert(struct defineQueue **q, struct tree *data) {

    struct queue *node=(struct queue*)malloc(sizeof(struct queue));
    node->data=data;
    node->next=NULL;
    if ((*q)->rear == NULL) (*q)->front=(*q)->rear=node;
    else {
        (*q)->rear->next=node;
        (*q)->rear=node;
    }

}

struct tree* delet(struct  defineQueue **q) {

    struct queue *temp=(*q)->front;
    struct tree *val=(*q)->front->data;
    (*q)->front=(*q)->front->next;
    if ((*q)->front == NULL) (*q)->rear=NULL;
    free(temp);
    return val;

}

int emptyQueue(struct defineQueue **q) {

    return (*q)->rear == NULL;

}

void push(struct stack **top, int data) {

    struct stack* node=(struct stack*)malloc(sizeof(struct stack));
    node->data=data;
    node->next=(*top);
    (*top)=node;

}

void pushDouble(struct stackDouble **top, double data) {

    struct stackDouble* node=(struct stackDouble*)malloc(sizeof(struct stackDouble));
    node->data=data;
    node->next=(*top);
    (*top)=node;

}

void pushTr(struct stackTree **topTr, struct tree *node) {

    struct stackTree* temp=(struct stackTree*)malloc(sizeof(struct stackTree));
    temp->data=node;
    temp->next=(*topTr);
    (*topTr)=temp;

}

int pop(struct stack **top) {

    int val=(*top)->data;
    struct stack* temp=*top;
    *top=(*top)->next;
    free(temp);
    return val;

}

double popDouble(struct stackDouble **top) {

    double val=(*top)->data;
    struct stackDouble* temp=*top;
    *top=(*top)->next;
    free(temp);
    return val;

}

struct tree* popTr(struct stackTree **topTr) {

    struct tree* val=(*topTr)->data;
    struct stackTree* temp=*topTr;
    *topTr=(*topTr)->next;
    free(temp);
    return val;

}

int peek(struct stack **top) {

    return (*top)->data;

}

int isEmpty(struct stack** top) {

    return *top == NULL;

}

int isEmptyTr(struct stackTree** topTr) {

    return *topTr == NULL;

}

int isEmptyDouble(struct stackDouble **top) {

    return *top == NULL;

}

void ocistiEkran() {
    system("cls");
}

void reset(int *poseceni[]) {

    for (int i=0;i<26;i++) poseceni[i]=0;

}

void ispisUnosa() {

    ocistiEkran();
    printf("Unesite aritmeticki izraz u infiksnom obliku bez ikakvih razmaka.\n"
           "Promenjlive se obelezavaju jednoslovnim velikim latinicnim slovima.\n"
           "Ponudjene operacije su unarno - (sa operandom okruzeni zagradama),\n i binarne +, -, *, /, ^.\n"
           "Ponudjene funkcije su log(B), exp i abs koje se unose u formatu\n"
           " log(B,A), exp(A), abs(A), redom, gde je B osnova logaritma, a A je argument funkcije.\n"
           "Usvojimo da uneti izraz nije duzi od 1000 karaktera.\n\n");

}

int glavniMeni() {

    int stavka;
    while (1) {
        printf("\nOdaberite jednu od ponudjenih stavki unosom njenog rednog broja:\n"
               "1. Unesite aritmeticki izraz\n"
               "2. Ispis stabla po nivoima.\n"
               "3. Ispis izraza u prefiksnoj notaciji.\n"
               "4. Izracunavanje izraza uz date vrednosti promenljivih.\n"
               "5. Diferenciranje izraza.\n"
               "6. Izlaz.\n\n");
        scanf("%d", &stavka);
        if (stavka < 1 || stavka > 6) {
            ocistiEkran();
            printf("Uneli ste neodgovarajuci broj. Pokusajte ponovo.\n");
        }
        else return stavka;
    }

}

void postfix(char izraz[], char *post, int *poseceni[], int tabela[][2], int *len) {

    struct stack *top=NULL;
    char waste;
    int pos=0;
    for (int i=0;i<strlen(izraz);i++) {
        if (izraz[i] >= 65 && izraz[i] <= 90) {
            post[pos++]=izraz[i];
            poseceni[izraz[i]-65]=1;
        }
        else if (izraz[i] == 'a' || izraz[i] == 'e' || izraz[i] == 'l') {
            push(&top, izraz[i]);
            i+=2;
        }
        else if (i > 0 && izraz[i-1] == '(' && izraz[i] == '-') {  /// unarno - prelazi u # radi lakseg baratanja
            push(&top, '#');
        }
        else {
            while (!isEmpty(&top) && tabela[peek(&top)][1] >= tabela[izraz[i]][0]) post[pos++]=pop(&top);

            if (izraz[i] == ')') waste=pop(&top);
            else if (izraz[i] != ',') push(&top, izraz[i]);

            if (!isEmpty(&top) && izraz[i] == ')') {
                waste=peek(&top);
                if (waste == 'a' || waste == 'e' || waste == 'l') post[pos++]=pop(&top);
            }
        }
    }
    while (!isEmpty(&top)) post[pos++]=pop(&top);
    *len=pos;

}

void stablo(char post[], int len, struct tree **root) {  /// kreiranje stabla od listova ka korenu

    struct stackTree *topTr=NULL;
    int val1, val2;
    for (int i=0;i<len;i++) {
        if (post[i] >= 65 && post[i] <= 90) {   /// operandi -> eksterni cvorovi
            struct tree *node=getNodeTree(post[i]);
            pushTr(&topTr, node);
        }
        else if (post[i] == 'e' || post[i] == 'a' || post[i] == '#') {   /// unarne operacije, samo levi sin
            struct tree *first=popTr(&topTr);
            struct tree *node=getNodeTree(post[i]);
            node->depth=first->depth+1;
            node->left=first;
            node->right=NULL;
            pushTr(&topTr, node);
        }
        else {   /// binarne operacije
            struct tree *first=popTr(&topTr);
            struct tree *second=popTr(&topTr);
            struct tree *node=getNodeTree(post[i]);
            val1=first->depth;
            val2=second->depth;
            node->depth= val1 > val2 ? val1+1 : val2+1;
            node->left=second;
            node->right=first;
            pushTr(&topTr, node);
        }
    }
    *root=popTr(&topTr);

}

void odrediNivoe(struct tree *root) {  /// svakom cvoru se dodaje njegov nivo, levelorder

    struct defineQueue *q=createQueue();
    struct tree *node=NULL;
    root->level=root->type=0;   /// type odredjuje da li je koren, levi ili desni sin
    insert(&q, root);
    while (!emptyQueue(&q)) {
        node=delet(&q);
        if (node->left != NULL) {
            node->left->level=node->level+1;
            node->left->type=-1;
            insert(&q, node->left);
        }
        if (node->right != NULL) {
            node->right->level=node->level+1;
            node->right->type=1;
            insert(&q, node->right);
        }
    }

}

void identiraj(struct tree *next, int maxDepth) {

    int val=7*(maxDepth-(next->level)+1);  /// 7*... radi boljeg pregleda

    if (next->type == 0) printf("root  ");
    else if (next->type == -1) printf("left  ");
    else printf("right ");

    for (int i=0;i<val;i++) printf("_");
    if (next->data == 'a') printf(" abs\n");
    else if (next->data == 'e') printf(" exp\n");
    else if (next->data == 'l') printf(" log\n");
    else if (next->data == 's') printf(" sgn\n");
    else if (next->data == 'u') printf(" e\n"); /// ojlerov broj, e je vec bilo zauzeto pa da se ne menja vec sve za exp
    else if (next->data == 'n') printf(" ln\n");
    else if (next->data == '#') printf(" -\n");
    else printf(" %c\n", (next->data));

}

void ispisStabla(struct tree *root, int maxDepth) {  /// na preorder nacin

    struct stackTree *topTr=NULL;
    struct tree *next;
    int val;

    printf("Identirana predstava stabla je:\n");
    pushTr(&topTr, root);
    while (topTr != NULL) {
        next=popTr(&topTr);
        while (next != NULL) {
            identiraj(next, maxDepth);
            if (next->right != NULL) pushTr(&topTr, next->right);
            next=next->left;
        }
    }

}

void ispisIzraza(struct tree *root) {  /// ispisuje izraz u prefiksnom obliku, preorder iteracija kroz stablo

    int ch;
    struct tree *next=NULL;
    struct stackTree *stek=NULL;

    ocistiEkran();
    printf("Izraz u prefiksnoj notaciji glasi: ");
    pushTr(&stek, root);
    while (!isEmpty(&stek)) {
        next=popTr(&stek);
        while (next != NULL) {
            ch=(next->data);
            if (ch == 'a') printf("abs");
            else if (ch == 'e') printf("exp");
            else if (ch == 'l') printf("log");
            else if (ch == 's') printf("sgn");
            else if (ch == 'u') printf("e");
            else if (ch == 'n') printf("ln");
            else if (ch == '#') printf("-");
            else printf("%c", ch);

            if ((next->right) != NULL) pushTr(&stek, (next->right));
            next=(next->left);
        }
    }
    printf("\n");

}

void postorderRacunanje(struct tree *root, double val[]) {

    struct stackDouble *top=NULL;  /// stek za racunanje, postorder -> postfix -> rezultat ostaje na kraju
    struct stackTree *topTr=NULL;  /// stek za cvorove, potreban za postorder
    struct tree *node=root;
    double first, second;

    while (node != NULL) {
        node->type=0;  /// 0 - prvi put u stek, 1 - drugi put u stek
        pushTr(&topTr, node);
        node=node->left;
    }
    while (!isEmptyTr(&topTr)) {
        node=popTr(&topTr);
        if (node->type == 0) {   /// samo jednom je cvor bio u steku
            node->type=1;
            pushTr(&topTr, node);
            node=node->right;
            while (node != NULL) {
                node->type=0;
                pushTr(&topTr, node);
                node=node->left;
            }
        }
        else {
            if (node->data >= 65 && node->data <= 90) pushDouble(&top, (double)val[node->data-65]);
            else if (node->data >= 48 && node->data <= 57) pushDouble(&top, (double)(node->data-48));
            else if (node->data == 'u') pushDouble(&top, exp(1));
            else if (node->data == 'a' || node->data == 'e' || node->data == '#' || node->data == 's' || node->data == 'n') {
                first=popDouble(&top);
                if (node->data == 'a') pushDouble(&top, first > 0 ? first : -first);
                else if (node->data == 'e') pushDouble(&top, exp(first));
                else if (node->data == 's') {   /// signum, moze nastati posle diferenciranja funkcije abs
                    if (first > 0) pushDouble(&top, 1.0);
                    else if (first < 0) pushDouble(&top, -1.0);
                    else pushDouble(&top, 0.0);
                }
                else if (node->data == 'n') pushDouble(&top, log(first));  /// n == ln
                else pushDouble(&top, -first);
            }
            else {
                first=popDouble(&top);
                second=popDouble(&top);
                if (node->data == '+') pushDouble(&top, second+first);
                else if (node->data == '-') pushDouble(&top, second-first);
                else if (node->data == '*') pushDouble(&top, second*first);
                else if (node->data == '/') {
                    if (first != 0) pushDouble(&top, second/first);
                    else {
                        printf("\nUps! Doslo je do deljenja nulom!\n");
                        while (!isEmptyDouble(&top)) popDouble(&top);
                        while (!isEmptyTr(&topTr)) popTr(&topTr);
                        return;
                    }
                }
                else if (node->data == '^') pushDouble(&top, pow(second, first));
                else pushDouble(&top, log(first)/log(second));
            }
        }
    }

    printf("\nVrednost izraza je: %lf\n", popDouble(&top));


}

void racunanje(struct tree *root, int poseceni[]) {  /// postorder obilazak i dva steka, jedan za postorder, drugi za racunanje

    double val[26];
    for (int i=0;i<26;i++) val[i]=0;  /// nije neophodno
    ocistiEkran();
    printf("Unesite vrednosti realnih promenljivih:\n(neke promenljive se mozda posle diferenciranja mnoze nulom,\npa nisu pristune na papiru tokom racunanja,\nali jesu u stablu)\n");
    for (int i=0;i<26;i++) {
        if (poseceni[i] == 1) {
            printf("%c = ", i+65);
            scanf("%lf", &val[i]);
        }
    }

    postorderRacunanje(root, val);

}

struct tree *kopiraj(struct tree *root) {   /// kopira po postorderu

    struct stackTree *topTr=NULL, *topCopy=NULL;
    struct tree *node=root;
    while (node != NULL) {
        node->type=0;
        pushTr(&topTr, node);
        node=node->left;
    }
    while (!isEmptyTr(&topTr)) {
        node=popTr(&topTr);
        if (node->type == 0) {
            node->type=1;
            pushTr(&topTr, node);
            node=node->right;
            while (node != NULL) {
                node->type=0;
                pushTr(&topTr, node);
                node=node->left;
            }
        }
        else {
            if (node->data >= 65 && node->data <= 90 || node->data >= 48 && node->data <= 57 || node->data == 'u') {
                struct tree *temp=getNodeTree(node->data);
                temp->depth=node->depth;
                temp->diff=node->diff;
                pushTr(&topCopy, temp);
            }
            else if (node->data == '#' || node->data == 'a' || node->data == 'e' || node->data == 's' || node->data == 'n') {
                struct tree *temp=getNodeTree(node->data);
                struct tree *first=popTr(&topCopy);
                temp->depth=node->depth;
                temp->diff=node->diff;
                temp->left=first;
                pushTr(&topCopy, temp);
            }
            else {
                struct tree *temp=getNodeTree(node->data);
                struct tree *first=popTr(&topCopy);
                struct tree *second=popTr(&topCopy);
                temp->depth=node->depth;
                temp->diff=node->diff;
                temp->left=second;
                temp->right=first;
                pushTr(&topCopy, temp);
            }
        }
    }
    return pop(&topCopy);

}

struct tree *diferencijal(struct tree *root, int *poseceni[]) {  /// postorder, listovi ce biti 0 ili 1, pa se posle gradi stablo

    char diff;
    printf("Postojane promenljive originalnog izraza su: ");
    for (int i=0;i<26;i++) if (poseceni[i] == 1) printf("%c ", i+65);
    printf("\nUnesite promenljivu po kojoj diferencirate: ");
    while ((diff = getchar()) < 65 || diff > 90);

    struct stackTree *topTr=NULL, *topDif=NULL;
    struct tree *node=root;
    int val1, val2;

    while (node != NULL) {
        node->type=node->diff=0;
        pushTr(&topTr, node);
        node=node->left;
    }
    while (!isEmptyTr(&topTr)) {
        node=popTr(&topTr);
        if (node->type == 0) {
            node->type=1;
            pushTr(&topTr, node);
            node=node->right;
            while (node != NULL) {
                node->type=node->diff=0;
                pushTr(&topTr, node);
                node=node->left;
            }
        }
        else {
            if (node->left != NULL && node->right != NULL) node->diff=max(node->left->diff, node->right->diff);
            else if (node->left != NULL) node->diff=node->left->diff;
            else if (node->right != NULL) node->diff=node->right->diff;
            else node->diff=0;

            if (node->data >= 65 && node->data <= 90 || node->data >= 48 && node->data <= 57) {
                struct tree *temp=NULL;
                if (node->data == diff) {
                    temp=getNodeTree('1');
                    node->diff=1;   /// ima diff u sebi, bitno za x^a izvode (da li je x^a ili a^x)
                    temp->diff=1;
                }
                else {
                    temp=getNodeTree('0');
                    node->diff=0;
                    temp->diff=0;
                }
                pushTr(&topDif, temp);
            }
            else if (node->data == 'u') {   /// u=exp(1)
                struct tree *temp=getNodeTree('0');
                node->diff=0;
                temp->diff=0;
                pushTr(&topDif, temp);
            }
            else if (node->data == '#') {  /// unaran minus
                struct tree *first=popTr(&topDif);
                struct tree *temp=getNodeTree('#');
                temp->depth=first->depth+1;
                temp->diff=first->diff;
                temp->left=first;
                pushTr(&topDif, temp);
            }
            else if (node->data == '+' || node->data == '-') {
                struct tree *first=popTr(&topDif);
                struct tree *second=popTr(&topDif);
                struct tree *temp=getNodeTree(node->data);
                temp->depth=max(first->depth, second->depth)+1;
                temp->diff=max(first->diff, second->diff);
                temp->left=second;
                temp->right=first;
                pushTr(&topDif, temp);
            }
            else if (node->data == '*') {
                struct tree *first=popTr(&topDif);
                struct tree *second=popTr(&topDif);
                struct tree *temp=getNodeTree('+');
                struct tree *temp1=getNodeTree('*');
                struct tree *temp2=getNodeTree('*');
                struct tree *temp3=kopiraj(node->left);
                struct tree *temp4=kopiraj(node->right);

                temp1->left=temp3;
                temp1->right=first;
                temp2->left=second;
                temp2->right=temp4;
                temp->left=temp1;
                temp->right=temp2;

                temp1->depth=max(temp3->depth, first->depth)+1;
                temp2->depth=max(second->depth, temp4->depth)+1;
                temp->depth=max(temp1->depth, temp2->depth)+1;

                temp1->diff=max(temp3->diff, first->diff);
                temp2->diff=max(second->diff, temp4->diff);
                temp->diff=max(temp1->diff, temp2->diff);

                pushTr(&topDif, temp);
            }
            else if (node->data == '/') {
                struct tree *first=pop(&topDif);
                struct tree *second=pop(&topDif);
                struct tree *temp=getNodeTree('/');
                struct tree *temp1=getNodeTree('-');
                struct tree *temp2=getNodeTree('^');
                struct tree *temp3=getNodeTree('*');
                struct tree *temp4=getNodeTree('*');
                struct tree *temp5=getNodeTree('2');
                struct tree *temp6=kopiraj(node->left);
                struct tree *temp7=kopiraj(node->right);
                struct tree *temp8=kopiraj(node->right);   /// imace ulogu negde druge nego temp7, ovo se radi kako ne bi bilo preklapanja, temp7 != temp8, dva razlicita cvora

                temp3->left=second;
                temp3->right=temp7;
                temp4->left=temp6;
                temp4->right=first;
                temp1->left=temp3;
                temp1->right=temp4;
                temp2->left=temp8;
                temp2->right=temp5;
                temp->left=temp1;
                temp->right=temp2;

                temp3->depth=max(second->depth, temp7->depth)+1;
                temp4->depth=max(temp6->depth, first->depth)+1;
                temp1->depth=max(temp3->depth, temp4->depth)+1;
                temp2->depth=temp8->depth+1;
                temp->depth=max(temp1->depth, temp2->depth)+1;

                temp3->diff=max(second->diff, temp7->diff);
                temp4->diff=max(temp6->diff, first->diff);
                temp1->diff=max(temp3->diff, temp4->diff);
                temp2->diff=temp8->diff;
                temp->diff=max(temp1->diff, temp2->diff);

                pushTr(&topDif, temp);
            }
            else if (node->data == '^')  {
                struct tree *first=popTr(&topDif);
                struct tree *second=popTr(&topDif);
                if (second->diff == 0 && first->diff == 0) {  /// a^b d/dx
                    struct tree *temp=getNodeTree('0');
                    pushTr(&topDif, temp);
                    free(first);
                    free(second);
                }
                else if (second->diff == 1 && first->diff == 0) { /// x^a d/dx
                    struct tree *temp=getNodeTree('*');
                    struct tree *temp1=getNodeTree('*');
                    struct tree *temp2=second;  /// x'
                    struct tree *temp3=kopiraj(node->right); /// a
                    struct tree *temp4=getNodeTree('^');
                    struct tree *temp5=kopiraj(node->left);
                    struct tree *temp6=getNodeTree('-');
                    struct tree *temp7=kopiraj(node->right);
                    struct tree *temp8=getNodeTree('1');

                    temp6->left=temp7;
                    temp6->right=temp8;
                    temp4->left=temp5;
                    temp4->right=temp6;
                    temp1->left=temp3;
                    temp1->right=temp4;
                    temp->left=temp1;
                    temp->right=temp2;

                    temp6->depth=temp7->depth+1;
                    temp4->depth=max(temp5->depth, temp6->depth)+1;
                    temp1->depth=max(temp3->depth, temp4->depth)+1;
                    temp->depth=max(temp1->depth, temp2->depth)+1;

                    temp6->diff=temp7->diff;
                    temp4->diff=max(temp5->diff, temp6->diff);
                    temp1->diff=max(temp3->diff, temp4->diff);
                    temp->diff=max(temp1->diff, temp2->diff);

                    free(first);
                    pushTr(&topDif, temp);
                }
                else if (second->diff == 0 && first->diff == 1) {   /// a^x d/dx
                    struct tree *temp=getNodeTree('*');
                    struct tree *temp1=getNodeTree('*');
                    struct tree *temp2=getNodeTree('n');  /// ln
                    struct tree *temp3=getNodeTree('^');
                    struct tree *temp4=first;     /// x'
                    struct tree *temp5=kopiraj(node->left); /// a
                    struct tree *temp6=kopiraj(node->left);
                    struct tree *temp7=kopiraj(node->right); /// x

                    temp2->left=temp5;
                    temp3->left=temp6;
                    temp3->right=temp7;
                    temp1->left=temp3;
                    temp1->right=temp4;
                    temp->left=temp1;
                    temp->right=temp2;

                    temp2->depth=temp5->depth+1;
                    temp3->depth=max(temp6->depth, temp7->depth)+1;
                    temp1->depth=max(temp3->depth, temp4->depth)+1;
                    temp->depth=max(temp1->depth, temp2->depth)+1;

                    temp2->diff=temp5->diff;
                    temp3->diff=max(temp6->diff, temp7->diff);
                    temp1->diff=max(temp3->diff, temp4->diff);
                    temp->diff=max(temp1->diff, temp2->diff);

                    free(second);
                    pushTr(&topDif, temp);
                }
                else { /// x^x d/dx
                    printf("\nDoslo je do logaritamskog diferenciranja sto program ne podrzava!\n");
                    free(first);
                    free(second);
                    struct tree *temp=NULL;
                    while (!isEmptyTr(&topDif)) {
                        temp=popTr(&topDif);
                        dealociraj(&temp);
                    }
                    while (!isEmptyTr(&topTr)) popTr(&topTr);
                    return root;
                }
            }
            else if (node->data == 'a') {
                struct tree *first=popTr(&topDif);
                struct tree *temp=getNodeTree('*');
                struct tree *temp1=getNodeTree('s');
                struct tree *temp2=kopiraj(node->left);

                temp1->left=temp2;
                temp->left=temp1;
                temp->right=first;

                temp1->depth=temp2->depth+1;
                temp->depth=max(temp1->depth, first->depth)+1;

                temp1->diff=temp2->diff;
                temp->diff=max(temp1->diff, first->diff);

                pushTr(&topDif, temp);
            }
            else if (node->data == 's') {
                struct tree *first=popTr(&topDif);
                struct tree *temp=getNodeTree('0');
                free(first);
                pushTr(&topDif, temp);
            }
            else if (node->data == 'e') {
                struct tree *first=popTr(&topDif);
                struct tree *temp=getNodeTree('*');
                struct tree *temp1=kopiraj(node);

                temp->left=temp1;
                temp->right=first;

                temp->depth=max(temp1->depth, first->depth)+1;
                temp->diff=max(temp1->diff, first->diff);

                pushTr(&topDif, temp);
            }
            else if (node->data == 'n') {
                struct tree *first=popTr(&topDif);
                if (first->diff == 0) {
                    struct tree *temp=getNodeTree('0');
                    free(first);
                    pushTr(&topDif, temp);
                }
                else {
                    struct tree *temp=getNodeTree('/');
                    struct tree *temp1=kopiraj(node->left);

                    temp->left=first;
                    temp->right=temp1;
                    temp->depth=max(first->depth, temp1->depth)+1;
                    temp->diff=max(first->diff, temp1->diff);

                    pushTr(&topDif, temp);
                }
            }
            else {  /// log
                struct tree *first=popTr(&topDif);
                struct tree *second=popTr(&topDif);
                if (first->diff == 0 && second->diff == 0) {  /// log(a,b) d/dx
                    struct tree *temp=getNodeTree('0');
                    free(first);
                    free(second);
                    pushTr(&topDif, temp);
                }
                else if (first->diff == 1 && second->diff == 0) { /// log(a,x) d/dx
                    struct tree *temp=getNodeTree('/');
                    struct tree *temp1=first;
                    struct tree *temp2=getNodeTree('*');
                    struct tree *temp3=kopiraj(node->right);
                    struct tree *temp4=getNodeTree('n');   /// ln
                    struct tree *temp5=kopiraj(node->left);

                    temp4->left=temp5;
                    temp2->left=temp3;
                    temp2->right=temp4;
                    temp->left=temp1;
                    temp->right=temp2;

                    temp4->depth=temp5->depth+1;
                    temp2->depth=max(temp3->depth, temp4->depth)+1;
                    temp->depth=max(temp1->depth, temp2->depth)+1;

                    temp4->diff=temp5->diff;
                    temp2->diff=max(temp3->diff, temp4->diff);
                    temp->diff=max(temp1->diff, temp2->diff);

                    free(second);
                    pushTr(&topDif, temp);
                }
                else if (first->diff == 0 && second->diff == 1) { /// log(x,a) d/dx
                    struct tree *temp=getNodeTree('/');
                    struct tree *temp1=getNodeTree('#');  /// unarno minus
                    struct tree *temp2=getNodeTree('*');
                    struct tree *temp3=getNodeTree('*');
                    struct tree *temp4=kopiraj(node->left);
                    struct tree *temp5=getNodeTree('^');
                    struct tree *temp6=getNodeTree('n');  /// ln
                    struct tree *temp7=second;
                    struct tree *temp8=getNodeTree('n');  /// ln
                    struct tree *temp9=getNodeTree('2');
                    struct tree *temp10=kopiraj(node->right);
                    struct tree *temp11=kopiraj(node->left);

                    temp6->left=temp10;
                    temp8->left=temp11;
                    temp3->left=temp6;
                    temp3->right=temp7;
                    temp5->left=temp8;
                    temp5->right=temp9;
                    temp2->left=temp4;
                    temp2->right=temp5;
                    temp1->left=temp3;
                    temp->left=temp1;
                    temp->right=temp2;

                    temp6->depth=temp10->depth+1;
                    temp8->depth=temp11->depth+1;
                    temp3->depth=max(temp6->depth, temp7->depth)+1;
                    temp5->depth=max(temp8->depth, temp9->depth)+1;
                    temp2->depth=max(temp4->depth, temp5->depth)+1;
                    temp1->depth=temp3->depth+1;
                    temp->depth=max(temp1->depth, temp2->depth)+1;

                    temp6->diff=temp10->diff;
                    temp8->diff=temp11->diff;
                    temp3->diff=max(temp6->diff, temp7->diff);
                    temp5->diff=max(temp8->diff, temp9->diff);
                    temp2->diff=max(temp4->diff, temp5->diff);
                    temp1->diff=temp3->diff;
                    temp->diff=max(temp1->diff, temp2->diff);

                    free(first);
                    pushTr(&topDif, temp);
                }
                else {   /// log(x1,x2) d/dx
                    struct tree *temp1=first;
                    struct tree *temp2=getNodeTree('n');
                    struct tree *temp3=second;
                    struct tree *temp4=getNodeTree('n');
                    struct tree *temp5=kopiraj(node->left);
                    struct tree *temp6=kopiraj(node->right);

                    temp2->left=temp5;
                    temp4->left=temp6;
                    temp2->depth=temp5->depth+1;
                    temp4->depth=temp6->depth+1;
                    temp2->diff=temp5->diff;
                    temp4->diff=temp6->diff;

                    temp5=getNodeTree('*');
                    temp6=getNodeTree('*');

                    temp5->left=temp1;
                    temp5->right=temp2;
                    temp6->left=temp3;
                    temp6->right=temp4;
                    temp5->depth=max(temp1->depth, temp2->depth)+1;
                    temp6->depth=max(temp3->depth, temp4->depth)+1;
                    temp5->diff=max(temp1->diff, temp2->diff);
                    temp6->diff=max(temp3->diff, temp4->diff);

                    temp1=getNodeTree('/');
                    temp2=getNodeTree('/');
                    temp3=kopiraj(node->right);
                    temp4=kopiraj(node->left);

                    temp1->left=temp5;
                    temp1->right=temp3;
                    temp2->left=temp6;
                    temp2->right=temp4;
                    temp1->depth=max(temp5->depth, temp3->depth)+1;
                    temp2->depth=max(temp6->depth, temp4->depth)+1;
                    temp1->diff=max(temp5->diff, temp3->diff);
                    temp2->diff=max(temp6->diff, temp4->diff);

                    temp5=getNodeTree('-');

                    temp5->left=temp1;
                    temp5->right=temp2;
                    temp5->depth=max(temp1->depth, temp2->depth)+1;
                    temp5->diff=max(temp1->diff, temp2->diff);

                    temp1=getNodeTree('/');
                    temp2=getNodeTree('^');
                    temp3=getNodeTree('n');
                    temp4=getNodeTree('2');
                    temp6=kopiraj(node->left);

                    temp3->left=temp6;
                    temp2->left=temp3;
                    temp2->right=temp4;
                    temp1->left=temp5;
                    temp1->right=temp2;
                    temp3->depth=temp6->depth+1;
                    temp2->depth=max(temp3->depth, temp4->depth)+1;
                    temp1->depth=max(temp5->depth, temp2->depth)+1;
                    temp3->diff=temp6->diff;
                    temp2->diff=max(temp3->diff, temp4->diff);
                    temp1->diff=max(temp5->diff, temp2->diff);

                    pushTr(&topDif, temp1);
                }
            }
        }
    }

    return popTr(&topDif);

}

void dealociraj(struct tree **root) {

    if (*root == NULL) return;

    struct defineQueue *q=createQueue();
    struct tree *node=NULL;
    insert(&q, *root);
    while (!emptyQueue(&q)) {
        node=delet(&q);
        if (node->left != NULL) insert(&q, node->left);
        if (node->right != NULL) insert(&q, node->right);
        free(node);
    }

}

void tabelaPrioriteta(int *tabela[][2]) {

    tabela['+'][0]=tabela['+'][1]=tabela['-'][0]=tabela['-'][1]=2;
    tabela['*'][0]=tabela['*'][1]=tabela['/'][0]=tabela['/'][1]=3;
    tabela['^'][0]=5;
    tabela['^'][1]=4;
    tabela['('][0]=6;
    tabela['('][1]=0;
    tabela[')'][0]=tabela[','][0]=1;
    tabela['a'][1]=tabela['e'][1]=tabela['l'][1]=0;
    tabela['#'][1]=2;  /// unarni minus

}

int main() {

    /// POSTOJI NAPOMENA NA VRHU, MOLIM VAS PROCITAJTE JE

    char izraz[MAXN], post[MAXN];
    int len, poseceni[26], tabela[128][2], maxDepth, postojiUnos=0;
    struct tree* root=NULL, *trash=NULL;

    tabelaPrioriteta(&tabela);

    printf("Dobrodosli!\n");
    while (1) {
        int stavka=glavniMeni();

        if (postojiUnos == 0 && stavka != 1 && stavka != 6) {
            ocistiEkran();
            printf("Niste uneli izraz. Unesite izraz odabirom stavke 1.\n");
            continue;
        }

        if (stavka == 1) {
            reset(&poseceni);
            dealociraj(&root);
            ispisUnosa();
            scanf("%s", &izraz);
            postojiUnos=1;
            postfix(izraz, &post, &poseceni, tabela, &len);  /// infiks -> postfiks -> stablo
            stablo(post, len, &root);  /// kreiranje stabla
            maxDepth=root->depth;  /// maksimalna dubina stabla, ima ulogu u ispisivanju stabla
        }
        else if (stavka == 2) {
            ocistiEkran();
            odrediNivoe(root);  /// svakom cvoru dodeljuje njegov odgovarajuci nivo
            ispisStabla(root, maxDepth);  /// maxDepth da se zna koliko linija da se ispise za svaki cvor
        }
        else if (stavka == 3) ispisIzraza(root);
        else if (stavka == 4) racunanje(root, poseceni);  /// racunanje preko postorder obilaska i steka
        else if (stavka == 5) {
            ocistiEkran();
            trash=root;
            root=diferencijal(root, &poseceni);
            maxDepth=root->depth;
            if (root != trash) dealociraj(&trash);  /// bice jednako ako je doslo do logaritamskog diferenciranja i izraz uopste nije diferenciran
        }
        else {
            dealociraj(&root);
            break;
        }

    }
    ocistiEkran();
    printf("Dovidjenja!\n");

    return 0;

}
