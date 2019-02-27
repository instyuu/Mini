#pragma once

#include "mono.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace loader {

struct triangle
{
	math::vec3 point_1, point_2, point_3;
};

enum obj_face_format
{
	no_backslash,
	has_backslash,
};

class obj_file
{
public:
	obj_file() = default;

	obj_file(const char *fp, obj_face_format fmt)
	{
		std::ifstream file;
		file.open(fp);
		char line_type;
		math::vec3 vert_tmp;
		math::vec3 face_tmp;
		std::string line;

		while (getline(file, line))
		{
			// Line describes a vertex
			if (line[0] == 'v' && line[1] == ' ')
			{
				std::stringstream vertex_ss(line);
				vertex_ss >> line_type >> vert_tmp.x_ >> vert_tmp.y_ >> vert_tmp.z_;
				vertices_.push_back(vert_tmp);
			}
			/*
			Line describes face, DEPENDS ON HOW IT WAS EXPORTED:
			Two possibilities:
				f a\\1 b\\1 c\\1
				OR
				f a b c
			Regardless, we have to be able to load both...
			Note that vertices are always above faces w/in an .obj file,
			so accessing the vertices_ vector will always be valid, as all
			of the vertices will have been loaded by the time we get to the
			faces
			*/
			if (line[0] == 'f' && line[1] == ' ')
			{
				if (fmt == no_backslash)
				{
					std::stringstream face_ss(line);
					face_ss >> line_type >> face_tmp.x_ >> face_tmp.y_ >> face_tmp.z_;
					faces_.push_back(triangle{ vertices_[face_tmp.x_ - 1],
						vertices_[face_tmp.y_ - 1],
						vertices_[face_tmp.z_ - 1] });
				}
				else if (fmt == has_backslash)
				{
					// I have to use the old C sscanf b/c stringstreams are stupid
					int normal_i = 0, normal_j = 0, normal_k = 0, normal_n = 0;
					int face_i = 0, face_j = 0, face_k, face_n = 0;
					auto c_str = line.c_str();
					auto space_count = std::count(line.begin(), line.end(), ' ');
					if (space_count == 3)
					{
						sscanf(c_str, "f %i//%i %i//%i %i//%i",
							&face_i, &normal_i, 
							&face_j, &normal_j, 
							&face_k, &normal_k);
						faces_.push_back(triangle{ vertices_[face_i - 1],
							vertices_[face_j - 1], vertices_[face_k - 1] });
					}
					else if (space_count == 4)
					{
						sscanf(c_str, "f %i//%i %i//%i %i//%i %i//%i",
							&face_i, &normal_i, 
							&face_j, &normal_j, 
							&face_k, &normal_k,
							&face_n, &normal_n);
						faces_.push_back(triangle{ vertices_[face_i - 1],
							vertices_[face_j - 1], vertices_[face_k - 1] });
						faces_.push_back(triangle{ vertices_[face_i - 1],
							vertices_[face_k - 1], vertices_[face_n - 1] });
					}
					// do something with the normals...
				}
			}
		}

		file.close();
	}

	obj_file(const obj_file &o)
		: vertices_{o.vertices_}
	{
	}

	std::vector<math::vec3> get_vertices() const
	{
		return vertices_;
	}

	std::vector<triangle> get_faces() const
	{
		return faces_;
	}
private:
	std::vector<math::vec3> vertices_;
	std::vector<triangle> faces_;
};

}