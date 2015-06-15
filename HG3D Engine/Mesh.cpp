#include "HG3D Engine.h"
#include "objLoader.h"
#include <allocators>
namespace HG3D_Engine
{
	void face::operator=(face input)//= operator
	{
		for (register int i = 0; i < 3; i++)
			index[i] = input.index[i];//simply copy index
	}

	void vertex::operator=(vertex input)//= operator
	{
		x = input.x;//copy x y z
		y = input.y;
		z = input.z;
		nx = input.nx;//copy normals
		ny = input.ny;
		nz = input.nz;
		cx = input.cx;//copy coords
		cy = input.cy;
	}

	void Mat::operator = (Mat input)//= operator
	{
		for (register int i = 0; i < 3; i++)
		{
			Ambient[i] = input.Ambient[i];
			Specular[i] = input.Specular[i];//copy colors
			Diffuse[i] = input.Diffuse[i];
			Emission[i] = input.Emission[i];
			TF[i] = input.TF[i];
		}
		dissolve = input.dissolve;
		Spec_Exponent = input.Spec_Exponent;
		optical_density = input.optical_density;//copy floats
		roughness = input.roughness;
		sharpness = input.sharpness;
	}

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
				if (MeshObj.textureCount > 0)
				{
					verts[i * 3 + j].cx = (float)MeshObj.textureList[MeshObj.faceList[i]->texture_index[j]]->e[0];
					verts[i * 3 + j].cy = (float)MeshObj.textureList[MeshObj.faceList[i]->texture_index[j]]->e[1];
				}
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
		subdata_changed = 0;//the whole data gonna get updated no need for this
		indices = (unsigned long int*)faces;//get the pointer it'll be used to draw elements

	}
	void Mesh::load_optimized(char *path)//load the .obj optimize for memory
	{

	}

	void Mesh::scale_model(float x, float y, float z)
	{
		for (register int i = 0; i < 16; i+=4)
		{
			model_matrix.x[i] *= x;//we know what the matrix is so lets speed the process up
			model_matrix.x[i + 1] *= y;
			model_matrix.x[i + 2] *= z;
		}
	}
	void Mesh::rotate_model_AIC(float theta, vector axis)//about it's center
	{
		point center = model_matrix*mesh_center;//find the center 
		_4x4matrix temp_rotation;
		temp_rotation.LoadRotation(axis,center,theta);//rotate about the center around the axis
		model_matrix = temp_rotation*model_matrix;
	}
	void Mesh::rotate_model(float theta, point origin, vector axis)//about it's center
	{
		_4x4matrix temp_rotation;
		temp_rotation.LoadRotation(axis, origin, theta);//rotate about the center around the axis
		model_matrix = temp_rotation*model_matrix;
	}
	void Mesh::move(vector movement)//about it's center
	{
		for (register int i = 0; i < 4; i++)
		{
			model_matrix.x[i] += model_matrix.x[12 + i] * float(movement.x);//we know what the matrix is so lets speed the process up
			model_matrix.x[i + 4] += model_matrix.x[12 + i] * float(movement.y);
			model_matrix.x[i + 8] += model_matrix.x[12 + i] * float(movement.z);
		}
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
		needs_update = 0;
		clear_last_buff = 1;//can clear the buffer next time
		subdata_changed = 0;//the whole data updated no need for this
	}
	void Mesh::remap_vbo()//update the mesh vbo
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);//bind the vbo
		GLvoid* VBO_Temp = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);//map buffer
		memcpy(VBO_Temp, verts, total_size);//copy data to vbo
		glUnmapBuffer(GL_ARRAY_BUFFER);//unmap buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);//unbind
		subdata_changed = 0;//jsut updated it
	}
	void Mesh::operator=(Mesh input)//= operator
	{
		//copy ints
		vert_nums = input.vert_nums;
		faces_nums = input.faces_nums;
		total_size = input.total_size;

		text_ID_diff = input.text_ID_diff;
		text_ID_diff = input.text_ID_spec;
		text_ID_normal = input.text_ID_normal;
		text_ID_height = input.text_ID_height;
		text_ID_mask = input.text_ID_mask;
		text_ID_alphamap = input.text_ID_alphamap;
		text_ID_dispmap = input.text_ID_dispmap;
		text_ID_TFmap = input.text_ID_TFmap;

		//copy bools
		have_diff_text = input.have_diff_text;
		have_spec_text = input.have_spec_text;
		have_NM_text = input.have_NM_text;
		have_HM_text = input.have_HM_text;
		have_mask_text = input.have_mask_text;
		have_alphamap_text = input.have_alphamap_text;
		have_dispmap_text = input.have_dispmap_text;
		have_TFmap_text = input.have_TFmap_text;

		hidden = input.hidden;
		clear_last_buff = 1;//can clear last buffers
		needs_update = 1;//the mesh has just been copied needs update
		subdata_changed = 0;//the whole data gonna get updated no need for this

		//copy mesh cube data
		for (register int i = 0; i < 8; i++)
			mesh_cube[i] = input.mesh_cube[i];
		mesh_center = input.mesh_center;
		model_matrix = input.model_matrix;

		verts = (vertex*)malloc(total_size);//allocate the vert memory
		faces = (face*)malloc(faces_nums*face_size);//allocate the faces memory
		//double the data
		for (register unsigned long int i = 0; i < faces_nums; i++)
			faces[i] = input.faces[i];
		for (register unsigned long int i = 0; i < vert_nums; i++)
			verts[i] = input.verts[i];
		indices = (unsigned long int*)faces; 
		for (register int i = 0; i < 4; i++)
			MeshID[i] = input.MeshID[i];

	}

	void Mesh::clone_NMA(Mesh input)		//clone with the same memory allocation (no new memory is allocated)
	{
		//copy ints
		vert_nums = input.vert_nums;
		faces_nums = input.faces_nums;
		total_size = input.total_size;

		text_ID_diff = input.text_ID_diff;
		text_ID_diff = input.text_ID_spec;
		text_ID_normal = input.text_ID_normal;
		text_ID_height = input.text_ID_height;
		text_ID_mask = input.text_ID_mask;
		text_ID_alphamap = input.text_ID_alphamap;
		text_ID_dispmap = input.text_ID_dispmap;
		text_ID_TFmap = input.text_ID_TFmap;

		//copy bools
		have_diff_text = input.have_diff_text;
		have_spec_text = input.have_spec_text;
		have_NM_text = input.have_NM_text;
		have_HM_text = input.have_HM_text;
		have_mask_text = input.have_mask_text;
		have_alphamap_text = input.have_alphamap_text;
		have_dispmap_text = input.have_dispmap_text;
		have_TFmap_text = input.have_TFmap_text;

		hidden = input.hidden;
		clear_last_buff = input.clear_last_buff;
		needs_update = input.needs_update;
		subdata_changed = input.subdata_changed;
		//copy mesh cube data
		for (register int i = 0; i < 8; i++)
			mesh_cube[i] = input.mesh_cube[i];
		mesh_center = input.mesh_center;
		model_matrix = input.model_matrix;

		//copy pointers (using same memoru allocation)
		faces = input.faces;
		verts = input.verts;

		//same memory same vbo and same vao
		VAO_ID = input.VAO_ID;
		VBO_ID = input.VBO_ID;
		indices = input.indices;
		for (register int i = 0; i < 4; i++)
			MeshID[i] = input.MeshID[i];
	}
}