/*
 * Code written by Vinz (GeckoGeek.fr)
 */
 
#include <stdio.h>
#include "highgui.h"
#include "cv.h"
 
int main() {
 
    // Touche clavier
    char key;
    // Image
     IplImage *image;
    // Capture vid�o
    CvCapture *capture;
 
    // Ouvrir le flux vid�o
    //capture = cvCreateFileCapture("/path/to/your/video/test.avi"); // chemin pour un fichier
    capture = cvCreateCameraCapture(CV_CAP_ANY);
 
    // V�rifier si l'ouverture du flux est ok
    if (!capture) {
 
       printf("Ouverture du flux vid�o impossible !\n");
       return 1;
 
    }
 
    // D�finition de la fen�tre
    cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE);
 
    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {
 
       // On r�cup�re une image
       image = cvQueryFrame(capture);
 
       // On affiche l'image dans une fen�tre
       cvShowImage( "GeckoGeek Window", image);
 
       // On attend 10ms
       key = cvWaitKey(10);
 
    }
 
    cvReleaseCapture(&capture);
    cvDestroyWindow("GeckoGeek Window");
 
    return 0;
 
}