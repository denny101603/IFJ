/*
 * TOHLE JE BERRYHO VETEV, VSICHNI VEN!!!
 */

#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    //ahoj, já jsem carbik a posilam comit
    //Galantní Jelen
    //Berry was here!
    //z toho by se jeden posral(hlavně denny)
    int Meduna[4] =[0, 15, 30, 45];
    int arr_time = 17;
    int shoda = 0;
    for (int i = 0; i < 4; i++)
    {
        if(arr_time == Meduna[i]) shoda = 1;
    }
    if(shoda)
    {
        printf("Vstup povolen. \n");
    }
    else
    {
        printf("A VEN!!!\n");
    }

    return 0;
}