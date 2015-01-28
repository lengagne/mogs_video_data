// /*
//  * Code written by Vinz (GeckoGeek.fr)
//  */
//  
// #include <stdio.h>
// #include "video_interface.h"
// 
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <dirent.h>
// # include <iostream>
// 
// 
// using namespace tld;
// using namespace cv;
// 
// typedef struct{
// 	IplImage *image;
// 	CvPoint* Pt1;
// 	CvPoint* Pt2;
// 	bool * rect_set;
// }data;
// 
// //repere la position d'un clic droit ou gauche
// void PositionCurseur(int event, int x, int y, int flags, void* userdata){
// 	data *d= (data*) userdata;
// /*****************************création rectangle**************************************************/
// 	if(event == EVENT_LBUTTONDOWN){
// 		//cout << "1er point" << endl;
// 		//cout << ""<<x<<""<<y<<"" << endl;
// 		//pointeur Pt1 qui pointe vers un point en x et y
// 		d->Pt1->x=x;
// 		d->Pt1->y=y;
// 	}
// 	if(event == EVENT_LBUTTONUP){
// 		//cout << ""<<x<<""<<y<<"" << endl;
// 		d->Pt2->x=x;
// 		d->Pt2->y=y;
// // 			cvRectangle(Img,*Pt1,*Pt2,cvScalar(255,0,0),2,8,0);
// 		std::cout<<" Pt1 = "<< d->Pt1->x<<":"<< d->Pt1->y<<"\t Pt2 = "<< d->Pt2->x<<":"<< d->Pt2->y<<std::endl;
// 		*(d->rect_set) = true;
// 	}
// }
// 
// int main(int argc, char ** argv) {
// 	
// 	video_interface project;	
// 	if (argc == 1)
// 	{
// 		std::cerr<<"You can call mogs_video_tracking help for more help"<<std::endl;
// 		return 0;
// 	}
// 	
// 	if (argc == 3)
// 	{
// 		std::string command = argv[1];
// 		std::string param = argv[2];
// 		if (command.compare("new")==0)
// 		{
// 			std::cout<<" command = "<< command <<std::endl;
// 			std::cout<<" param = "<< param <<std::endl;
// 			project.new_project(param);
// 			return 1;
// 		}	
// 	}
// 	
// 	if (argc > 1)
// 	{
// 		std::string command = argv[1];
// 		if (command.compare("-completion")==0)
// 		{
// 			// part for the auto completion
// 			if (argc ==3)
// 			{
// 				std::cout<<"help new project show"<<std::endl;
// 			}else if( argc == 4)
// 			{
// 				std::string command1 = argv[2];
// 				if (command1.compare("new")==0)
// 				{
// 					std::cout<<"Project_name"<<std::endl;
// 				}else if (command1.compare("show")==0 ||
// 					  command1.compare("project")==0)
// 				{
// 					// print the list of all the project files
// 					struct dirent *dp;
// 					const char *dir_path="./";
// 					DIR *dir = opendir(dir_path);
// 					while ((dp=readdir(dir)) != NULL)
// 					{
// 						std::string tmp = dp->d_name;
// 						if(strstr (tmp.c_str (), ".xml"))
// 						{							
// 							int lastindex = tmp.find_last_of("."); 
// 							tmp = tmp.substr(0, lastindex); 
// 							std::cout<<tmp<<" "<<std::endl;
// 						}
// 					}
// 					closedir(dir);
// 				}
// 			}else if( argc == 5)
// 			{
// 				std::string command1 = argv[2];
// 				if (command1.compare("project")==0)
// 					std::cout<<"add_video add_point edit_tracking play"<<std::endl;
// 			}else if( argc == 6)
// 			{
// 				std::string command3 = argv[4];
// 				if (command3.compare("add_video")==0)
// 					std::cout<<"file: avi|mov"<<std::endl;
// 				if (command3.compare("add_point")==0)
// 					std::cout<<"point_name"<<std::endl;
// 				if (command3.compare("edit_tracking")==0 || command3.compare("play")==0)
// 				{
// 					std::string param = argv[3];
// 					if (project.read(param))
// 						project.show_video_list();
// 				}
// 			}else if( argc == 7)
// 			{
// 				std::string command3 = argv[4];
// 				if (command3.compare("edit_tracking")==0)
// 				{
// 					std::string param = argv[3];
// 					if (project.read(param))
// 						project.show_point_list();
// 				}else if (command3.compare("play")!=0 && command3.compare("add_point")!=0)
// 					std::cout<<"name"<<std::endl;
// 			}
// 			return 1;
// 		}
// 	}
// 	if (argc ==2)
// 	{
// 		std::string command = argv[1];
// 		if (command.compare("help") == 0)
// 		{
// 			std::cout<<"Help of mogs_video_tracking"<<std::endl;
// 			std::cout<<"\t mogs_video_tracking  new project_name : Create a new project."<<std::endl;
// 			std::cout<<"\t mogs_video_tracking  show project_name : Show the details of a project."<<std::endl;
// 			std::cout<<"\t mogs_video_tracking  project 'project_name' add_video 'video_file' 'video_name': Add a video to the project."<<std::endl;
// 			std::cout<<"\t mogs_video_tracking  project 'project_name' add_point 'point_name': Add a point to the project."<<std::endl;
// 			std::cout<<"\t mogs_video_tracking  project 'project_name' edit_tracking 'video_name' 'point_name' :  Edit the point for the video."<<std::endl;
// 			std::cout<<"\t mogs_video_tracking  project 'project_name' play video_name: Visualize one video."<<std::endl;
// 			return 1;
// 		}
// 	}
// 		
// 	if (argc == 3)
// 	{
// 		std::string command = argv[1];
// 		std::string param = argv[2];
// 		if (command.compare("show")==0)
// 		{
// 			// first we read the project
// 			if (!project.read(param))
// 			{
// 				std::cerr<<"There is some mistake and reading the project "<<param<<std::endl;
// 				return -1;
// 			}
// 			project.show();
// 			return 1;
// 		}
// 	}
// 	
// 	{
// 		std::string command = argv[1];
// 		if( command.compare("project")==0)
// 		{
// 			std::string param = argv[2];
// 			if (!project.read(param))
// 			{
// 				std::cerr<<"There is some mistake and reading the project "<<param<<std::endl;
// 				return -1;
// 			}		
// 			std::string command3 = argv[3];
// 			if (command3.compare("add_video")==0 && argc == 6)
// 			{
// 				std::string file = argv[4];
// 				std::string name = argv[5];
// 				if (!project.add_video_to_project(file, name))
// 				{
// 					std::cerr<<"Error cannot add the file : "<< file <<"."<<std::endl;
// 				}
// 				return 1;
// 			}else if (command3.compare("add_point")==0 && argc == 5)
// 			{
// 				std::string name = argv[4];
// 				if (!project.add_point_to_project(name))
// 				{
// 					std::cerr<<"Error cannot add the point : "<< name <<"."<<std::endl;
// 				}
// 				return 1;
// 			}else if (command3.compare("edit_tracking")==0 && argc == 6)
// 			{
// 				std::string video = argv[4];
// 				std::string point = argv[5];
// // 				project.read_data();
// 				project.edit_data( video, point);
// 				std::cout<<" editing point "<< point <<" on video: "<< video<<std::endl;
// 				return 1;
// 			}else if (command3.compare("play")==0 && argc == 5)
// 			{
// 				std::string video = argv[4];
// 				project.play_video( video);
// 				return 1;
// 			}
// 		}
// 	}
// 
// 	// if not return before : there might be a trouble
// 	std::cerr<<"This sequence is not defined "<<std::endl;
// 	for (int i = 0;i<argc;i++)
// 		std::cout<<"argv["<<i<<"] = "<< argv[i]<<std::endl;
// 	return 0;
// }

#include <QtGui/QApplication>
#include "mogs_video_widget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mogs_video_widget w;
	w.show();
	
	return a.exec();
}
