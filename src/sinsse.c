
#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Error: Usage %s image_file_name\n", argv[0]);
        return EXIT_FAILURE;
    }
    /*
        IplImage* Chroma = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
        if (!Chroma) {
            printf("Error: file %s not found\n", argv[1]);
            return EXIT_FAILURE;
        }*/
    IplImage* Paisaje = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);
    if (!Paisaje) {
        printf("Error: file %s not found\n", argv[1]);
    }

    CvCapture* capture = 0;
    IplImage *Chroma = 0;

    if (!(capture = cvCaptureFromCAM(0))) {
        printf("Cannot initialize camera\n");
        return EXIT_FAILURE;
    }

    while (Chroma > 0) {
        Chroma = cvQueryFrame(capture);

        IplImage* Mask = cvCreateImage(cvGetSize(Paisaje), IPL_DEPTH_8U, 3);
        int fila, columna, col;
        unsigned char valor = 15;

        unsigned char Bderecho;
        unsigned char Gderecho;
        unsigned char Rderecho;
        unsigned char Breves;
        unsigned char Greves;
        unsigned char Rreves;

        for (fila = 0; fila < Chroma->height; fila++) {

            unsigned char *pChroma = (unsigned char *) (Chroma->imageData + fila * Chroma->
                    widthStep);
            unsigned char *pMask = (unsigned char *) (Mask->imageData + fila *
                    Mask->widthStep);
            Bderecho = pChroma[0];
            Gderecho = pChroma[1];
            Rderecho = pChroma[2];

            for (columna = 0; columna < Chroma->width; columna++) {

                if (((Bderecho - valor) <= (pChroma[0]))
                        && ((pChroma[0]) <= (Bderecho + valor))
                        && ((Gderecho - valor) <= (pChroma[1]))
                        && ((pChroma[1]) <= (Gderecho + valor))
                        && ((Rderecho - valor) <= (pChroma[2]))
                        && ((pChroma[2]) <= (Rderecho + valor))) {
                    *pMask++ = 255;
                    *pMask++ = 255;
                    *pMask++ = 255;
                    Bderecho = *pChroma++;
                    Gderecho = *pChroma++;
                    Rderecho = *pChroma++;
                } else {
                    *pMask++ = 0;
                    *pMask++ = 0;
                    *pMask++ = 0;
                    pChroma++;
                    pChroma++;
                    pChroma++;
                }
            }
        }

        for (fila = 0; fila < Chroma->height; fila++) {

            unsigned char *pChroma = (unsigned char *) (Chroma->imageData + fila * Chroma->widthStep + Chroma->widthStep);
            unsigned char *pMask = (unsigned char *) (Mask->imageData + fila * Mask->widthStep + Mask->widthStep);
            Breves = pChroma[0];
            Greves = pChroma[1];
            Rreves = pChroma[2];

            for (col = Chroma->width; col > 0; col--) {
                if (((Breves - valor) <= (pChroma[0]))
                        && ((pChroma[0]) <= (Breves + valor))
                        && ((Greves - valor) <= (pChroma[1]))
                        && ((pChroma[1]) <= (Greves + valor))
                        && ((Rreves - valor) <= (pChroma[2]))
                        && ((pChroma[2]) <= (Rreves + valor))) {
                    pMask[0] = 255;
                    pMask[1] = 255;
                    pMask[2] = 255;
                    Breves = pChroma[0];
                    Greves = pChroma[1];
                    Rreves = pChroma[2];
                }
                pChroma--;
                pChroma--;
                pChroma--;
                pMask--;
                pMask--;
                pMask--;
            }
        }

        cvNamedWindow("Mascara", 1);
        cvShowImage("Mascara", Mask);
        cvWaitKey(0);



        cvNamedWindow("Animacion", 1);
        IplImage* Animacion = cvLoadImage(argv[2], CV_LOAD_IMAGE_UNCHANGED);

        for (fila = 0; fila < Paisaje->height; fila++) {

            unsigned char *pPaisaje = (unsigned char *) Paisaje->imageData + fila * Paisaje->widthStep;
            unsigned char *pAnimacion = (unsigned char *) Animacion->imageData + fila * Animacion-> widthStep;
            unsigned char *pMask = (unsigned char *) Mask->imageData + fila * Mask->widthStep;
            unsigned char *pChroma = (unsigned char *) Chroma->imageData + fila * Chroma->widthStep;

            for (columna = 0; columna < Paisaje->width; columna++) {

                *pAnimacion++ = (*pPaisaje++ & *pMask) | (*pChroma++ & ~(*pMask++));
                *pAnimacion++ = (*pPaisaje++ & *pMask) | (*pChroma++ & ~(*pMask++));
                *pAnimacion++ = (*pPaisaje++ & *pMask) | (*pChroma++ & ~(*pMask++));
            }
        }
        cvShowImage("Animacion", Animacion);
        cvWaitKey(0);
    }
    return EXIT_SUCCESS;
}