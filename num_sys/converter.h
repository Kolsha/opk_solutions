#ifndef CONVERTER_H
#define CONVERTER_H

#define DIGEST "0123456789ABCDEFGHIGKLMNOPQRSTUVWXYZ"

typedef struct tDoubleNumber{
    int whole;
    int offset_part;
    int part;
} DoubleNumber;

DoubleNumber *num2dec(char *num, int from);

char *dec2num(DoubleNumber *num, int to);

#endif // CONVERTER_H
