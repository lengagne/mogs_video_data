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

bool video_interface::add_video_to_project(const std::string &file,
					   const std::string &name)
{
	if (video_exists(name))
	{
		std::cerr<<"The video "<<name<<" already exists."<<std::endl;
		exit(0);
	}
	
	// check and get the data about the video.
	video_description tmp;
	tmp.video_name = name;
	tmp.video_file = file;
	
	CvCapture  *capture = cvCaptureFromAVI( file.c_str() );
	// Check if we can read the video Vérifier si l'ouverture du flux est ok
	if (!capture) {
		std::cerr<<"Error when  try to open the video!"<<std::endl;
		return false;
	}	
	tmp.fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	std::cout<<"The video has "<< tmp.fps<<" fps."<<std::endl;
	int nFrames = (int) cvGetCaptureProperty( capture , CV_CAP_PROP_FRAME_COUNT);
	std::cout<<"The video has "<< nFrames<<" frames."<<std::endl;
	tmp.duration = nFrames * 1.0 / tmp.fps;
	
	IplImage *image = cvQueryFrame(capture);
	tmp.width = image->width;
	tmp.height = image->height;
		
	// add the data to the xml
	tinyxml2::XMLElement * video = doc_.NewElement ("video");
	El_videos_->InsertEndChild (video);
	
	tinyxml2::XMLNode * node = doc_.NewElement ("video_name");
	tinyxml2::XMLText * text = doc_.NewText (tmp.video_name.c_str ());
	node->InsertEndChild (text);
	video->InsertEndChild (node);

	node = doc_.NewElement ("video_file");
	text = doc_.NewText (tmp.video_file.c_str ());
	node->InsertEndChild (text);
	video->InsertEndChild (node);
	
	node = doc_.NewElement ("width");
	text = doc_.NewText (std::to_string(tmp.width).c_str());
	node->InsertEndChild (text);
	video->InsertEndChild (node);
	
	node = doc_.NewElement ("height");
	text = doc_.NewText (std::to_string(tmp.height).c_str());
	node->InsertEndChild (text);
	video->InsertEndChild (node);
	
	node = doc_.NewElement ("fps");
	text = doc_.NewText (std::to_string(tmp.fps).c_str());
	node->InsertEndChild (text);
	video->InsertEndChild (node);
	
	node = doc_.NewElement ("duration");
	text = doc_.NewText (std::to_string(tmp.duration).c_str());
	node->InsertEndChild (text);
	video->InsertEndChild (node);
	
// 	video_name,video_file,width,height,fps,duration

	doc_.SaveFile (project_file_.c_str());
	
// 	tinyxml2::XMLNode * ELname = doc_.NewElement ("name");
// 	tinyxml2::XMLText * text = doc_.NewText (name.c_str ());
// 	ELname->InsertEndChild (text);
// 	environment->InsertEndChild (ELname);
// 
// 	tinyxml2::XMLNode * ELxml = doc_.NewElement ("xml_file");
// 	std::string tmp_name = name;
// 	std::string xml_in_project = "Environments/" + tmp_name + "/" + tmp_name + ".xml";
// 	text = doc_.NewText (xml_in_project.c_str ());
// 	ELxml->InsertEndChild (text);
// 	environment->InsertEndChild (ELxml);
// 
// 	std::string cmd = "cd Environments;  mkdir " + name;
// 	int dummy = system (cmd.c_str ());
// 
// 	cmd = "cp " + xml + " Environments/" + name + "/" + name + ".xml -v ";
// 	dummy = system (cmd.c_str ());	
	
	
	return true;
}

void video_interface::new_project(const std::string project_name)
{   
	project_file_ = project_name + ".xml";
	if(fopen(project_file_.c_str(),"r")) {
		std::cerr<<"This project already exists !!"<<std::endl;
		return;
	}	
	// test if the file already exists.
	std::string command = "mkdir videos_"+project_name;
	int dummy = system(command.c_str());
	int lastindex = project_name.find_last_of("."); 
	std::string rawname = project_name.substr(0, lastindex); 
	command = "cp /usr/local/share/MoGS/templates/mogs_project_video_tracking.xml " + rawname +".xml";
	dummy = system (command.c_str());
}

