//      video_extracted_data.h
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

#ifndef __VIDEO_EXTRACTED_DATA__
#define __VIDEO_EXTRACTED_DATA__

#include <vector>
#include <string>
#include <iostream>
#include "highgui.h"
#include "cv.h"

typedef struct
{
	int frame;
	std::string video;
	int video_id;
	std::string point;
	int point_id;
	int version;
	std::string source;
	CvPoint value;
}video_data;

inline std::ostream & operator<< (std::ostream & output,
					const video_data & in)
{
	output << "\tframe = " << in.frame << std::endl;
	output << "\tvideo = " << in.video << std::endl;
	output << "\tvideo_id = " << in.video_id << std::endl;
	output << "\tpoint = " << in.point << std::endl;
	output << "\tpoint_id = " << in.point_id << std::endl;
	output << "\tversion = " << in.version << std::endl;
	output << "\tsource = " << in.source << std::endl;
	output << "\tvalue = " << in.value.x<<" : "<< in.value.y << std::endl;	
	return output;
}

class video_extracted_data
{
      public:
	video_extracted_data (int nb);
	
	~video_extracted_data ();
	
	void add_data(const video_data& in);
	
	int get_next_version()const;
	
	void get_new_version_data(int version,
				  std::vector<video_data> & datas) const;
	
	int get_number_data() const
	{
		return datas_.size();
	}
	
	int get_number_of_point() const;
	
	bool get_value ( int frame,
			 int video_id,
			 int point_id,
			 CvPoint & val) const;
	
private:
	
	/** Test if the a is comparable and
	 * updatable with b	*/
	bool updatable( const video_data& a,
			const video_data& b) const;
	
	std::vector<video_data> datas_;
	
	int nb_frames_;
};
#endif