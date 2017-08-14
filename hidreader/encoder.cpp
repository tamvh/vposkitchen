#include "encoder.h"

const permut permut1[] = {permut(0, 9), permut(1, 3), permut(2, 7), permut(4, 11), permut(5, 6), permut(8, 10), permut(0, 0)};
const permut permut2[] = {permut(0, 5), permut(1, 4), permut(2, 3), permut(6, 10), permut(7, 8), permut(9, 11), permut(0, 0)};
const int key[] = {0,3,8,2,7,9,5,6,1,4};

Encoder::Encoder()
{

}

void Encoder::permute12(char str[], bool en)
{
    char swap;

    if (en)
    {
        for (int i = 0; permut1[i].src != 0 || permut1[i].dst != 0; i++)
        {
            swap = str[permut1[i].src];
            str[permut1[i].src] = str[permut1[i].dst];
            str[permut1[i].dst] = swap;
        }

        for (int i = 0; permut2[i].src != 0  || permut2[i].dst != 0; i++)
        {
            swap = str[permut2[i].src];
            str[permut2[i].src] = str[permut2[i].dst];
            str[permut2[i].dst] = swap;
        }
    }
    else
    {
        for (int i = 0; permut2[i].src != 0  || permut2[i].dst != 0; i++)
        {
            swap = str[permut2[i].src];
            str[permut2[i].src] = str[permut2[i].dst];
            str[permut2[i].dst] = swap;
        }

        for (int i = 0; permut1[i].src != 0 || permut1[i].dst != 0; i++)
        {
            swap = str[permut1[i].src];
            str[permut1[i].src] = str[permut1[i].dst];
            str[permut1[i].dst] = swap;
        }
    }
}

void Encoder::encode(char in[], int len, char out[])
{
    int k, a, b;
    k = in[6] - '0';

    out[6] = in[6];
    for (int i = 0; i < len; i++)
    {
        if (i != 6)
        {
            k = (k % 9) + 1;
            a = in[i] - '0';
            b = a + key[k];
            if (b >= 10)
                b = b - 10;
            out[i] = (char) (b + '0');
            k = k + i + 1;
        }
    }

    permute12(out, true);
}

void Encoder::decode(char in[], int len, char out[])
{
    int k, a, b;

    permute12(in, false);

    k = in[6] - '0';

    out[6] = in[6];
    for (int i = 0; i < len; i++)
    {
        if (i != 6) {
            k = (k % 9) + 1;
            a = in[i] - '0';
            if (a >= key[k])
                b = a - key[k];
            else
                b = a + 10 - key[k];

            out[i] = (char) (b + '0');
            k = k + i + 1;
        }
    }
}