bool video_interface::read(const std::string project_name)
{
	project_file_ = project_name + ".xml";
	std::string cmd = "xmllint --dtdvalid " + (std::string) DTD_REPOSITORY + "/project_video_tracking.dtd --noout "+project_file_;
	int retCode = system (cmd.c_str ());
	if (retCode != 0)
		return false;
	
	int loadOkay = doc_.LoadFile (project_file_.c_str());
	if (loadOkay != tinyxml2::XML_NO_ERROR)
	{
		std::cerr << "Error in " << __FILE__ << " line:" << __LINE__ << " when opening mogs_project.xml" << std::endl;
		std::cerr << "doc.LoadFile returns :" << loadOkay << std::endl;
		return false;
	}
	
	root_ = doc_.RootElement ();
	if (!root_)
	{
		std::cerr << " Error cannot find the root" << std::endl;
		return false;
	}
	
	El_des_ = root_->FirstChildElement ("Definitions");
	if (!El_des_)
	{
		std::cerr << " Error cannot find the Definitions" << std::endl;
		return false;
	}
	
	El_videos_ = El_des_->FirstChildElement ("videos");
	if (!El_videos_)
	{
		std::cerr << " Error cannot find the videos" << std::endl;
		return false;
	}
	
	int cpt = 0;
	tinyxml2::XMLElement * El_video = El_videos_->FirstChildElement ("video");
	for (El_video; El_video; El_video = El_video->NextSiblingElement ("video"))
	{
		read_video_description(El_video);
		cpt++;
	}
	std::cout<<"there is/are "<<cpt<<" camera(s)."<<std::endl;
	
	El_points_ = El_des_->FirstChildElement ("points");
	if (!El_points_)
	{
		std::cerr << " Error cannot find the points" << std::endl;
		return false;
	}
	cpt = 0;
	tinyxml2::XMLElement * El_point = El_des_->FirstChildElement ("point");
	for (El_point; El_point; El_point = El_point->NextSiblingElement ("point"))
	{
		cpt++;
	}
	std::cout<<"there is/are "<<cpt<<" point(s)."<<std::endl;
	
	El_data_ = root_->FirstChildElement ("Datas");
	if (!El_data_)
	{
		std::cerr << " Error cannot find the Datas" << std::endl;
		return false;
	}	
	
	return true;
}

void video_interface::read_video_description(tinyxml2::XMLElement * El)
{
	video_description tmp_video;
	tinyxml2::XMLElement * name = El->FirstChildElement ("video_name");
	if (!name)
	{
		std::cerr << " Error cannot find the video_name" << std::endl;
		exit(0);
	}
	tmp_video.video_name = char_to_string(name->GetText());
	
	tinyxml2::XMLElement * file = El->FirstChildElement ("video_file");
	if (!file)
	{
		std::cerr << " Error cannot find the video_file" << std::endl;
		exit(0);
	}
	tmp_video.video_file = char_to_string(file->GetText());
	
	tinyxml2::XMLElement * width = El->FirstChildElement ("width");
	if (!width)
	{
		std::cerr << " Error cannot find the width" << std::endl;
		exit(0);
	}	
	tmp_video.width = string_to_int(width->GetText());
	
	tinyxml2::XMLElement * height = El->FirstChildElement ("height");
	if (!height)
	{
		std::cerr << " Error cannot find the height" << std::endl;
		exit(0);
	}	
	tmp_video.height = string_to_int(height->GetText());
	
	tinyxml2::XMLElement * fps = El->FirstChildElement ("fps");
	if (!fps)
	{
		std::cerr << " Error cannot find the fps" << std::endl;
		exit(0);
	}
	tmp_video.fps = string_to_int(fps->GetText());
	
	tinyxml2::XMLElement * duration = El->FirstChildElement ("duration");
	if (!duration)
	{
		std::cerr << " Error cannot find the duration" << std::endl;
		exit(0);
	}
	tmp_video.duration = string_to_double(duration->GetText());
	videos_.push_back(tmp_video);
}

void video_interface::show() const
{
	
}

bool video_interface::video_exists(const std::string & name)
{
	int nb = videos_.size();
	for (int i=0;i<nb;i++)
	{
		if ( name == videos_[i].video_name)
			return true;
	}
	return false;
}