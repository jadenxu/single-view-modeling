#include "Vrml.h"


Vrml::Vrml(void)
{
	header = "#VRML V1.0 ascii\n"
		"PointLight{ intensity 1 location 50 100 300 }\n"
		"PointLight{ intensity 1 location 40 100 250 }\n"
		"PointLight{ intensity 1 location 60 100 250 }\n" 
		"PointLight{ intensity 1 location 20 100 100 }\n" 
		"PointLight{ intensity 3 location 80 100 100 }\n" 
		"PointLight{ intensity 5 location 50 400 50 }\n"
		"ShapeHints { \n"
		" vertexOrdering  COUNTERCLOCKWISE \n"
		" shapeType       SOLID \n"
		" }\n";

	initial_position = 
		"Separator { \n"
		"Transform {      translation -15 -30 -300}\n"
		"Transform {      rotation 0 1 0 -1.27   }\n" 
		"Transform {      rotation 1 0 0 0  } \n"
		"Transform {      rotation 0 0 1 -0.45  }\n";

}

void Vrml::appendPolygon(string filename, vector<Vector3i> world_pts, vector<Point> texture_pts){
	int x = 0, y = 1, z = 2;
	int ground_counter = 0;

	string separator = "Separator {\n";
	string coordinate3 = "Coordinate3 { point [\n";
	for (int i = 0; i < world_pts.size(); i++) {
		coordinate3 += "\t";
		coordinate3 += convertInt(world_pts[i](x));
		coordinate3 += " ";
		coordinate3 += convertInt(world_pts[i](y));
		coordinate3 += " ";
		coordinate3 += convertInt(world_pts[i](z));
		coordinate3 += " ,\n";

		if (world_pts[i](z) == 0)
			ground_counter++;
	}
	coordinate3 += "]}\n";

	string texture2 = 
		"Texture2 { filename \"" +
		filename +
		"\" }\n";

	string texture_coordinate2 = "TextureCoordinate2 { point [\n";
	for (int i = 0; i < texture_pts.size(); i++) {
		int x = texture_pts[i].x,
			y = texture_pts[i].y;

		if (x == 0 && y == 0)
			texture_coordinate2 += "0 1 ,\n";
		else if (x == 0 && y != 0)
			texture_coordinate2 += "0 0 ,\n";
		else if (x != 0 && y == 0)
			texture_coordinate2 += "1 1 ,\n";
		else if (x != 0 && y != 0)
			texture_coordinate2 += "1 0 ,\n";
	}
	texture_coordinate2 += "]}\n";

	string indexed_faceset;
	/*
	if (ground_counter == 4)
		indexed_faceset = "IndexedFaceSet { coordIndex [1, 0, 3, 2]}\n";
	else
	*/
	indexed_faceset = "IndexedFaceSet { coordIndex [0, 1, 2, 3]}\n";

	string end_separator = "}\n\n";

	vrml_file +=
		separator + 
		coordinate3 +
		texture2 +
		texture_coordinate2 +
		indexed_faceset +
		end_separator;
}

void Vrml::startVrml() {
	vrml_file = header + initial_position;
}

void Vrml::endVrml() {
	vrml_file += "}\n";
}

void Vrml::debug() {
	vector<Point> texture;
	vector<Vector3i> world;

	for (int i = 0; i < 4; i++) {
		texture.push_back(Point(i, 0));
		texture.push_back(Point(i, 1));
	}

	world.push_back(Vector3i(0, 0, 0));
	world.push_back(Vector3i(100, 0, 0));
	world.push_back(Vector3i(0, 100, 0));
	world.push_back(Vector3i(100, 100, 0));
	
	startVrml();
	appendPolygon("1.jpg", world, texture);
	appendPolygon("2.jpg", world, texture);
	appendPolygon("3.jpg", world, texture);
	endVrml();
}


string Vrml::convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

Vrml::~Vrml(void)
{
}
