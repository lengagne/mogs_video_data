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
#include <QFile>

typedef struct
{
	QString video_name;
	QString video_file;
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
	bool add_point_to_project(const QString &name);


	/** Add a video to the xml of the project*/
	bool add_video_to_project(const QString &file,
				  const QString &name);

	/** Edit the project file for point in video
	 *  Return false when the editing is finished
	 **/
	bool edit_data( const QString &video,
			const QString &point_name,
			int frame,
			const CvPoint & point1,
			const CvPoint & point2,
			const EditType type,
			IplImage *image = NULL);

	/** Get all the images from the video */
	void get_images(const QString video_name,
			std::vector<IplImage *> & images,
			int * fps);

	/** Return true if the point exists*/
	bool get_point( const QString &video_name,
			const QString &point_name,
			int frame,
			CvPoint& point) const;

	/** Return the list of the existing points **/
	std::vector<QString> get_points_list() const
	{
		return points_;
	}

	/** Return the list of the existing points **/
	std::vector<QString>  get_videos_list();

	/** Create a new project and return the name of the file**/
	QString new_project(const QString project_name);

	/** read a project and return false in case of trouble */
	bool read(const QString project_name);

	/** Read the data  */
	bool read_data( );

	/** Remove point from the list	*/
	bool remove_point_to_project(const QString &name);

	/** Remove point from the list	*/
	bool remove_video_to_project(const QString &name);

	/** Save the new selected values **/
	void save_data();

	/** Show the details of a project **/
	void show() const;

	/** Show the list of the points **/
	void show_point_list() const;

	/** Show the list of the videos **/
	void show_video_list() const;
private:
	QString project_file_;

	/** Get the id of the point **/
	int get_point_id( const QString & name) const;

	/** Get the id of the video **/
	int get_video_id( const QString & name) const;

	/** Check if a point name is already defined
	 *	return false if the video does not exists
	 */
	bool point_exists(const QString & name);

	/** If a point exist, it returns the xml node
	 */
	QDomElement  point_exists(const QString & video,
					    const QString & point_name,
						int frame);

	/** Read the video description in the file */
	void read_video_description(QDomElement El);

	/** Check if a video name is already defined
	 *	return false if the video does not exists
	 */
	bool video_exists(const QString & name);

	/** read the xml project variables */
	QDomDocument * doc_;

	/** main node of the xml file */
	QDomElement root_;

	/** node for descriptions and data*/
	QDomElement El_des_, El_datas_;

	/** node for description of video and of point */
	QDomElement  El_videos_, El_points_;

	/** processing variables	*/
	std::vector<video_description> videos_;

	std::vector<QString> points_;

	video_extracted_data *video_data_;


	int nb_frames_;		// the minimal number of frames for all the videos

	/** Variables for the tld algorithms **/
	tld::TLD * tld_ ;
};
#endif
