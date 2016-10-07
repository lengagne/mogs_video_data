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

using namespace tld;
using namespace cv;


video_interface::video_interface():tld_(NULL)
{
	nb_frames_ = 1e9;
}

video_interface::~video_interface()
{

}

bool video_interface::add_point_to_project(const QString &name)
{
	if (point_exists(name))
	{
		std::cerr<<"The point "<<name.toStdString()<<" already exists."<<std::endl;
		exit(0);
	}

	// add the data to the xml
	QDomElement point = doc_->createElement("point");
	El_points_.appendChild (point);
	QDomText text = doc_->createTextNode (name);
	point.appendChild (text);

    QFile file(project_file_);
    file.open(QIODevice::ReadWrite);
    QByteArray xml = doc_->toByteArray();
    file.write(xml);
    file.close();

	return true;
}

bool video_interface::add_video_to_project(const QString &file,
					   const QString &name)
{
	if (video_exists(name))
	{
		std::cerr<<"The video "<<name.toStdString() <<" already exists."<<std::endl;
		exit(0);
	}

	// check and get the data about the video.
	video_description tmp;
	tmp.video_name = name;
	tmp.video_file = file;

	CvCapture  *capture = cvCaptureFromAVI( file.toStdString().c_str() );
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
	QDomElement video = doc_->createElement("video");
	El_videos_.appendChild (video);

	QDomNode node = doc_->createElement("video_name");
	QDomText text = doc_->createTextNode (tmp.video_name.toStdString().c_str ());
	node.appendChild (text);
	video.appendChild (node);

	node = doc_->createElement("video_file");
	text = doc_->createTextNode (tmp.video_file.toStdString().c_str ());
	node.appendChild (text);
	video.appendChild (node);

	node = doc_->createElement("width");
	text = doc_->createTextNode (std::to_string(tmp.width).c_str());
	node.appendChild (text);
	video.appendChild (node);

	node = doc_->createElement("height");
	text = doc_->createTextNode (std::to_string(tmp.height).c_str());
	node.appendChild (text);
	video.appendChild (node);

	node = doc_->createElement("fps");
	text = doc_->createTextNode (std::to_string(tmp.fps).c_str());
	node.appendChild (text);
	video.appendChild (node);

	node = doc_->createElement("nb_frames");
	text = doc_->createTextNode (std::to_string(tmp.nb_frames).c_str());
	node.appendChild (text);
	video.appendChild (node);

	node = doc_->createElement("duration");
	text = doc_->createTextNode (std::to_string(tmp.duration).c_str());
	node.appendChild (text);
	video.appendChild (node);

	//doc_.SaveFile (project_file_.c_str());
	save_data();

	return true;
}

bool video_interface::edit_data( const QString &video,
				 const QString &point_name,
				 int frame,
				 const CvPoint & point1,
				 const CvPoint & point2,
				 const EditType type,
				 IplImage *image)
{
	int video_id = get_video_id(video);
	int point_id = get_point_id(point_name);
	QDomElement el;
	QDomText text;
	QString tmp;
	switch(type)
	{
		case(MANUAL_EDITING):	std::cout<<"Manual editing"<<std::endl;
				el = point_exists(video,point_name,frame);
				if (!el.isNull())
				{
					// the point exist : we need to remove it
					El_datas_.removeChild (el);
				}
				// add the new point
				el = doc_->createElement("data");
				El_datas_.appendChild (el);
				el.setAttribute("frame",frame);
				el.setAttribute("video",video);
				el.setAttribute("point",point_name);
				el.setAttribute("source","manual");
				tmp = QString::number(point1.x)+ " " + QString::number(point1.y);
				text = doc_->createTextNode (tmp);
				el.appendChild (text);
				save_data();

				return false;
				break;
		case(TLD_EDITING):	// if the extractor does not exist we create it
				if(!image)
				{
					std::cout<<"No image loaded"<<std::endl;
					return false;
				}
				if(!tld_)
				{
					tld_ = new tld::TLD();
					int initialBB[4];
					if (point1.x < point2.x){
						initialBB[0] = point1.x;
						initialBB[2] = point2.x - point1.x;
					}
					else {
						initialBB[0] = point2.x;
						initialBB[2] = point1.x - point2.x;
					}
					if (point1.y < point2.y){
						initialBB[1] = point1.y;
						initialBB[3] = point2.y - point1.y;
					}
					else{
						initialBB[1] = point2.y;
						initialBB[3] = point1.y - point2.y;
					}

					cv::Mat grey(image->height, image->width, CV_8UC1);
					cvtColor(cv::cvarrToMat(image), grey, CV_BGR2GRAY);

					tld_->detectorCascade->imgWidth = grey.cols;
					tld_->detectorCascade->imgHeight = grey.rows;
					tld_->detectorCascade->imgWidthStep = grey.step;

					Rect bb = tldArrayToRect(initialBB);
					printf("Starting at %d %d %d %d\n", bb.x, bb.y, bb.width, bb.height);
					tld_->selectObject(grey, &bb);
				}

				// continue the tld algo
				tld_->processImage(cvarrToMat(image));
				if(tld_->currBB != NULL)
				{
					printf("%.2d %.2d %.2d %.2d %f\n", tld_->currBB->x, tld_->currBB->y, tld_->currBB->width, tld_->currBB->height, tld_->currConf);
				}
				else
				{
					printf("NaN NaN NaN NaN NaN\n");
				}
				double threshold = 0.5;
				if(tld_->currConf >= threshold)
				{
					if(tld_->currBB != NULL)
					{
						el = point_exists(video,point_name,frame);
						if (!el.isNull())
						{
							// the point exist : we need to remove it
							El_datas_.removeChild (el);
						}
						// add the new point
						el = doc_->createElement("data");
						El_datas_.appendChild (el);
						el.setAttribute("frame",frame);
						el.setAttribute("video",video);
						el.setAttribute("point",point_name);
						el.setAttribute("source","tld");
						tmp = QString::number(tld_->currBB->x+tld_->currBB->width/2)+ " " + QString::number(tld_->currBB->y+tld_->currBB->height/2);
						text = doc_->createTextNode (tmp);
						el.appendChild (text);
						save_data();
					}
				}
//				delete tld_;
//				tld_ = NULL;
				std::cout<<"TLD editing done"<<std::endl;

				return  true;
				break;
	}
}

