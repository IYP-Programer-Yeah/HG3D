#include "HG3D Engine.h"
namespace HG3D_Engine
{
	void __fastcall point::build(long double ix, long double iy, long double iz) //build point
	{
		x = ix; //set x value
		y = iy; //set y value
		z = iz; //set z value
	}
	void __fastcall point::build(float ix, float iy, float iz) //build point
	{
		x = long double(ix); //set x value
		y = long double(iy); //set y value
		z = long double(iz); //set z value
	}
	void __fastcall point::move(long double ax, long double ay, long double az) //move the point
	{
		x = x + ax; //move along x axis
		y = y + ay; //move along y axis
		z = z + az; //move along z axis
	}
	void __fastcall point::rotate(point from, long double ax, long double ay, long double az) //rotate point
	{
		long double dx, dy, dz;//point coord in new space whit "from" as the origin
		dx = x - from.x; //transform x to new world
		dy = y - from.y; //transform y to new world
		dz = z - from.z; //transform z to new world
		x = from.x + (dx*(cos(ay)*cos(az)) + dy*(-1 * cos(ax)*sin(az) + sin(ax)*sin(ay)*cos(az)) + dz*(sin(ax)*sin(az) + cos(ax)*sin(ay)*cos(az))); //do the rotation on x
		y = from.y + (dx*(cos(ay)*sin(az)) + dy*(cos(ax)*cos(az) + sin(ax)*sin(ay)*sin(az)) + dz*(-1 * sin(ax)*cos(az) + cos(ax)*sin(ay)*sin(az))); //do the rotation on y
		z = from.z + (dx*-1 * sin(ay) + dy*sin(ax)*cos(ay) + dz*cos(ax)*cos(ay));                                                                   //do the rotation on z
	}
	void __fastcall point::move(float ax, float ay, float az) //just do a conversion
	{
		move(long double(ax), long double(ay), long double(az));
	}
	void __fastcall point::rotate(point from, float ax, float ay, float az)//rotate point
	{                           
		rotate(from, long double(ax), long double(ay), long double(az));//just do a conversion
	}
	void __fastcall point::operator =(point intery) //operator =
	{
		build(intery.x, intery.y, intery.z);//build the point
	}


	void __fastcall vector::build(point start, point end)//point to point vector
	{
		x = end.x - start.x;
		y = end.y - start.y;
		z = end.z - start.z;
	}
	void __fastcall vector::build(long double ix, long double iy, long double iz)//simpil build
	{
		x = ix;
		y = iy;
		z = iz;
	}
	void __fastcall vector::build(float ix, float iy, float iz)//simpile build with float
	{
		x = long double(ix); //set x value
		y = long double(iy); //set y value
		z = long double(iz); //set z value
	}
	void __fastcall vector::operator =(vector intery)//set value
	{
		build(intery.x, intery.y, intery.z);
	}
	vector __fastcall vector::operator + (vector entry) //vector vector sum
	{
		vector ret;
		ret.build(entry.x + x, entry.y + y, entry.z + z);
		return ret;
	}
	vector __fastcall vector::operator *(long double entry)//vector number multipliction
	{
		vector ret;
		ret.build(entry*x, entry*y, entry*z);
		return ret;
	}
	vector __fastcall vector::operator /(long double entry)//division by long double
	{
		vector ret;
		ret.build(x / entry, y / entry, z / entry);
		return ret;
	}
	vector __fastcall vector::operator *(float entry)//vector float number multipliction
	{
		vector ret;
		ret.build(x * long double(entry), y* long double(entry), z * long double(entry));
		return ret;
	}
	vector __fastcall vector::operator /(float entry)//division by float
	{
		vector ret;
		ret.build(x / long double(entry), y / long double(entry), z / long double(entry));
		return ret;
	}
	long double __fastcall vector::getsize()//get vector size
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	long double __fastcall dot(vector v1, vector v2) //dot product
	{
		return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	}

	vector __fastcall cross(vector v1, vector v2) //cross product
	{
		vector temple;
		temple.build(v1.y*v2.z - v2.y*v1.z, v1.z*v2.x - v2.z*v1.x, v1.x*v2.y - v2.x*v1.y);
		return temple;
	}

