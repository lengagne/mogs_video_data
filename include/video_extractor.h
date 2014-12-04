//      video_extractor.h
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

#ifndef __VIDEO_EXTRACTOR__
#define __VIDEO_EXTRACTOR__

#include "video_extracted_data.h"
#include "additionnal_tinyxml.h"
// #include "TLD.h"
// #include "TLDUtil.h"



typedef struct{
	IplImage *image;
	CvPoint* Pt1;
	CvPoint* Pt2;
	bool * rect_set;
}data_mouse;

void PositionCurseur(int event, int x, int y, int flags, void* userdata);

class video_extractor
{
      public:
	video_extractor (const std::string & video,
			 int video_id,
			 const std::string & point,
			 int point_id,
			 const std::string& video_file);
	~video_extractor ();
	
	/** Edit a new set of data and return the new version of this point */
	int edit_data(const std::string& point);
	
	void play( ) const;
	
	void set_data(video_extracted_data * in)
	{
		video_data_ = in;
	}
	
	
private:
	std::string point_;
	int point_id_;
	
	std::string video_;
	int video_id_;
	
	std::string video_file_;
	
	video_extracted_data *video_data_;
	
};
#endif