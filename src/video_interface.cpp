//      video_interface.cpp
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

#include "video_interface.h"

video_interface::video_interface()
{
	
}

video_interface::~video_interface()
{
	
}

void video_interface::new_project(const std::string project_name)
{
	int dummy = system("mkdir videos");
	int lastindex = project_name.find_last_of("."); 
	std::string rawname = project_name.substr(0, lastindex); 
	std::string command = "cp /usr/local/share/MoGS/templates/mogs_project_video_tracking.xml " + rawname +".xml";
	dummy = system (command.c_str());
}