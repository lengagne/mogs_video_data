//      video_interface.h
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

#ifndef __VIDEO_INTERFACE__
#define __VIDEO_INTERFACE__

#include <vector>
#include <string>
#include "additionnal_tinyxml.h"
#include "config_Video_data.h"
#include "highgui.h"
#include "cv.h"

typedef struct
{
	std::string video_name;
	std::string video_file;
	int width;
	int height;
	int fps;
	double duration;
}video_description;

class video_interface
{
      public:
	video_interface ();
	~video_interface ();

	/** Add a point to the xml of the project*/
	bool add_point_to_project(const std::string &name);

	
	/** Add a video to the xml of the project*/
	bool add_video_to_project(const std::string &file,
				  const std::string &name);
	
	/** Create a new project **/
	void new_project(const std::string project_name);
	
	/** read a project and return false in case of trouble */
	bool read(const std::string project_name);
	
	/** Show the details of a project **/
	void show() const;
	
	/** Show the list of the points **/
	void show_point_list() const;
	
	/** Show the list of the videos **/
	void show_video_list() const;
private:
	std::string project_file_;

	/** Check if a point name is already defined 
	 *	return false if the video does not exists
	 */
	bool point_exists(const std::string & name);	
	
	/** Read the video description in the file */
	void read_video_description(tinyxml2::XMLElement * El);
	
	/** Check if a video name is already defined 
	 *	return false if the video does not exists
	 */
	bool video_exists(const std::string & name);
		
	/** read the xml project variables */	
	tinyxml2::XMLDocument doc_;
	/** main node of the xml file */
	tinyxml2::XMLElement * root_;
	
	/** node for descriptions and data*/
	tinyxml2::XMLElement * El_des_, *El_data_;
	
	/** node for description of video and of point */
	tinyxml2::XMLElement * El_videos_, *El_points_;
	
	/** processing variables	*/
	std::vector<video_description> videos_;
	
	std::vector<std::string> points_;
	
};
#endif