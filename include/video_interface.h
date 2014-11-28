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

#include <string>
#include "additionnal_tinyxml.h"

class video_interface
{
      public:
	video_interface ();
	~video_interface ();
	
	/** Create a new project **/
	void new_project(const std::string project_name);
private:
	tinyxml2::XMLDocument doc_;
	
};
#endif