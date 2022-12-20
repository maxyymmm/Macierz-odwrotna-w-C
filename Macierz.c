#include<malloc.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct 
{
    int wiersze, kolumny;
    float *elementy;
} macierz;

float wyznacznik(int rozmiar, float *ptr)
{
    float wynik = 0, wynik_wew, wyznacznik_wew;
    float a, b, c, d;
   
    if((rozmiar==1)||(rozmiar==2)) //Warunek STOP. Oblicza ostatni wyznacznik 2x2
    {
        if(rozmiar==1)
            wynik =*ptr;
        else
        {
            a = *ptr;
            b = *(ptr+1);
            c = *(ptr+2);
            d = *(ptr+3);
            wynik = (a * d) - (b * c);
        }
    }
    else
    {
        float podstawa; // podstawa - liczba względem której rozwijamy
        float element; // element do podmacierzy
        float znak = 1;
        float *podmacierz = calloc((rozmiar-1)*(rozmiar-1), sizeof(float));

        for(int i=0; i<rozmiar; i++)
        {
            int index=0; //index macierzy głównej. Potrzebne do elementu - skacze co 1
            int index_pod=0; //index podmacierzy -  skacze co 1 do podmacierzy
            podstawa = *(ptr+i);

            for(int j=0; j<rozmiar; j++)
            {
                for(int k=0; k<rozmiar; k++)
                {
                    element = *(ptr+index);
                    if((j==0) || (i==k)); // j = 0, bo rozwijamy zawsze wzgledem 1 wiersza, j-wiersz, k-kolumna
                    else
                    {
                        *(podmacierz+index_pod) = element;
                        index_pod++; //iteruje indexy podmacierzy co 1
                    }
                    index++; // iteruje indexy macierzy głównej co 1
                }
            }
            wyznacznik_wew = wyznacznik(rozmiar-1,podmacierz);  //Wywołuje rekurencje. Podaje macierz do obliczenia wyznacznika o rozmiarze: rozmiar-1
            wynik_wew = znak * podstawa * wyznacznik_wew; //wynik wyznacznika macierzy wewnetrznej
            wynik = wynik + wynik_wew;
            znak = znak * (-1); //(-1)^kolumna
        }
        free(podmacierz);
    }
    return wynik;
}

macierz transponowanie(macierz T)
{
    macierz transponowanie = (macierz) {.wiersze = T.kolumny, .kolumny= T.wiersze, .elementy = calloc(T.wiersze* transponowanie.kolumny, sizeof(float) )};
   
    for(int i=0; i<T.kolumny; i++) //transponowanie
    {
        for(int k =0; k<T.wiersze; k++)
        {
            transponowanie.elementy[i*T.wiersze+k] = T.elementy[k*T.kolumny+i];
        }
    }
    return transponowanie;
}

void print_macierz(macierz P)
{
    for(int i=0; i<P.wiersze*P.kolumny; i++)
    {
        printf("%12.2f",*(P.elementy+i));
        if((i+1)%P.kolumny == 0)
            printf("\n");
    }
}

macierz dopelnienie(macierz A)

