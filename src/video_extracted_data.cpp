//      video_extracted_data.cpp
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

#include "video_extracted_data.h"

video_extracted_data::video_extracted_data(int nb)
: nb_frames_(nb)
{
	
}

video_extracted_data::~video_extracted_data()
{
	
}

void video_extracted_data::add_data(const video_data& in)
{
	if (nb_frames_ > in.frame)
	{
		int nb = datas_.size();
		// test if we update the value or add a new one
		for (int i=0;i<nb;i++)
			if (updatable(datas_[i],in))
			{
				datas_[i] = in;
				return;
			}
		datas_.push_back(in);
	}
}

int video_extracted_data::get_next_version()const
{
	int num = -1;
	int nb = datas_.size();
	// test if we update the value or add a new one
	for (int i=0;i<nb;i++)
		if (datas_[i].version > num )
		{
			num = datas_[i].version;
		}
		
	return num+1;	
}

bool video_extracted_data::updatable( 	const video_data& a,
					const video_data& b) const
{
	if (a.frame != b.frame)
		return false;
	if (a.video_id != b.video_id)
		return false;
	if (a.point_id != b.point_id)
		return false;
	if (a.version > b.version)
		return false;
	return true;
}
