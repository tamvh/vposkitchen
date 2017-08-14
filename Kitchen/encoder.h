#ifndef ENCODER_H
#define ENCODER_H

typedef struct _permut
{
    unsigned char src;
    unsigned char dst;
    _permut(unsigned char src, unsigned char dst)
    {
        this->src = src;
        this->dst = dst;
    }
} permut;

class Encoder
{
public:
    Encoder();
    static void permute12(char str[], bool en);
    static void encode(char in[], int len, char out[]);
    static void decode(char in[], int len, char out[]);
};

#endif // ENCODER_H
