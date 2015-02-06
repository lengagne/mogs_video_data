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

using namespace tld;
using namespace cv;

video_extractor::video_extractor (	const std::string & video,
					int video_id,
					const std::string& video_file)
:video_(video),video_id_(video_id),point_("not_defined"),point_id_(-1), video_file_(video_file)
{
	
}

video_extractor::video_extractor (	const std::string & video,
					int video_id,
					const std::string & point,
					int point_id,
					const std::string& video_file)
:video_(video),video_id_(video_id),point_(point),point_id_(point_id), video_file_(video_file)
{
	
}

video_extractor::~video_extractor()
{
	
}

int video_extractor::edit_data(const std::string& point)
{
	std::cout<<"video_extractor::edit_data"<<std::endl;
	IplImage *image;
   	CvCapture  *capture = cvCaptureFromAVI( video_file_.c_str() );
	if (!capture) {
		std::cout<<"Cannot open "<< video_file_ << " !"<<std::endl;
		return -1;
	}
	
	image = cvQueryFrame(capture);
	int nFrames = (int) cvGetCaptureProperty( capture , CV_CAP_PROP_FRAME_COUNT);
	int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	
	CvPoint Pt1,Pt2,center;	
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
	int cpt = 0;
	CvPoint visu_point;
	bool tld_mode = false;
	bool update_image = false;
	do {
		if (update_image)
			image = cvQueryFrame(capture);
		update_image = true;
		IplImage * image2 = cvCloneImage(image);
		key = 0;
		if (!tld_mode)	while (key != 'n' && key != 'q' && key != 'Q' && key != 't')
		{
			IplImage * image2 = cvCloneImage(image);
			std::string tmp = "frame " +double_to_string(cpt) + "/" + double_to_string(nFrames) ;			
			if (rect_set)
			{
				if ( (Pt1.x - Pt2.x) > 5 || (Pt1.x - Pt2.x) < -5 || (Pt1.y - Pt2.y) > 5 || (Pt1.y - Pt2.y) < -5)
				{
					tmp = tmp +" selected rectangle center : " + double_to_string(Pt1.x) + " : " + double_to_string(Pt1.y);
					center.x = (Pt1.x + Pt2.x )/2;
					center.y = (Pt1.y + Pt2.y )/2;
					cvRectangle(image2, Pt1, Pt2, CV_RGB(255, 255, 255), 1);
				}else
				{
					center = Pt1;
					tmp = tmp +" selected point: " + double_to_string(Pt1.x) + " : " + double_to_string(Pt1.y);
				}
				cvLine(image2, cvPoint(center.x-2, center.y), cvPoint(center.x+2, center.y), cvScalar(255,255,255), 2);
				cvLine(image2, cvPoint(center.x, center.y+2), cvPoint(center.x, center.y-2), cvScalar(255,255,255), 2);
				
			}else
			{
				if (video_data_->get_value(cpt,video_id_, point_id_,visu_point))
				{
					cvLine(image2, cvPoint(visu_point.x-2, visu_point.y), cvPoint(visu_point.x+2, visu_point.y), cvScalar(255,255,255), 2);
					cvLine(image2, cvPoint(visu_point.x, visu_point.y+2), cvPoint(visu_point.x, visu_point.y-2), cvScalar(255,255,255), 2);
				}					
			}
			cvPutText(image2, tmp.c_str(), cvPoint(25, 25), &font, white);
			cvShowImage( "MoGS_video_tracking", image2);
			key = cvWaitKey(1000./fps);
		}
		if (key == 't' && !tld_mode)
		{
			if ( (Pt1.x - Pt2.x) < 5 && (Pt1.x - Pt2.x) > -5 && (Pt1.y - Pt2.y) < 5 && (Pt1.y - Pt2.y) > -5)
			{
				tld_mode = false;
			}else
			{
				tld_mode = true;
				// init the tld mode
				tld = new tld::TLD();
				int initialBB[4];
				std::cout<<"Pt1 = "<< Pt1.x<<" " << Pt1.y <<std::endl;
				std::cout<<"Pt2 = "<< Pt2.x<<" " << Pt2.y <<std::endl;
				if (Pt1.x < Pt2.x){
					initialBB[0] = Pt1.x;
					initialBB[2] = Pt2.x - Pt1.x;
				}
				else {
					initialBB[0] = Pt2.x;
					initialBB[2] = Pt1.x - Pt2.x;
				}
				if (Pt1.y < Pt2.y){
					initialBB[1] = Pt1.y;
					initialBB[3] = Pt2.y - Pt1.y;
				}
				else{
					initialBB[1] = Pt2.y;
					initialBB[3] = Pt1.y - Pt2.y;
				}
				
				cv::Mat grey(image->height, image->width, CV_8UC1);
				cvtColor(cv::cvarrToMat(image), grey, CV_BGR2GRAY);

				tld->detectorCascade->imgWidth = grey.cols;
				tld->detectorCascade->imgHeight = grey.rows;
				tld->detectorCascade->imgWidthStep = grey.step;
				
				Rect bb = tldArrayToRect(initialBB);
				printf("Starting at %d %d %d %d\n", bb.x, bb.y, bb.width, bb.height);
				tld->selectObject(grey, &bb);
			}
		}
		if (tld_mode)
		{
			// continue the tld algo
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
			if(tld->currConf >= threshold)
			{
				CvScalar blue = CV_RGB(0, 0, 255);
				CvScalar black = CV_RGB(0, 0, 0);
				CvScalar white = CV_RGB(255, 255, 255);

				if(tld->currBB != NULL)
				{
					cvRectangle(image2, tld->currBB->tl(), tld->currBB->br(), blue, 8, 8, 0);
					center = cvPoint(tld->currBB->x+tld->currBB->width/2, tld->currBB->y+tld->currBB->height/2);
					cvLine(image2, cvPoint(center.x-2, center.y-2), cvPoint(center.x+2, center.y+2), blue, 2);
					cvLine(image2, cvPoint(center.x-2, center.y+2), cvPoint(center.x+2, center.y-2), blue, 2);
					rect_set = true;
				}	
				cvShowImage( "MoGS_video_tracking", image2);
				key = cvWaitKey(1000./fps);
			}else
			{
				tld_mode = false;
				update_image = false;
			}
		}	

		if (rect_set && update_image)
		{
			// add the point !!
			video_data tmp;
			tmp.frame = cpt;
			tmp.video = video_;
			tmp.video_id = video_id_;
			tmp.point = point_;
			tmp.point_id = point_id_;
			if (tld_mode)
				tmp.source = "tld";
			else
				tmp.source = "manual";
			tmp.value = center;
			video_data_->add_data(tmp);
		}		
		rect_set = false;
		if (update_image)
			cpt++;
	} while(key != 'q' && key != 'Q' && cpt < nFrames);
	cvReleaseCapture(&capture);
	cvDestroyWindow("MoGS_video_tracking");
	return 0;
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
	int nb_points = video_data_->get_number_of_point();
	CvPoint visu_point;
	while(key != 'q' && key != 'Q' && cpt < nFrames) {
		cpt++;
		image = cvQueryFrame(capture);
		
		for (int i=0;i<nb_points;i++)
			if (video_data_->get_value(cpt,video_id_, i,visu_point))
			{
				cvLine(image, cvPoint(visu_point.x-2, visu_point.y), cvPoint(visu_point.x+2, visu_point.y), cvScalar(255,255,255), 2);
				cvLine(image, cvPoint(visu_point.x, visu_point.y+2), cvPoint(visu_point.x, visu_point.y-2), cvScalar(255,255,255), 2);
			}

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
	switch(event)
	{
		case(EVENT_LBUTTONDOWN):
			d->Pt1->x=x;
			d->Pt1->y=y;
			break;
		case(EVENT_LBUTTONUP):
			std::cout << ""<<x<<""<<y<<"" << std::endl;
			d->Pt2->x=x;
			d->Pt2->y=y;
			*(d->rect_set) = true;
			break;
	}

}