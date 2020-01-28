#include <iostream>
#include <math.h>
#include "stdio.h"
#include "stdlib.h"
#include "lodepng.h"
#define __USE_SQUARE_BRACKETS_FOR_ELEMENT_ACCESS_OPERATOR
#include "simple_fft/fft.h"
using namespace std;
typedef unsigned char u8;
void ImageResize(u8 *in,int w1,int h1,u8 *out,int w2,int h2){
    double dx=(double)w1/w2,x,
           dy=(double)h1/h2,y;
    int i,j;
    for(i=0,x=0;i<w2;i++,x+=dx){
        for(j=0,y=0;j<h2;j++,y+=dy){
            int x0=(int)x,y0=(int)y;
            double rx=x-x0,ry=y-y0;
            double f0=(1-rx)*(1-ry),f1=rx*(1-ry),
                   f2=(1-rx)*ry,    f3=rx*ry;
            int i0=(y0*w1+x0)<<2,    i1=(y0*w1+x0+1)<<2,
                i2=((y0+1)*w1+x0)<<2,i3=((y0+1)*w1+x0+1)<<2;
            u8 *p=out+((j*w2+i)<<2);
            *p++=(u8)(f0*in[i0++]+f1*in[i1++]+f2*in[i2++]+f3*in[i3++]);
            *p++=(u8)(f0*in[i0++]+f1*in[i1++]+f2*in[i2++]+f3*in[i3++]);
            *p++=(u8)(f0*in[i0++]+f1*in[i1++]+f2*in[i2++]+f3*in[i3++]);
            *p++=(u8)(f0*in[i0++]+f1*in[i1++]+f2*in[i2++]+f3*in[i3++]);
        }
    }
}

int main(int argc, char const *argv[]){
    u8 *raw;
    unsigned w,h;
    int error=lodepng_decode32_file(&raw,&w,&h,"0.png");
    printf("%d,%d",w,h);
    u8 *out=(u8*)malloc(2048*2048*4);
    ImageResize(raw,w,h,out,2048,2048);
    ImageResize(out,2048,2048,raw,w,h);
    lodepng_encode32_file("2.png",raw,w,h);
    free(raw);
    free(out);
}

int main_(int argc, char const *argv[])
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