void video_interface::get_images(const QString video_name,
				std::vector<IplImage *> & images,
				int * fps)
{
	images.clear();
	int video_id = get_video_id(video_name);
	CvCapture  *capture = cvCaptureFromAVI( videos_[video_id].video_file.toStdString().c_str() );
	if (!capture) {
		qDebug()<<"Cannot open "<< videos_[video_id].video_file << " !";
		return;
	}
	int nFrames = (int) cvGetCaptureProperty( capture , CV_CAP_PROP_FRAME_COUNT);
	*fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);

	images.resize(nFrames);

	for(int i=0;i<nFrames;i++)
	{
		images[i] = cvCloneImage( cvQueryFrame(capture));
	}
	cvReleaseCapture(&capture);
}

bool video_interface::get_point( const QString &video_name,
				 const QString &point_name,
				 int frame,
				 CvPoint& point) const
{
	int video_id = get_video_id(video_name);
	int point_id = get_point_id(point_name);

	return video_data_->get_value(frame,video_id,point_id,point);
}

int video_interface::get_point_id( const QString & name) const
{
	int nb = points_.size();
	for (int i=0;i<nb;i++)	if (name == points_[i])
		return i;
	return -1;
}

int video_interface::get_video_id( const QString & name) const
{
	int nb = videos_.size();
	for (int i=0;i<nb;i++)	if (name== videos_[i].video_name)
		return i;
	return -1;
}

std::vector<QString>  video_interface::get_videos_list()
{
	int nb = videos_.size();
	std::vector<QString> out;
	for (int i=0;i<nb;i++)
		out.push_back(videos_[i].video_name);
	return out;
}

QString video_interface::new_project(const QString project_name)
{
	project_file_ = project_name + ".xml";
	if(fopen(project_file_.toStdString().c_str(),"r")) {
		std::cerr<<"This project already exists !!"<<std::endl;
		return "";
	}

	// test if the file already exists.
/*	std::string command = "mkdir videos_"+project_name;
	int dummy = system(command.c_str());*/
	std::string std_name = project_name.toStdString();
	int lastindex = std_name.find_last_of(".");
	std::string rawname = std_name.substr(0, lastindex);
	std::string command = "cp /usr/local/share/MoGS/templates/mogs_project_video_tracking.xml " + rawname +".xml";
	int dummy = system (command.c_str());

	return project_file_;
}

bool video_interface::point_exists(const QString & name)
{
	int nb = points_.size();
	for (int i=0;i<nb;i++)
	{
		if ( name == points_[i])
			return true;
	}
	return false;
}

QDomElement video_interface::point_exists(	const QString & video_name,
							const QString & point_name,
							int frame)
{
	QDomElement El_data = El_datas_.firstChildElement ("data");
	for (El_data; !El_data.isNull(); El_data = El_data.nextSiblingElement ("data"))
	{
		int f = (El_data.attribute ("frame")).toInt();
		QString video = El_data.attribute ("video");
		QString point = El_data.attribute ("point");
		if (frame == f && video == video_name && point == point_name)
		{
			return El_data;
		}
	}
	return QDomElement();
}

