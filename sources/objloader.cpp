				#include "../headers/objloader.h"


bool loadOBJ(const char * path,vector<vec3> & out_vertices,vector<vec2> & out_uvs,vector<vec3> & out_normals){
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<vec3> temp_vertices;
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;
	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}
	while( 1 ){
		char lineHeader[128];
				// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		if ( strcmp( lineHeader, "v" ) == 0 ){
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		vec3 vertex = temp_vertices[ vertexIndex-1 ];
		out_vertices.push_back(vertex);
	}
	for( unsigned int i=0; i<uvIndices.size(); i++ ){
		unsigned int uvIndex = uvIndices[i];
		vec2 uv = temp_uvs[ uvIndex-1 ];
		out_uvs.push_back(uv);
	}
	for( unsigned int i=0; i<normalIndices.size(); i++ ){
		unsigned int normalIndex = normalIndices[i];
		vec3 normal = temp_normals[ normalIndex-1 ];
		out_normals.push_back(normal);
	}
	return true;
}