/*
 * Code written by Vinz (GeckoGeek.fr)
 */
 
#include <stdio.h>
#include "highgui.h"
#include "cv.h"
#include "TLD.h"
#include "ImAcq.h"


int main(int argc, char ** argv) {
	
	tld::TLD * tld = new tld::TLD();
	ImAcq *imAcq = imAcqAlloc();
	
    // Touche clavier
    char key;
    // Image
     IplImage *image;
 
	if (argc != 2)
	{
		std::cout<<"You must specify a video"<<std::endl;
		return -1;
	}
    
   	CvCapture  *capture = cvCaptureFromAVI( argv[1] );
    // V�rifier si l'ouverture du flux est ok
    if (!capture) {
 
       std::cout<<"Ouverture du flux vid�o impossible !"<<std::endl;
       return -1;
 
    }
	cvQueryFrame( capture );
	int nFrames = (int) cvGetCaptureProperty( capture , CV_CAP_PROP_FRAME_COUNT);
	std::cout<<"The video has "<< nFrames<<" frames."<<std::endl;
	int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	std::cout<<"The video has "<< fps<<" fps."<<std::endl;
    
    // D�finition de la fen�tre
    cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE);
 
    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(key != 'q' && key != 'Q') {
 
       // On r�cup�re une image
       image = cvQueryFrame(capture);
 
       // On affiche l'image dans une fen�tre
       cvShowImage( "GeckoGeek Window", image);
 
       // On attend 10ms
       key = cvWaitKey(1000./fps);
 
    }
 
    cvReleaseCapture(&capture);
    cvDestroyWindow("GeckoGeek Window");
 
    return 0;
 
}