	vector __fastcall normalize(vector v1)//normalize
	{
		return v1/v1.getsize();
	}

	void __fastcall _4x4matrix::operator = (_4x4matrix input)//mat4 multiply
	{
		for (register int i = 0; i < 16; i++) //go through array
			x[i] = input.x[i];
	}
	_4x4matrix __fastcall _4x4matrix::operator * (_4x4matrix input)//mat4 multiply
	{
		_4x4matrix ret;
		ret.LoadZero();//set it to zero
		for (register int i = 0; i < 16; i++) //go through array
			for (register int j = 0; j < 4; j++) //go through columns and rows
				ret.x[i] += x[int(i / 4) * 4 +j] * input.x[j * 4 + i % 4]; //do the multipliction 
		return ret;
	}
	void __fastcall _4x4matrix::LoadIdentity()//load matric with i
	{
		for (register int i = 0; i < 16; i++)
			if (i % 5 == 0)
				x[i] = 1.0;
			else
				x[i] = 0.0;
	}
	void __fastcall _4x4matrix::LoadZero()//load matric with 0
	{
		for (register int i = 0; i < 16; i++)
				x[i] = 0.0;
	}
	void __fastcall _4x4matrix::LoadScaler(float sx,float sy,float sz)//load matric with 0
	{
		LoadIdentity();//w is set to 1 all others are 0
		x[0] = sx;
		x[5] = sy;  //set the matrix
		x[10] = sz;
	}

	_4x4matrix LookAt(point cam_pos, vector forward, vector up)
	{
		vector xaxis, yaxis, zaxis;//define axises;
		vector eye;//campos in vetor shape
		eye.build(cam_pos.x, cam_pos.y, cam_pos.z);//set eye as campos
		zaxis = normalize(forward*-1.0f);//set z axis
		xaxis = normalize(cross(up, zaxis));//get x axis normalized
		yaxis = cross(zaxis, xaxis); //get y axis (already normalized)
		_4x4matrix return_mat;
		return_mat.x[0] = (float)xaxis.x;					return_mat.x[1] = (float)yaxis.x;					return_mat.x[2] = (float)zaxis.x;				return_mat.x[3] = (float)0.0;
		return_mat.x[4] = (float)xaxis.y;					return_mat.x[5] = (float)yaxis.y;					return_mat.x[6] = (float)zaxis.y;				return_mat.x[7] = (float)0.0;
		return_mat.x[8] = (float)xaxis.z;					return_mat.x[9] = (float)yaxis.z;					return_mat.x[10] = (float)zaxis.z;				return_mat.x[11] = (float)0.0;
		return_mat.x[12] = (float)-dot(xaxis, eye);		    return_mat.x[13] = (float)-dot(yaxis, eye);			return_mat.x[14] = (float)-dot(zaxis, eye);		return_mat.x[15] = (float)1.0;
		/*	xaxis.x           yaxis.x           zaxis.x			     0
			xaxis.y           yaxis.y           zaxis.y				 0
			xaxis.z           yaxis.z           zaxis.z				 0
			- dot(xaxis, eye) - dot(yaxis, eye) - dot(zaxis, eye)	 1*/
		return return_mat;
	}

	_4x4matrix Projection(float left, float right, float buttom, float top, float nearz, float farz)
	{
		_4x4matrix return_mat;
		float w = right - left;//width
		float h = top - buttom;//height
		float d = farz - nearz;//depth
		return_mat.x[0] = (float)2.0 * nearz / w;    return_mat.x[4] = (float)0.0;                   return_mat.x[8] = (float)0.0;						return_mat.x[12] = 0;
		return_mat.x[1] = (float)0.0;                return_mat.x[5] = (float)2.0 * nearz / h;       return_mat.x[9] = (float)0.0;						return_mat.x[13] = 0;
		return_mat.x[2] = (float)(right + left) / w; return_mat.x[6] = (float)(top + buttom) / h;    return_mat.x[10] = (float)-1.0*(farz + nearz) / d;	return_mat.x[14] = -1;
		return_mat.x[3] = (float)0.0;                return_mat.x[7] = (float)0.0;                   return_mat.x[11] = (float)-1.0*nearz*farz / d;		return_mat.x[15] = 0;
		return return_mat;
	}
}