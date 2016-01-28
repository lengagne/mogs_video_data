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


#include "config_Video_data.h"
#include "video_extracted_data.h"
#include "video_extractor.h"
#include <QString>

typedef struct
{
	std::string video_name;
	std::string video_file;
	int width;
	int height;
	int fps;
	int nb_frames;
	double duration;
}video_description;

enum EditType
{
	MANUAL_EDITING,
	TLD_EDITING
};

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
	
	/** Edit the project file for point in video 
	 *  Return false when the editing is finished
	 **/
	bool edit_data( const std::string &video,
			const std::string &point_name,
			int frame,
			const CvPoint & point1,
			const CvPoint & point2,
			const EditType type,
			IplImage *image = NULL);
	
	/** Get all the images from the video */
	void get_images(const std::string video_name,
			std::vector<IplImage *> & images,
			int * fps);
	
	/** Return true if the point exists*/
	bool get_point( const std::string &video_name,
			const std::string &point_name,
			int frame,
			CvPoint& point) const;
	
	/** Return the list of the existing points **/
	std::vector<std::string> get_points_list() const
	{
		return points_;
	}
	
	/** Return the list of the existing points **/
	std::vector<std::string>  get_videos_list();
	
	/** Create a new project and return the name of the file**/
	QString new_project(const std::string project_name);
	
	/** Visualize the video **/
	void play_video(const std::string &video);
	
	/** read a project and return false in case of trouble */
	bool read(const std::string project_name);
	
	/** Read the data  */
	bool read_data( );
	
	/** Remove point from the list	*/
	bool remove_point_to_project(const std::string &name);
	
	/** Remove point from the list	*/
	bool remove_video_to_project(const std::string &name);
	
	/** Save the new selected values **/
	void save_data();
	
	/** Show the details of a project **/
	void show() const;
	
	/** Show the list of the points **/
	void show_point_list() const;
	
	/** Show the list of the videos **/
	void show_video_list() const;
private:
	std::string project_file_;

	/** Get the id of the point **/
	int get_point_id( const std::string & name) const;
	
	/** Get the id of the video **/
	int get_video_id( const std::string & name) const;

	/** Check if a point name is already defined 
	 *	return false if the video does not exists
	 */
	bool point_exists(const std::string & name);	
	
	/** If a point exist, it returns the xml node
	 */
	tinyxml2::XMLElement * point_exists(const std::string & video,
					    const std::string & point_name,
						int frame);
	
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
	tinyxml2::XMLElement * El_des_, *El_datas_;

	/** node for description of video and of point */
	tinyxml2::XMLElement * El_videos_, *El_points_;

	/** processing variables	*/
	std::vector<video_description> videos_;

	std::vector<std::string> points_;

	video_extracted_data *video_data_;
	
//	video_extractor *extractor_;
	
	int nb_frames_;		// the minimal number of frames for all the videos
	
	/** Variables for the tld algorithms **/
	tld::TLD * tld_ ;
};
#endif