/*
 * Code written by Vinz (GeckoGeek.fr)
 */
 
#include <stdio.h>
#include "highgui.h"
#include "cv.h"
#include "TLD.h"
#include "TLDUtil.h"

using namespace tld;
using namespace cv;

typedef struct{
	IplImage *image;
	CvPoint* Pt1;
	CvPoint* Pt2;
	bool * rect_set;
}data;

//repere la position d'un clic droit ou gauche
void PositionCurseur(int event, int x, int y, int flags, void* userdata){
	data *d= (data*) userdata;
/*****************************création rectangle**************************************************/
	if(event == EVENT_LBUTTONDOWN){
		//cout << "1er point" << endl;
		//cout << ""<<x<<""<<y<<"" << endl;
		//pointeur Pt1 qui pointe vers un point en x et y
		d->Pt1->x=x;
		d->Pt1->y=y;
	}
	if(event == EVENT_LBUTTONUP){
		//cout << ""<<x<<""<<y<<"" << endl;
		d->Pt2->x=x;
		d->Pt2->y=y;
// 			cvRectangle(Img,*Pt1,*Pt2,cvScalar(255,0,0),2,8,0);
		std::cout<<" Pt1 = "<< d->Pt1->x<<":"<< d->Pt1->y<<"\t Pt2 = "<< d->Pt2->x<<":"<< d->Pt2->y<<std::endl;
		*(d->rect_set) = true;
	}
}


int main(int argc, char ** argv) {
	
	tld::TLD * tld = new tld::TLD();
// 	ImAcq *imAcq = imAcqAlloc();
	int *initialBB;
        if(initialBB == NULL)
        {
            initialBB = new int[4];
        }

	const char *modelPath = NULL;
	
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
	// Vérifier si l'ouverture du flux est ok
	if (!capture) {
		std::cout<<"Ouverture du flux vidéo impossible !"<<std::endl;
		return -1;
	}
	
	image = cvQueryFrame(capture);
	int nFrames = (int) cvGetCaptureProperty( capture , CV_CAP_PROP_FRAME_COUNT);
	std::cout<<"The video has "<< nFrames<<" frames."<<std::endl;
	int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	std::cout<<"The video has "<< fps<<" fps."<<std::endl;

	
	IplImage *image2;
	image2=cvCloneImage(image);
	
	CvPoint Pt1,Pt2;	
	bool rect_set;
	rect_set = false;
	data d;
	d.image = image2;
	d.Pt1 = &Pt1;
	d.Pt2 = &Pt2;
	d.rect_set = &rect_set;
	// Définition de la fenêtre
	cvNamedWindow("Selection Window", CV_WINDOW_AUTOSIZE);
	setMouseCallback("Selection Window", PositionCurseur, (void*) &d);//important de mettre le & sinon sa ne compile pas!
	std::cout<<" Rectangle setting"<<std::endl;
	// choose the object to track
	while(key != 'n' && key != 'N') {
		if (rect_set)
		{
			cvRectangle(image2,Pt1,Pt2,cvScalar(255,0,0),2,8,0);
		}
		
		// On affiche l'image dans une fenêtre
		cvShowImage( "Selection Window", image2);	
			
		// On attend 10ms
		key = cvWaitKey(1000./fps);
	}
	
	std::cout<<" Rectangle set"<<std::endl;
        CvRect box;
	box.x = Pt1.x;
	box.y = Pt1.y;
	box.width = Pt2.x - Pt1.x;
	box.height= Pt2.y - Pt1.y;
        initialBB[0] = box.x;
        initialBB[1] = box.y;
        initialBB[2] = box.width;
        initialBB[3] = box.height;	
	
	cv::Mat grey(image->height, image->width, CV_8UC1);
	cvtColor(cv::cvarrToMat(image), grey, CV_BGR2GRAY);

	tld->detectorCascade->imgWidth = grey.cols;
	tld->detectorCascade->imgHeight = grey.rows;
	tld->detectorCascade->imgWidthStep = grey.step;
	
	Rect bb = tldArrayToRect(initialBB);
	printf("Starting at %d %d %d %d\n", bb.x, bb.y, bb.width, bb.height);
	tld->selectObject(grey, &bb);	
	
	cvDestroyWindow("Selection Window");	
	cvNamedWindow("GeckoGeek Window", CV_WINDOW_AUTOSIZE);

	int cpt = 1;
	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q' && cpt < nFrames) {
		std::cout<<"cpt = "<< cpt<<std::endl;
		std::cout<<"nFrames = "<< nFrames<<std::endl;
		// On récupère une image
		image = cvQueryFrame(capture);
		cpt ++;
		
		
		tld->processImage(cvarrToMat(image));
		
		if(tld->currBB != NULL)
		{
			printf("%.2d %.2d %.2d %.2d %f\n", tld->currBB->x, tld->currBB->y, tld->currBB->width, tld->currBB->height, tld->currConf);
		}
		else
		{
			printf("NaN NaN NaN NaN NaN\n");
		}	
		double threshold = 0.5;
		int confident = (tld->currConf >= threshold) ? 1 : 0;
		
		CvScalar yellow = CV_RGB(255, 255, 0);
		CvScalar blue = CV_RGB(0, 0, 255);
		CvScalar black = CV_RGB(0, 0, 0);
		CvScalar white = CV_RGB(255, 255, 255);

		if(tld->currBB != NULL)
		{
			CvScalar rectangleColor = (confident) ? blue : yellow;
			cvRectangle(image, tld->currBB->tl(), tld->currBB->br(), rectangleColor, 8, 8, 0);

			CvPoint center = cvPoint(tld->currBB->x+tld->currBB->width/2, tld->currBB->y+tld->currBB->height/2);
			cvLine(image, cvPoint(center.x-2, center.y-2), cvPoint(center.x+2, center.y+2), rectangleColor, 2);
			cvLine(image, cvPoint(center.x-2, center.y+2), cvPoint(center.x+2, center.y-2), rectangleColor, 2);
		}	
		
		CvFont font;
		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, 8);
		cvRectangle(image, cvPoint(0, 0), cvPoint(image->width, 50), black, CV_FILLED, 8, 0);
// 		cvPutText(image, string, cvPoint(25, 25), &font, white);
		
		for(size_t i = 0; i < tld->detectorCascade->detectionResult->fgList->size(); i++)
		{
			Rect r = tld->detectorCascade->detectionResult->fgList->at(i);
			cvRectangle(image, r.tl(), r.br(), white, 1);
		}
		
		if(key == 'c')
		{
			//clear everything
			tld->release();
		}

		if(key == 'l')
		{
			tld->learningEnabled = !tld->learningEnabled;
			printf("LearningEnabled: %d\n", tld->learningEnabled);
		}

		if(key == 'a')
		{
			tld->alternating = !tld->alternating;
			printf("alternating: %d\n", tld->alternating);
		}

		if(key == 'e')
		{
			tld->writeToFile("model");
		}

		if(key == 'i')
		{
			tld->readFromFile(modelPath);
		}

		if(key == 'r')
		{
			CvRect box;
	// 		if(getBBFromUser(image, box, gui) == PROGRAM_EXIT)
	// 		{
	// 		break;
	// 		}
			Rect r = Rect(box);
			tld->selectObject(grey, &r);
		}
		
		// On affiche l'image dans une fenêtre
		cvShowImage( "GeckoGeek Window", image);	
			
		// On attend 10ms
		key = cvWaitKey(1000./fps);
	}
 
	cvReleaseCapture(&capture);
	cvDestroyWindow("GeckoGeek Window");
	
	return 0;
 
}