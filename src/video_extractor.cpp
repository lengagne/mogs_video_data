//      video_extractor.cpp
//      Copyright (C) 2014 lengagne (sebastien.lengagne@univ-bpclermont.fr)
// 
//      This program is free software: you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation, either version 3 of the License, or
//      (at your option) any later version.
// 
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
// 
//      You should have received a copy of the GNU General Public License
//      along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//      This program was developped in the following labs:
//	from 2013 : Université Blaise Pascal / axis : ISPR / theme MACCS

#include "video_extractor.h"

using namespace cv;

video_extractor::video_extractor (const std::string& video_file)
:video_file_(video_file)
{
	
}

video_extractor::~video_extractor()
{
	
}

void video_extractor::edit_data(const std::string& point,
				int version)
{
	IplImage *image;
   	CvCapture  *capture = cvCaptureFromAVI( video_file_.c_str() );
	if (!capture) {
		std::cout<<"Cannot open "<< video_file_ << " !"<<std::endl;
		return ;
	}
	
	image = cvQueryFrame(capture);
	int nFrames = (int) cvGetCaptureProperty( capture , CV_CAP_PROP_FRAME_COUNT);
	int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	CvPoint Pt1,Pt2;	
	bool rect_set;
	rect_set = false;
	data_mouse d;
	d.image = image;
	d.Pt1 = &Pt1;
	d.Pt2 = &Pt2;
	d.rect_set = &rect_set;
	
	CvScalar black = CV_RGB(0, 0, 0);
	CvScalar white = CV_RGB(255, 255, 255);
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, 8);
	
	cvNamedWindow("MoGS_video_tracking", CV_WINDOW_AUTOSIZE);
	setMouseCallback("MoGS_video_tracking", PositionCurseur, (void*) &d);
	char key;
	int cpt = 1;
	do {
		image = cvQueryFrame(capture);
		key = 0;
		while (key != 'n' && key != 'q' && key != 'Q')
		{
			std::string tmp = "frame " +double_to_string(cpt) + "/" + double_to_string(nFrames) ;
			IplImage * image2 = cvCloneImage(image);
			if (rect_set)
			{
				tmp = tmp +" selected point: " + double_to_string(Pt1.x) + " : " + double_to_string(Pt1.y);
			}	
			cvPutText(image2, tmp.c_str(), cvPoint(25, 25), &font, white);
			cvShowImage( "MoGS_video_tracking", image2);	
			key = cvWaitKey(1000./fps);	
		}
		if (rect_set)
		{
			// add the point !!
		}		
		rect_set = false;
		cpt++;
	} while(key != 'q' && key != 'Q' && cpt < nFrames);
	cvReleaseCapture(&capture);
	cvDestroyWindow("MoGS_video_tracking");	
}

void video_extractor::play() const
{
	IplImage *image;
   	CvCapture  *capture = cvCaptureFromAVI( video_file_.c_str() );
	// Vérifier si l'ouverture du flux est ok
	if (!capture) {
		std::cout<<"Cannot open "<< video_file_ << " !"<<std::endl;
		return ;
	}
	
	image = cvQueryFrame(capture);
	int nFrames = (int) cvGetCaptureProperty( capture , CV_CAP_PROP_FRAME_COUNT);
	int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	
	cvNamedWindow("MoGS_video_tracking", CV_WINDOW_AUTOSIZE);
	char key;
	int cpt = 1;
	while(key != 'q' && key != 'Q' && cpt < nFrames) {
		cpt++;
		image = cvQueryFrame(capture);
		
// 		CvScalar black = CV_RGB(0, 0, 0);
// 		CvScalar white = CV_RGB(255, 255, 255);
// 		CvFont font;
// 		cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, .5, .5, 0, 1, 8);
// // 		cvRectangle(image, cvPoint(0, 0), cvPoint(image->width, 5), black, CV_FILLED, 8, 0);
// 		cvPutText(image, "toto", cvPoint(25, 25), &font, white);
		cvShowImage( "MoGS_video_tracking", image);	
		key = cvWaitKey(1000./fps);
	} 
	cvReleaseCapture(&capture);
	cvDestroyWindow("MoGS_video_tracking");
}

/** Interface*/

//repere la position d'un clic droit ou gauche
void PositionCurseur(int event, int x, int y, int flags, void* userdata){
	data_mouse *d= (data_mouse*) userdata;
/*****************************création rectangle**************************************************/
	if(event == EVENT_LBUTTONDOWN){
		d->Pt1->x=x;
		d->Pt1->y=y;
	}
	if(event == EVENT_LBUTTONUP){
		std::cout << ""<<x<<""<<y<<"" << std::endl;
		d->Pt2->x=x;
		d->Pt2->y=y;
		*(d->rect_set) = true;
	}
}