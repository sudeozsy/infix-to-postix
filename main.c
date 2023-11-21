
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*Program calistirilir dosyadan infix ifade okunur ve dosyaya postfix asamalari kaydedilir.
  Bu yuzden tekrar calistirilmadan dosyaninin icine sifirdan infix ifade kaydedilmeli.*/

struct stack
{
    char opr;
    struct stack* next;
};
struct stack* top = NULL;   //ana stack yapısı
struct stack *temp = NULL;

int push(char opr)                  //stack'e eleman ekleme fonks
{
    struct stack* nev = (struct stack*)malloc(sizeof(struct stack));

    if(top == NULL)  //hic eleman yoksa
    {
        nev->opr = opr;
        nev->next = nev;

        top = nev;     //eklenen elemani stack yapısı icin basa alir
    }
    else
    {
        if(top->next == top)  //2 eleman varsa
        {
            nev->opr = opr;
            nev->next = top;
            top->next = nev;

            top= nev;
        }
        else
        {
            temp = top;
            while(temp->next != top)
            {
                temp = temp->next;
            }
            nev->opr = opr;
            temp->next = nev;
            nev->next = top;

            top = nev;
        }

    }
    return 1;
}
int pop()                           //stackten eleman silme fonks
{
    if(top == NULL)
    {
        printf("Stack yapisi bos\n");
        return 1;
    }
    else if(top->next == top)
    {
        top = NULL;
    }
    else
    {
        temp = top;
        while(temp->next != top)
        {
            temp = temp->next;
        }
        temp->next = temp->next->next;
        top = temp->next;
    }
}
int isOperator(char opr)            //operator olup olmadıgını kontrol edip operatorlerin degerini belirler
{
    if(opr == '+' ||opr == '-')
        return 1;
    else if(opr == '*' ||opr == '/')
        return 2;
    else if(opr == '^')
        return 3;
    else
        return -1;
}
char* infixToPostfix(char* infix)  //infixten postixe donusum fonks
{
    char postfix[50];     //postfix ifade kaydedilecek
    int i,j=0;
    FILE *fpostfix;
    fpostfix = fopen("postfix.txt","a");

    for(i=0; i<strlen(infix); i++)
    {
        if(infix[i] == " " || infix[i] == "\t")
            continue;
        if(isdigit(infix[i]))    //sayı olup olmadigigni kontrol ediyor
        {
            postfix[j] = infix[i];   //karakteri postfix dizisine atıyor
            j++;
        }
        else if(infix[i] == '(')
        {
            push(infix[i]);        //stack'e ekliyor
        }
        else if(infix[i] == ')')
        {
            temp = top;

            while(top != NULL && temp->opr != '(')   //parantez kapamaya geldigi zaman diğer operatorleri stacke atmak icin
            {
                postfix[j] = temp->opr;
                pop();          //posfixe atilan operatoru stackten siliyor
                temp =top;
                j++;            //postfix dizisinin bi sonraki karakterlerini ekleyebilmek icin
            }
            pop();              //son atilani da siliyor
        }
        else if(isOperator(infix[i])>0)    //eger operatorun degeri 0dan buyukse gecerli operatorlerden biridir
        {
            temp = top;
            while(top != NULL  && isOperator(temp->opr)>= isOperator(infix[i]))  //operator deger karsilastirmasi buyukse postfixe ekliyor
            {
                postfix[j] = temp->opr;
                pop();
                temp = top;
                j++;
            }
            push(infix[i]);    //stack'e karsilastirma bittikten sonra ekliyor
        }

        temp = top;
        if(top == NULL)
        {
            fprintf(fpostfix,"\nStack: [Bos]\t\t");
        }
        else
        {
            fprintf(fpostfix,"\nStack: ");
            while(temp->next != top)
            {
                fprintf(fpostfix, "%c", temp->opr);   //stack yapisini dosyaya yazdirma icin
                temp = temp->next;
            }
            fprintf(fpostfix, "%c\t\t",temp->opr);
        }

        fprintf(fpostfix, "Postfix: ");

        printf("\nPostfix:");
        for(int k=0; k<j; k++)
        {
            fprintf(fpostfix,"%c",postfix[k]);    //postfixi dosyaya yazdirmak icin
            printf("%c",postfix[k]);              //postfixi ekrana yazdirmak icin
        }
    }
    while(top != NULL)        //en sonda stackte operator kaldıysa ve parantez degilse postfixe ekliyor
    {
        if(top->opr != '(' || top->opr != ')')
        {
            postfix[j] = top->opr;
            pop();
            j++;
        }
    }
    fprintf(fpostfix, "\n\t\t\tPostfix:");
    for(int k=0; k<j; k++)
    {
        fprintf(fpostfix,"%c",postfix[k]);  //son halini dosyaya yazdiriyor
    }

    printf("\nPostfix:");
    for(int k=0; k<j; k++)
    {
        printf("%c",postfix[k]);        //son halini ekrana yazdiriyor
    }

    fclose(fpostfix);
    return postfix;
}
int main()
{
    char inf[50];
    FILE *infix;
    infix = fopen("postfix.txt","r");
    fscanf(infix,"%s",inf);        //dosyadan infix ifadeyi okuyup diziye atiyoruz
    fclose(infix);

    infixToPostfix(inf);

    return 0;
}