{
    macierz dopelnienie = (macierz) {.wiersze = A.wiersze, .kolumny= A.kolumny, .elementy = calloc(A.wiersze* A.kolumny,sizeof(float) )};
   
    if (A.kolumny != A.wiersze);
    else
    {
        if (A.kolumny==2)
        {
            int rozmiar = A.wiersze*A.kolumny;
            for(int i=0; i<A.wiersze*A.kolumny; i++)
            {
                dopelnienie.elementy[i]=A.elementy[rozmiar-1];
                rozmiar--;
            }
        }
        else
        {
            float element; // element do podmacierzy
            float znak = 1;
            float *podmacierz = calloc((A.kolumny-1)*(A.kolumny-1), sizeof(float));
            int wiersz=0; //Potrzebne do sprawdzenie w if'ie (odpowiada za wykreślanie wierszy j==wiersz)
            int index_dop=0; //index dopełnienia

            for(int m=0; m<A.kolumny; m++)
            {
                for(int i=0; i<A.kolumny; i++)
                {
                    int index=0; //index macierzy głównej. Potrzebne do elementu - skacze co 1
                    int index_pod=0; //index podmacierzy -  skacze co 1 do podmacierzy

                    for(int j=0; j<A.kolumny; j++)
                    {
                        for(int k=0; k<A.kolumny; k++)
                        {
                            element = A.elementy[index];
                            if((j==wiersz) || (i==k)); // j = 0, bo rozwijamy zawsze wzgledem 1 wiersza, j-wiersz, k-kolumna
                            else
                            {
                                *(podmacierz+index_pod) = element;
                                index_pod++; //iteruje indexy podmacierzy co 1
                            }
                            index++; // iteruje indexy macierzy głównej co 1
                        }
                    }
                    dopelnienie.elementy[index_dop] = znak * wyznacznik(A.kolumny-1, podmacierz);
                    znak = znak * (-1); //(-1)^kolumna
                    index_dop++;
                }
                wiersz++;
            }
            free(podmacierz);
        }
    }
    return dopelnienie;
}

macierz odwracanie(macierz A)
{  
    macierz odwrotna = (macierz) {.wiersze = A.wiersze, .kolumny= A.kolumny, .elementy = calloc(A.wiersze* A.kolumny, sizeof(float) )};
    float wyz = wyznacznik(A.kolumny,A.elementy);
    if(wyz == 0)
        printf("Wyznacznik = 0. Nie mozna odwrocic macierzy\n");
    else if (A.kolumny != A.wiersze)
        printf("Nie mozna odwrocic macierzy. Macierz nie jest kwadratowa");
    else if (A.kolumny == 1)
    {
        odwrotna.elementy[0] = 1/A.elementy[0];
    }
    else
    {
        odwrotna = dopelnienie(A);
        printf("Macierz dopelenien =\n");
        print_macierz(odwrotna);
        printf("\n");

        printf("Macierz transponowana =\n");
        odwrotna = transponowanie(odwrotna);
        print_macierz(odwrotna);
        printf("\n");

        for(int i=0; i<A.wiersze*A.kolumny; i++)
        {
            odwrotna.elementy[i] = (1/wyz) * odwrotna.elementy[i];
        }
    }
    return odwrotna;
}

macierz mnozenie(macierz A, macierz B)

{
    if (A.kolumny != B.wiersze);

    macierz iloczyn = (macierz) {.wiersze = A.wiersze, .kolumny= B.kolumny, .elementy = calloc(A.wiersze* B.kolumny, sizeof(float) )};

    for(int i = 0; i<A.wiersze ; i++)
    {
        for(int j = 0; j<A.kolumny; j++)
        {
            for(int k = 0; k<B.wiersze; k++)  
            {
                iloczyn.elementy[i*A.kolumny+j] += A.elementy[k+i*A.kolumny] * B.elementy[k*B.wiersze+j];
                // printf("%d | %d | %d\n", i*A.kolumny+j, k+i*A.kolumny, k*B.wiersze+j);
            }
        }
    }
    return iloczyn;
}

void main()
{
    int liczba = 10;

    macierz mA = {.wiersze = liczba, .kolumny = liczba, .elementy = calloc(liczba*liczba, sizeof(float) )};
   
    time_t t;
   
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    /* Print 5 random numbers from 0 to 49 */
    for(int i = 0 ; i < mA.wiersze*mA.kolumny ; i++ ) 
    {
        liczba = rand() % 50;
        mA.elementy[i] = liczba;
    }


    printf("Macierz = \n");
    print_macierz(mA);
    printf("\n");

    macierz odwrocona = odwracanie(mA);

    printf("Macierz odwrotna = \n");          
    print_macierz(odwrocona);
    printf("\n");

    macierz jednostkowa = mnozenie(mA, odwrocona);
    
    printf("Macierz jednostkowa = \n");
    print_macierz(jednostkowa);

}