bool video_interface::read(const QString project_name)
{
	project_file_ =project_name;
	QString xsd_url = QString(XSD_REPOSITORY) + QString("/project_video_tracking.xsd");

    QUrl schemaUrl(xsd_url);
    QXmlSchema schema;
    schema.load(schemaUrl);
    if (schema.isValid()) {
        QFile file(project_file_);
        file.open(QIODevice::ReadOnly);
        QXmlSchemaValidator validator(schema);
#ifdef DEBUG
        qDebug()<<" xsd is valid";
#endif
        if (! validator.validate(QUrl(QString("file://")+project_file_)))
        {
            qDebug() << "instance document "<< QString("file://")+project_file_ << " is invalid";
            return false;
        }
#ifdef DEBUG
        else
            qDebug() << "instance document "<< QString("file://")+path << " is valid";
#endif
    }else
    {
        qDebug()<<" Error the xsd "<< xsd_url << " is not valid";
        exit(0);
    }

    doc_ = new QDomDocument("Project");
    QFile file(project_file_);
    if (!file.open(QIODevice::ReadWrite))
    {
        return false;
    }
    if (!doc_->setContent(&file)) {
        file.close();
        return false;
    }
    file.close();
	root_ = doc_->documentElement ();

	if (root_.isNull())
	{
		std::cerr << " Error cannot find the root" << std::endl;
		return false;
	}

	El_des_ = root_.firstChildElement ("Definitions");
	if (El_des_.isNull())
	{
		std::cerr << " Error cannot find the Definitions" << std::endl;
		return false;
	}

	El_videos_ = El_des_.firstChildElement ("videos");
	if (El_videos_.isNull())
	{
		std::cerr << " Error cannot find the videos" << std::endl;
		return false;
	}

	int cpt = 0;
	QDomElement El_video = El_videos_.firstChildElement ("video");
	for (El_video; !El_video.isNull(); El_video = El_video.nextSiblingElement ("video"))
	{
		read_video_description(El_video);
		cpt++;
	}
// 	std::cout<<"nb_frames = "<<nb_frames_<<std::endl;

	El_points_ = El_des_.firstChildElement ("points");
	if (El_points_.isNull())
	{
		std::cerr << " Error cannot find the points" << std::endl;
		return false;
	}
	cpt = 0;
	QDomElement El_point = El_points_.firstChildElement ("point");
	for (El_point; !El_point.isNull(); El_point = El_point.nextSiblingElement ("point"))
	{
		points_.push_back(El_point.text());
		cpt++;
	}
// 	std::cout<<"there is/are "<<cpt<<" point(s)."<<std::endl;

	El_datas_ = root_.firstChildElement ("Datas");
	if (El_datas_.isNull())
	{
		std::cerr << " Error cannot find the Datas" << std::endl;
		return false;
	}

	read_data();

	return true;
}

bool video_interface::read_data( )
{
	video_data_ = new video_extracted_data(nb_frames_);
	std::cout<<"video_interface::read_data()"<<std::endl;
	int cpt = 0;
	QDomElement El_data = El_datas_.firstChildElement ("data");
	for (El_data; !El_data.isNull(); El_data = El_data.nextSiblingElement ("data"))
	{
		video_data tmp;
		tmp.frame = El_data.attribute ("frame").toInt();
		tmp.video = El_data.attribute ("video");
		tmp.video_id = get_video_id(tmp.video);
		tmp.point = El_data.attribute ("point");
		tmp.point_id = get_point_id(tmp.point);
		tmp.source = El_data.attribute ("source");
		std::string stmp = El_data.text ().toStdString();
		std::istringstream smallData (stmp, std::ios_base::in);
		smallData >> tmp.value.x;
		smallData >> tmp.value.y;
		video_data_->add_data(tmp);
		cpt++;
	}
	std::cout<<"number of data = "<< cpt <<std::endl;
	std::cout<<"number of data = "<< video_data_->get_number_data() <<std::endl;

	if (cpt>0)
		return true;
	else
		return false;
}

