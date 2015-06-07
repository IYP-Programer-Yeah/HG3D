#include "HG3D Engine.h"
#include "objLoader.h"
#include <allocators>
namespace HG3D_Engine
{
	void Mesh::load_mesh(char *path)
	{
		objLoader MeshObj;
		float min[3];//minimum x,y,z for mesh cube
		float max[3];//maximum x,y,z for mesh cube
		MeshObj.load(path);//load the .obj file
		vert_nums = 3 * MeshObj.faceCount;//3 verts for each face
		faces_nums = MeshObj.faceCount;//get the number of faces
		total_size = vert_nums*vertex_size;//calculate total verts size
		verts = (vertex*)malloc(total_size);//allocate the vert memory
		faces = (face*)malloc(faces_nums*face_size);//allocate the faces memory
		for (register unsigned long int i = 0; i < faces_nums; i++)//feed the .obj data to mesh
		{
			faces[i].index[0] = i * 3;//there are no reuse of verts so just go forward
			faces[i].index[1] = i * 3 + 1;
			faces[i].index[2] = i * 3 + 2;
			for (register int j = 0; j < 3; j++)
			{
				//load vert positions
				verts[i * 3 + j].x = (float)MeshObj.vertexList[MeshObj.faceList[i]->vertex_index[j]]->e[0];
				verts[i * 3 + j].y = (float)MeshObj.vertexList[MeshObj.faceList[i]->vertex_index[j]]->e[1];
				verts[i * 3 + j].z = (float)MeshObj.vertexList[MeshObj.faceList[i]->vertex_index[j]]->e[2];
				for (register int k = 0; k < 3; k++)//go through xyz
					if ((float)MeshObj.vertexList[MeshObj.faceList[i]->vertex_index[j]]->e[k]>max[k])//check if more than max
						max[k] = (float)MeshObj.vertexList[MeshObj.faceList[i]->vertex_index[j]]->e[k];//reset max
					else if ((float)MeshObj.vertexList[MeshObj.faceList[i]->vertex_index[j]]->e[k]<min[k])//check if less than min
						min[k] = (float)MeshObj.vertexList[MeshObj.faceList[i]->vertex_index[j]]->e[k];//reset min
				//load vert normals
				verts[i * 3 + j].nx = (float)MeshObj.normalList[MeshObj.faceList[i]->normal_index[j]]->e[0];
				verts[i * 3 + j].ny = (float)MeshObj.normalList[MeshObj.faceList[i]->normal_index[j]]->e[1];
				verts[i * 3 + j].nz = (float)MeshObj.normalList[MeshObj.faceList[i]->normal_index[j]]->e[2];
				//load vert coords
				//verts[i * 3 + j].cx = (float)MeshObj.textureList[MeshObj.faceList[i]->texture_index[j]]->e[0];
				//verts[i * 3 + j].cy = (float)MeshObj.textureList[MeshObj.faceList[i]->texture_index[j]]->e[1];
			}
		}
		float mesh_cube_vals[2][3];
		for (register int i = 0; i < 3; i++)//feed mesh cube data
			mesh_cube_vals[0][i] = max[i];
		for (register int i = 0; i < 3; i++)//feed mesh cube data
			mesh_cube_vals[1][i] = max[i];
		mesh_center.build((max[0] + min[0]) / 2, (max[1] + min[1]) / 2, (max[2] + min[2]) / 2);//build mesh cube center
		for (register int i = 0; i < 8; i++)
			mesh_cube[i].build(mesh_cube_vals[i % 2][0], mesh_cube_vals[int(i / 2) % 2][1], mesh_cube_vals[int(i / 4)][2]); //build mesh cube
		model_matrix.LoadIdentity();//initialie model matrix
		needs_update = 1;//after loading mesh need to update the vbo

	}
	void Mesh::load_optimized(char *path)//load the .obj optimize for memory
	{

	}
	void Mesh::free_mesh() //free the mesh memory
	{
		free(verts); //free verts
		free(faces); //free faces
		if (clear_last_buff) //check if buffer should be cleared
		{
			glDeleteBuffers(1, &VBO_ID); //clear buffer
			glDeleteVertexArrays(1, &VAO_ID); //clear VAO
		}
	}
	void Mesh::update_vbo()//update the mesh vbo
	{
		if (clear_last_buff) //check if buffer should be cleared
		{
			glDeleteBuffers(1, &VBO_ID); //clear buffer
			glDeleteVertexArrays(1, &VAO_ID); //clear VAO
		}
		glGenVertexArrays(1, &VAO_ID);//make a new VAO
		glBindVertexArray(VAO_ID);//bind vao
		glGenBuffers(1, &VBO_ID);//make a new buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);//binde vbo
		glBufferData(GL_ARRAY_BUFFER, total_size, verts, GL_STATIC_DRAW);//send data to vbo
		glEnableVertexAttribArray(0);//set the index 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, 0);//set position first pos: 0-Vert_Pos_Size
		glEnableVertexAttribArray(1);//set the index 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_size, (void*)Vert_Pos_Size);//set normals first normal: Vert_Pos_Size-Vert_Normal_Size + Vert_Pos_Size 
		glEnableVertexAttribArray(2);//set the index 2
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertex_size, (void*)(Vert_Pos_Size + Vert_Normal_Size));//set position first pos: Vert_Normal_Size + Vert_Pos_Size -Vert_Normal_Size + Vert_Pos_Size + Vert_Coord_Size
		glBindBuffer(GL_ARRAY_BUFFER, 0);//unbinde vbo
		glBindVertexArray(0);//unbind vao
		clear_last_buff = 1;//can clear the buffer next time
	}
}