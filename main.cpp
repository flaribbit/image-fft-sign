#include <iostream>
#include <math.h>
#include "stdio.h"
#include "stdlib.h"
#include "lodepng.h"
#define __USE_SQUARE_BRACKETS_FOR_ELEMENT_ACCESS_OPERATOR
#include "simple_fft/fft.h"
using namespace std;
typedef unsigned char u8;
typedef complex<double> Complex;
typedef vector<Complex> Vec;
typedef vector<Vec> Vec2d;
void ImageResize(u8 *in,int w1,int h1,u8 *out,int w2,int h2){
    double dx=(double)w1/w2,x,
           dy=(double)h1/h2,y;
    int i,j;
    int length=w1*h1*4;
    for(i=0,y=0;i<h2;i++,y+=dy){
        for(j=0,x=0;j<w2;j++,x+=dx){
            int x0=(int)x,y0=(int)y;
            double rx=x-x0,ry=y-y0;
            double f0=(1-rx)*(1-ry),f1=rx*(1-ry),
                   f2=(1-rx)*ry,    f3=rx*ry;
            u8 *p0=in+((y0*w1+x0)<<2),*p1=p0+4,
               *p2=p0+(w1<<2),        *p3=p2+4;
            u8 *p=out+((i*w2+j)<<2);
            if(p1-in>length)p1=in+length-4;
            if(p2-in>length)p2=in+length-4;
            if(p3-in>length)p3=in+length-4;
            *p++=(u8)(f0**p0+++f1**p1+++f2**p2+++f3**p3++);
            *p++=(u8)(f0**p0+++f1**p1+++f2**p2+++f3**p3++);
            *p++=(u8)(f0**p0+++f1**p1+++f2**p2+++f3**p3++);
            *p++=(u8)(f0**p0+++f1**p1+++f2**p2+++f3**p3++);
        }
    }
}

void ImageChannel(u8 *img,int w,int h,int channel,Vec2d &plane){
    for(int i=0;i<h;i++){
        u8 *p=img+i*w*4+channel;
        Vec row(w);
        for(int j=0;j<w;j++,p+=4){
            row[j]=*p;
        }
        plane[i]=row;
    }
}

void ImageSetChannel(u8 *img,int w,int h,int channel,Vec2d plane){
    for(int i=0;i<h;i++){
        u8 *p=img+i*w*4+channel;
        Vec row=plane[i];
        for(int j=0;j<w;j++,p+=4){
            *p=(u8)(row[j].real()/751419785.*255);
        }
    }
    FILE *fp=fopen("1.txt","w");
    for(int i=0;i<2048;i++){
        fprintf(fp,"%lf\n",fabs(plane[i][i].real()));
    }
    fclose(fp);
}

int main(int argc, char const *argv[]){
    u8 *raw;
    unsigned w,h;
    int r=lodepng_decode32_file(&raw,&w,&h,"0.png");
    // puts(lodepng_error_text(r));
    u8 *out=(u8*)malloc(2048*2048*4);
    printf("%d,%d",w,h);
    ImageResize(raw,w,h,out,2048,2048);
    r=lodepng_encode32_file("test0.png",out,2048,2048);
    Vec2d plane(2048);
    const char *error;
    ImageChannel(out,2048,2048,0,plane);
    simple_fft::FFT<Vec2d>(plane,2048,2048,error);
    ImageSetChannel(out,2048,2048,0,plane);

    ImageChannel(out,2048,2048,1,plane);
    simple_fft::FFT<Vec2d>(plane,2048,2048,error);
    ImageSetChannel(out,2048,2048,1,plane);

    ImageChannel(out,2048,2048,2,plane);
    simple_fft::FFT<Vec2d>(plane,2048,2048,error);
    ImageSetChannel(out,2048,2048,2,plane);

    lodepng_encode32_file("test1.png",out,2048,2048);
    puts("OK");
    free(raw);
    free(out);
}
