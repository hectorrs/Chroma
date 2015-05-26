
#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <emmintrin.h>

typedef __m128i m128i;

typedef union {
    __m128i mm;
    int si[4];
    unsigned int usi[4];
    short hi[8];
    unsigned short uhi[8];
    signed char qi[16];
    unsigned char uqi[16];
} __attribute__((aligned(16))) um128i;

int main(int argc, char** argv) {

    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* Chroma = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);

    if (!Chroma) {
        printf("Error: file %s not found\n", argv[1]);
        return EXIT_FAILURE;
    }


    IplImage* Paisaje = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

    if (!Paisaje) {
        printf("Error: file %s not found\n", argv[1]);
    }
    
     // PAISAJE DE CUATRO CANALES:
    int fila, columna;
    IplImage* Paisaje4 = cvCreateImage(cvSize(Paisaje->width, Paisaje->height), Paisaje->depth, 4);
    for (fila = 0; fila < Paisaje->height; fila++) {
        unsigned char *pPaisaje4 = (unsigned char *) Paisaje4->imageData + fila *
                Paisaje4->widthStep;
        unsigned char *pPaisaje = (unsigned char *) Paisaje->imageData + fila *
                Paisaje->widthStep;
        for (columna = 0; columna < Paisaje->width; columna++) {
            *pPaisaje4++ = *pPaisaje++;
            *pPaisaje4++ = *pPaisaje++;
            *pPaisaje4++ = *pPaisaje++;
            *pPaisaje4++ = 0;
        }
    }
    cvNamedWindow("Paisaje4", 1);
    cvShowImage("Paisaje4", Paisaje4);
    cvWaitKey(0);



    // CHROMA DE CUATRO CANALES:
    IplImage* Chroma4 = cvCreateImage(cvSize(Chroma->width, Chroma->height), Chroma->depth, 4);
    for (fila = 0; fila < Chroma->height; fila++) {
        unsigned char *pChroma4= (unsigned char *) Chroma4->imageData + fila *
                Chroma4->widthStep;
        unsigned char *pChroma = (unsigned char *) Chroma->imageData + fila *
                Chroma->widthStep;
        for (columna = 0; columna < Chroma->width; columna++) {
            *pChroma4++ = *pChroma++;
            *pChroma4++ = *pChroma++;
            *pChroma4++ = *pChroma++;
            *pChroma4++ = 0;
        }
    }
    cvNamedWindow("Chroma4", 1);
    cvShowImage("Chroma4", Chroma4);
    cvWaitKey(0);
    
    
    //MASCARA DE CUATRO CANALES
    IplImage* mascara4 = cvCreateImage(cvSize(Chroma4->width, Chroma4->height), Chroma4->depth, 4);
    
    unsigned char valor = 20;
    
    int verde = *((int *) (Chroma4->imageData));
    um128i verde4;
    verde4.mm = _mm_set1_epi32((int) verde); 

    for (fila = 0; fila < Chroma4->height; fila++) {
        um128i *pmascara4 = (um128i *) (mascara4->imageData + fila * mascara4->widthStep);
        um128i *pChroma4 = (um128i *) (Chroma4->imageData + fila * Chroma4->widthStep);
        for (columna = 0; columna < Chroma4->width; columna += 4) {
              if ( ((Bv - valor) < pChroma4[0]) &&(pChroma4[0]< (Bv + valor)) &&
              ((Gv - valor) < pChroma4[1]) &&(pChroma4[1]< (Gv + valor)) &&
              ((Rv - valor) < pChroma4[2]) &&( pChroma4[2]< (Rv + valor)) ){
                *pmascara4++ = 255;
                *pmascara4++ = 255;
                *pmascara4++ = 255;
                verde = *((int *) (pChroma4));
                verde4.mm = _mm_set1_epi32((int) verde);    
            } else {
                *pmascara4++ = 0;
                *pmascara4++ = 0;
                *pmascara4++ = 0;
                pChroma4++;
                pChroma4++;
                pChroma4++;
            }
            um128i pixelsX4;

            pixelsX4 = *pChroma4++;
            pixelsX4.mm = _mm_cmpeq_epi32(pixelsX4.mm, verde4.mm);
            *pmascara4++ = pixelsX4;
        }
    }

    cvNamedWindow("mascara4", 1);
    cvShowImage("mascara4", mascara4);
    cvWaitKey(0);

    return EXIT_SUCCESS;
}