#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "lodepng.h"
#define __USE_SQUARE_BRACKETS_FOR_ELEMENT_ACCESS_OPERATOR
#include "simple_fft/fft.h"
using namespace std;
int main(int argc, char const *argv[])
{
    vector<vector<complex<double> > >x(4096);
    const char *error;
    for(int i=0;i<4096;i++){
        vector<complex<double> >row(4096);
        for(int j=0;j<4096;j++){
            row[j]=rand()%100*.01;
        }
        x[i]=row;
    }
    simple_fft::FFT(x,4096,4096,error);
    getchar();
    return 0;
}
