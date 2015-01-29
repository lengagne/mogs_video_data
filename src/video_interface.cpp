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
	nb_frames_ = 1e9;
}

video_interface::~video_interface()
{
	
}

bool video_interface::add_point_to_project(const std::string &name)
{
	if (point_exists(name))
	{
		std::cerr<<"The point "<<name<<" already exists."<<std::endl;
		exit(0);
	}

	// add the data to the xml
	tinyxml2::XMLElement * point = doc_.NewElement ("point");
	El_points_->InsertEndChild (point);
	tinyxml2::XMLText * text = doc_.NewText (name.c_str ());
	point->InsertEndChild (text);

	doc_.SaveFile (project_file_.c_str());
	
	return true;
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
	tmp.nb_frames = nFrames;
	std::cout<<"The video has "<< nFrames<<" frames."<<std::endl;
	tmp.duration = nFrames * 1.0 / tmp.fps;
	
	IplImage *image = cvQueryFrame(capture);
	tmp.width = image->width;
	tmp.height = image->height;
	cvReleaseCapture(&capture);
	
		
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
	
	node = doc_.NewElement ("nb_frames");
	text = doc_.NewText (std::to_string(tmp.nb_frames).c_str());
	node->InsertEndChild (text);
	video->InsertEndChild (node);
	
	node = doc_.NewElement ("duration");
	text = doc_.NewText (std::to_string(tmp.duration).c_str());
	node->InsertEndChild (text);
	video->InsertEndChild (node);
	
	doc_.SaveFile (project_file_.c_str());

	return true;
}

void video_interface::edit_data( const std::string &video,
				const std::string &point)
{
	std::cout<<"Push 'n' to see next frame "<<std::endl;
	std::cout<<"Click on the image to select the point for "<< point<<"."<<std::endl;
	std::cout<<"Draw a rectangle and push 't' in order to do automatic tracking."<<std::endl;
	std::cout<<"edit_data "<< video<< " "<< point<<std::endl;
	read_data();
	int video_id = get_video_id(video);
	int point_id = get_point_id(point);
	extractor_ = new video_extractor(video,video_id,point,point_id, videos_[video_id].video_file);
	extractor_->set_data(video_data_);
	int new_version = extractor_->edit_data(point);
	// save the added_point
	save_data(new_version);
}

int video_interface::get_point_id( const std::string & name) const
{
	int nb = points_.size();
	for (int i=0;i<nb;i++)	if (name == points_[i])
		return i;
	return -1;
}

int video_interface::get_video_id( const std::string & name) const
{
	int nb = videos_.size();
	for (int i=0;i<nb;i++)	if (name == videos_[i].video_name)
		return i;
	return -1;
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

void video_interface::play_video(const std::string &video )
{
	read_data();
	int video_id = get_video_id(video);
	extractor_ = new video_extractor(video,video_id, videos_[video_id].video_file);
	extractor_->set_data(video_data_);
	extractor_->play();	
}

bool video_interface::point_exists(const std::string & name)
{
	int nb = points_.size();
	for (int i=0;i<nb;i++)
	{
		if ( name == points_[i])
			return true;
	}
	return false;
}

bool video_interface::read(const std::string project_name)
{
	project_file_ = project_name;
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
// 	std::cout<<"nb_frames = "<<nb_frames_<<std::endl;
	
	El_points_ = El_des_->FirstChildElement ("points");
	if (!El_points_)
	{
		std::cerr << " Error cannot find the points" << std::endl;
		return false;
	}
	cpt = 0;
	tinyxml2::XMLElement * El_point = El_points_->FirstChildElement ("point");
	for (El_point; El_point; El_point = El_point->NextSiblingElement ("point"))
	{
		std::string tmp = char_to_string(El_point->GetText());
		points_.push_back(tmp);
		cpt++;
	}
// 	std::cout<<"there is/are "<<cpt<<" point(s)."<<std::endl;
	
	El_datas_ = root_->FirstChildElement ("Datas");
	if (!El_datas_)
	{
		std::cerr << " Error cannot find the Datas" << std::endl;
		return false;
	}	
	
	return true;
}

bool video_interface::read_data( )
{
	video_data_ = new video_extracted_data(nb_frames_);
	std::cout<<"video_interface::read_data()"<<std::endl;
	int cpt = 0;
	tinyxml2::XMLElement * El_data = El_datas_->FirstChildElement ("data");
	for (El_data; El_data; El_data = El_data->NextSiblingElement ("data"))
	{
		video_data tmp;
		tmp.frame = string_to_int(El_data->Attribute ("frame"));
		tmp.video = El_data->Attribute ("video");
		tmp.video_id = get_video_id(tmp.video);
		tmp.point = El_data->Attribute ("point");
		tmp.point_id = get_point_id(tmp.point);
		tmp.version = string_to_int(El_data->Attribute ("version"));
		tmp.source = El_data->Attribute ("source");
		std::string stmp = char_to_string (El_data->GetText ());
		std::istringstream smallData (stmp, std::ios_base::in);
		smallData >> tmp.value.x;
		smallData >> tmp.value.y;
		video_data_->add_data(tmp);
		cpt++;
	}
	std::cout<<"number of data = "<< cpt <<std::endl;
	std::cout<<"number of data = "<< video_data_->get_number_data() <<std::endl;
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
	
	tinyxml2::XMLElement * nb_frames = El->FirstChildElement ("nb_frames");
	if (!nb_frames)
	{
		std::cerr << " Error cannot find the fps" << std::endl;
		exit(0);
	}
	tmp_video.nb_frames = string_to_int(nb_frames->GetText());
	if (nb_frames_ > tmp_video.nb_frames)
		nb_frames_ = tmp_video.nb_frames;
	
	tinyxml2::XMLElement * duration = El->FirstChildElement ("duration");
	if (!duration)
	{
		std::cerr << " Error cannot find the duration" << std::endl;
		exit(0);
	}
	tmp_video.duration = string_to_double(duration->GetText());
	videos_.push_back(tmp_video);
}

void video_interface::save_data(int version)
{
	std::vector<video_data> new_data;
	video_data_->get_new_version_data(version,new_data);
	int nb= new_data.size();
	for (int i=0;i<nb;i++)
	{
		// add the data to the xml
		tinyxml2::XMLElement * data = doc_.NewElement ("data");
		El_datas_->InsertEndChild (data);
		data->SetAttribute("frame",new_data[i].frame);
		data->SetAttribute("video",new_data[i].video.c_str());
		data->SetAttribute("point",new_data[i].point.c_str());
		data->SetAttribute("version",new_data[i].version);
		data->SetAttribute("source",new_data[i].source.c_str());
		std::string tmp = double_to_string(new_data[i].value.x)+ " " + double_to_string(new_data[i].value.y);
		tinyxml2::XMLText * text = doc_.NewText (tmp.c_str ());
		data->InsertEndChild (text);
	}
	doc_.SaveFile (project_file_.c_str());
}

void video_interface::show() const
{
	std::cout<<"Registered videos"<<std::endl;
	show_video_list();
}

void video_interface::show_point_list() const
{
	int nb = points_.size();
	for (int i=0;i<nb;i++)
		std::cout<<"\t"<< points_[i] <<std::endl;	
}

void video_interface::show_video_list() const
{
	int nb = videos_.size();
	for (int i=0;i<nb;i++)
		std::cout<<"\t"<< videos_[i].video_name <<std::endl;
}

bool video_interface::video_exists(const std::string & name)
{
	int nb = videos_.size();
	for (int i=0;i<nb;i++)
		if ( name == videos_[i].video_name)
			return true;
	return false;
}