void video_interface::read_video_description(QDomElement El)
{
	video_description tmp_video;
	QDomElement name = El.firstChildElement ("video_name");
	if (name.isNull())
	{
		std::cerr << " Error cannot find the video_name" << std::endl;
		exit(0);
	}
	tmp_video.video_name = name.text();

	QDomElement file = El.firstChildElement ("video_file");
	if (file.isNull())
	{
		std::cerr << " Error cannot find the video_file" << std::endl;
		exit(0);
	}
	tmp_video.video_file = file.text();

	QDomElement width = El.firstChildElement ("width");
	if (width.isNull())
	{
		std::cerr << " Error cannot find the width" << std::endl;
		exit(0);
	}
	tmp_video.width = width.text().toInt();

	QDomElement height = El.firstChildElement ("height");
	if (height.isNull())
	{
		std::cerr << " Error cannot find the height" << std::endl;
		exit(0);
	}
	tmp_video.height = height.text().toInt();

	QDomElement fps = El.firstChildElement ("fps");
	if (fps.isNull())
	{
		std::cerr << " Error cannot find the fps" << std::endl;
		exit(0);
	}
	tmp_video.fps = fps.text().toInt();

	QDomElement nb_frames = El.firstChildElement ("nb_frames");
	if (nb_frames.isNull())
	{
		std::cerr << " Error cannot find the fps" << std::endl;
		exit(0);
	}
	tmp_video.nb_frames = nb_frames.text().toInt();
	if (nb_frames_ > tmp_video.nb_frames)
		nb_frames_ = tmp_video.nb_frames;

	QDomElement duration = El.firstChildElement ("duration");
	if (duration.isNull())
	{
		std::cerr << " Error cannot find the duration" << std::endl;
		exit(0);
	}
	tmp_video.duration = duration.text().toDouble();
	videos_.push_back(tmp_video);
}

bool video_interface::remove_point_to_project(const QString &name)
{
	if (!point_exists(name))
	{
		std::cerr<<"The point "<<name.toStdString()<<" does not exist and you try to remove it."<<std::endl;
		return false;
	}

	// remove from the xml
	QDomElement El_point = El_points_.firstChildElement ("point");
	for (El_point;!El_point.isNull(); El_point = El_point.nextSiblingElement ("point"))
	{
		QString tmp = El_point.text();
		if (tmp == name)
		{
			El_points_.removeChild(El_point);
		}
	}
	// save the xml
	save_data();

	// remove from the list files
	for (int i=0;i<points_.size();i++)
		if (points_[i]==name)
		{
			points_.erase(points_.begin()+i);
		}
	return true;
}

bool video_interface::remove_video_to_project(const QString &name)
{
	if (!video_exists(name))
	{
		std::cerr<<"The video "<<name.toStdString()<<" does not exist and you try to remove it."<<std::endl;
		return false;
	}

	// remove from the xml
	QDomElement El_video = El_videos_.firstChildElement ("video");
	for (El_video; !El_video.isNull(); El_video = El_video.nextSiblingElement ("video"))
	{
		QDomElement El_name = El_video.firstChildElement ("video_name");
		if (El_name.isNull())
		{
			std::cerr << " Error cannot find the video_name" << std::endl;
			exit(0);
		}

		QString tmp = El_name.text();
		if (tmp == name)
		{
			El_videos_.removeChild (El_video);
		}
	}
	// save the xml
	save_data();

	// remove from the list files
	for (int i=0;i<videos_.size();i++)
		if (videos_[i].video_name == name)
		{
			videos_.erase(videos_.begin()+i);
		}
	return true;
}

void video_interface::save_data( )
{
// 	std::vector<video_data> new_data;
// 	int nb= new_data.size();
// 	for (int i=0;i<nb;i++)
// 	{
// 		// add the data to the xml
// 		QDomElement data = doc_->createElement("data");
// 		El_datas_.appendChild (data);
// 		data.setAttribute("frame",new_data[i].frame);
// 		data.setAttribute("video",new_data[i].video.c_str());
// 		data.setAttribute("point",new_data[i].point.c_str());
// 		data.setAttribute("source",new_data[i].source.c_str());
// 		std::string tmp = QString::number(new_data[i].value.x)+ " " + QString::number(new_data[i].value.y);
// 		QDomText text = doc_->createTextNode (tmp.c_str ());
// 		data.appendChild (text);
// 	}
//	doc_.SaveFile (project_file_.c_str());
    QFile file(project_file_);
    file.open(QIODevice::ReadWrite);
    QByteArray xml = doc_->toByteArray();
    file.write(xml);
    file.close();

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
		qDebug()<<"\t"<< points_[i];
}

void video_interface::show_video_list() const
{
	int nb = videos_.size();
	for (int i=0;i<nb;i++)
		qDebug()<<"\t"<< videos_[i].video_name;
}

bool video_interface::video_exists(const QString & name)
{
	int nb = videos_.size();
	for (int i=0;i<nb;i++)
		if ( name == videos_[i].video_name)
			return true;
	return false;
}
