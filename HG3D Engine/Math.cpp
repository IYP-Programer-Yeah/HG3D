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
		return sqrt(x*x + y*y + z*z);
	}

	long double __fastcall vector::getsizeSq()//get vector size squared
	{
		return x*x + y*y + z*z;
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
#ifndef _M_X64
		ret.LoadZero();//set it to zero
		for (register int i = 0; i < 16; i++) //go through array
			for (register int j = 0; j < 4; j++) //go through columns and rows
				ret.x[i] += x[int(i / 4) * 4 +j] * input.x[j * 4 + i % 4]; //do the multipliction 
#else
		register __declspec(align(16)) __m128 temp[4];
		_MM_TRANSPOSE4_PS(input.data_row[0], input.data_row[1], input.data_row[2], input.data_row[3]);
		for (register int i = 0; i < 4; i++)
		{
			temp[0] = _mm_mul_ps(data_row[i], input.data_row[0]);
			temp[1] = _mm_mul_ps(data_row[i], input.data_row[1]);
			temp[2] = _mm_mul_ps(data_row[i], input.data_row[2]);
			temp[3] = _mm_mul_ps(data_row[i], input.data_row[3]);
			_MM_TRANSPOSE4_PS(temp[0], temp[1], temp[2], temp[3]);
			ret.data_row[i] = _mm_add_ps(temp[3], _mm_add_ps(temp[2], _mm_add_ps(temp[1], temp[0])));
		}
#endif
		return ret;
	}
	vector __fastcall _4x4matrix::operator * (vector input)//mat4 multiply
	{
		vector ret;
		ret.x = x[0] * input.x + x[1] * input.y + x[2] * input.z;
		ret.y = x[4] * input.x + x[5] * input.y + x[6] * input.z;//do the multiplication there is no w its vector
		ret.z = x[8] * input.x + x[9] * input.y + x[10] * input.z;
		return ret;
	}
	point __fastcall _4x4matrix::operator * (point input)//mat4 multiply
	{
		point ret;
		long double w;
		ret.x = x[0] * input.x + x[1] * input.y + x[2] * input.z + x[3];
		ret.y = x[4] * input.x + x[5] * input.y + x[6] * input.z + x[7];//do the multiplication there is no w its vector
		ret.z = x[8] * input.x + x[9] * input.y + x[10] * input.z + x[11];
		w = x[12] * input.x + x[13] * input.y + x[14] * input.z + x[15];//calculate w
		ret.x /= w;//divide by w
		ret.y /= w;
		ret.z /= w;
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
	void __fastcall _4x4matrix::LoadScaler(float sx,float sy,float sz)//load scaler matrix
	{
		LoadIdentity();//w is set to 1 all others are 0
		x[0] = sx;
		x[5] = sy;  //set the matrix
		x[10] = sz;
	}
	void __fastcall _4x4matrix::LoadTranslate(float sx, float sy, float sz)//load translate matrix
	{
		LoadIdentity();//load i
		x[3] = sx;
		x[7] = sy;  //set the matrix
		x[11] = sz;
	}
	void __fastcall _4x4matrix::LoadRotation(vector axis, point origin, float theta)//load rotation matrix
	{
		axis = normalize(axis);
		float u = (float)axis.x, v = (float)axis.y, w = (float)axis.z;
		float u2 = float(axis.x*axis.x), v2 = float(axis.y*axis.y), w2 = float(axis.z*axis.z);
		float a = (float)origin.x, b = (float)origin.y, c = (float)origin.z;
		float CosTheta = float(cos(double(theta)));
		float SinTheta = float(sin(double(theta)));
		x[0] = u2 + (v2 + w2)*CosTheta;					x[1] = u*v*(1.0f - CosTheta) - w*SinTheta;	x[2] = w*u*(1.0f - CosTheta) + v*SinTheta;	x[3] = (a*(v2 + w2) - u*(b*v + c*w))*(1.0f - CosTheta) + (b*w - c*v)*SinTheta;
		x[4] = u*v*(1.0f - CosTheta) + w*SinTheta;		x[5] = v2 + (u2 + w2)*CosTheta;				x[6] = w*v*(1.0f - CosTheta) - u*SinTheta;	x[7] = (b*(u2 + w2) - v*(a*u + c*w))*(1.0f - CosTheta) + (c*u - a*w)*SinTheta;
		x[8] = u*w*(1.0f - CosTheta) - v*SinTheta;		x[9] = w*v*(1.0f - CosTheta) + u*SinTheta;	x[10] = w2 + (v2 + u2)*CosTheta;			x[11] = (c*(u2 + v2) - w*(b*v + a*u))*(1.0f - CosTheta) + (a*v - b*u)*SinTheta;
		x[12] = 0.0f;									x[13] = 0.0f;								x[14] = 0.0f;								x[15] = 1.0f;
	}

	_4x4matrix LookAt(point cam_pos, vector forward, vector up)
	{
		vector xaxis, yaxis, zaxis;//define axises;
		vector eye;//campos in vetor shape
		eye.build(cam_pos.x, cam_pos.y, cam_pos.z);//set eye as campos
		zaxis = normalize(forward*-1.0f);//set z axis
		yaxis = normalize(up); //get y axis (already normalized)
		xaxis = normalize(cross(yaxis, zaxis));//get x axis normalized
		_4x4matrix return_mat;
		return_mat.x[0] = (float)xaxis.x;					return_mat.x[4] = (float)yaxis.x;					return_mat.x[8] = (float)zaxis.x;				return_mat.x[12] = (float)0.0;
		return_mat.x[1] = (float)xaxis.y;					return_mat.x[5] = (float)yaxis.y;					return_mat.x[9] = (float)zaxis.y;				return_mat.x[13] = (float)0.0;
		return_mat.x[2] = (float)xaxis.z;					return_mat.x[6] = (float)yaxis.z;					return_mat.x[10] = (float)zaxis.z;				return_mat.x[14] = (float)0.0;
		return_mat.x[3] = (float)-dot(xaxis, eye);		    return_mat.x[7] = (float)-dot(yaxis, eye);			return_mat.x[11] = (float)-dot(zaxis, eye);		return_mat.x[15] = (float)1.0;
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
		return_mat.x[0] = (float)2.0 * nearz / w;    return_mat.x[4] = (float)0.0;                   return_mat.x[8] = (float)0.0;						return_mat.x[12] = (float)0.0;
		return_mat.x[1] = (float)0.0;                return_mat.x[5] = (float)2.0 * nearz / h;       return_mat.x[9] = (float)0.0;						return_mat.x[13] = (float)0.0;
		return_mat.x[2] = (float)(right + left) / w; return_mat.x[6] = (float)(top + buttom) / h;    return_mat.x[10] = (float)-1.0*(farz + nearz) / d;	return_mat.x[14] = (float)-1.0;
		return_mat.x[3] = (float)0.0;                return_mat.x[7] = (float)0.0;                   return_mat.x[11] = (float)-2.0*nearz*farz / d;		return_mat.x[15] = (float)0.0;
		return return_mat;
	}

	_4x4matrix __fastcall Inverse(_4x4matrix input)//invers mat
	{
		float det = 0;
		_4x4matrix return_mat;
		return_mat.x[0] = input.x[5] * (input.x[10] * input.x[15] -
			input.x[11] * input.x[14]) -
			input.x[9] * (input.x[6] * input.x[15] -
			input.x[7] * input.x[14]) +
			input.x[13] * (input.x[6] * input.x[11] -
			input.x[7] * input.x[10]);

		return_mat.x[4] = -input.x[4] * (input.x[10] * input.x[15] -
			input.x[11] * input.x[14]) +
			input.x[8] * (input.x[6] * input.x[15] -
			input.x[7] * input.x[14]) -
			input.x[12] * (input.x[6] * input.x[11] -
			input.x[7] * input.x[10]);

		return_mat.x[8] = input.x[4] * (input.x[9] * input.x[15] -
			input.x[11] * input.x[13]) -
			input.x[8] * (input.x[5] * input.x[15] -
			input.x[7] * input.x[13]) +
			input.x[12] * (input.x[5] * input.x[11] -
			input.x[7] * input.x[9]);

		return_mat.x[12] = -input.x[4] * (input.x[9] * input.x[14] -
			input.x[10] * input.x[13]) +
			input.x[8] * (input.x[5] * input.x[14] -
			input.x[6] * input.x[13]) -
			input.x[12] * (input.x[5] * input.x[10] -
			input.x[6] * input.x[9]);

		return_mat.x[1] = -input.x[1] * (input.x[10] * input.x[15] -
			input.x[11] * input.x[14]) +
			input.x[9] * (input.x[2] * input.x[15] -
			input.x[3] * input.x[14]) -
			input.x[13] * (input.x[2] * input.x[11] -
			input.x[3] * input.x[10]);

		return_mat.x[5] = input.x[0] * (input.x[10] * input.x[15] -
			input.x[11] * input.x[14]) -
			input.x[8] * (input.x[2] * input.x[15] -
			input.x[3] * input.x[14]) +
			input.x[12] * (input.x[2] * input.x[11] -
			input.x[3] * input.x[10]);

		return_mat.x[9] = -input.x[0] * (input.x[9] * input.x[15] -
			input.x[11] * input.x[13]) +
			input.x[8] * (input.x[1] * input.x[15] -
			input.x[3] * input.x[13]) -
			input.x[12] * (input.x[1] * input.x[11] -
			input.x[3] * input.x[9]);

		return_mat.x[13] = input.x[0] * (input.x[9] * input.x[14] -
			input.x[10] * input.x[13]) -
			input.x[8] * (input.x[1] * input.x[14] -
			input.x[2] * input.x[13]) +
			input.x[12] * (input.x[1] * input.x[10] -
			input.x[2] * input.x[9]);

		return_mat.x[2] = input.x[1] * (input.x[6] * input.x[15] -
			input.x[7] * input.x[14]) -
			input.x[5] * (input.x[2] * input.x[15] -
			input.x[3] * input.x[14]) +
			input.x[13] * (input.x[2] * input.x[7] -
			input.x[3] * input.x[6]);

		return_mat.x[6] = -input.x[0] * (input.x[6] * input.x[15] -
			input.x[7] * input.x[14]) +
			input.x[4] * (input.x[2] * input.x[15] -
			input.x[3] * input.x[14]) -
			input.x[12] * (input.x[2] * input.x[7] -
			input.x[3] * input.x[6]);

		return_mat.x[10] = input.x[0] * (input.x[5] * input.x[15] -
			input.x[7] * input.x[13]) -
			input.x[4] * (input.x[1] * input.x[15] -
			input.x[3] * input.x[13]) +
			input.x[12] * (input.x[1] * input.x[7] -
			input.x[3] * input.x[5]);

		return_mat.x[14] = -input.x[0] * (input.x[5] * input.x[14] -
			input.x[6] * input.x[13]) +
			input.x[4] * (input.x[1] * input.x[14] -
			input.x[2] * input.x[13]) -
			input.x[12] * (input.x[1] * input.x[6] -
			input.x[2] * input.x[5]);

		return_mat.x[3] = -input.x[1] * (input.x[6] * input.x[11] -
			input.x[7] * input.x[10]) +
			input.x[5] * (input.x[2] * input.x[11] -
			input.x[3] * input.x[10]) -
			input.x[9] * (input.x[2] * input.x[7] -
			input.x[3] * input.x[6]);

		return_mat.x[7] = input.x[0] * (input.x[6] * input.x[11] -
			input.x[7] * input.x[10]) -
			input.x[4] * (input.x[2] * input.x[11] -
			input.x[3] * input.x[10]) +
			input.x[8] * (input.x[2] * input.x[7] -
			input.x[3] * input.x[6]);

		return_mat.x[11] = -input.x[0] * (input.x[5] * input.x[11] -
			input.x[7] * input.x[9]) +
			input.x[4] * (input.x[1] * input.x[11] -
			input.x[3] * input.x[9]) -
			input.x[8] * (input.x[1] * input.x[7] -
			input.x[3] * input.x[5]);

		return_mat.x[15] = input.x[0] * (input.x[5] * input.x[10] -
			input.x[6] * input.x[9]) -
			input.x[4] * (input.x[1] * input.x[10] -
			input.x[2] * input.x[9]) +
			input.x[8] * (input.x[1] * input.x[6] -
			input.x[2] * input.x[5]);

		det = input.x[0] * return_mat.x[0] + input.x[1] * return_mat.x[4] + input.x[2] * return_mat.x[8] + input.x[3] * return_mat.x[12];

		if (det == 0)
		{
			return_mat.LoadZero();
				return return_mat;//det is zero it's not inversable
		}
		for (register int i = 0; i < 16; i++)
			return_mat.x[i] = return_mat.x[i] / det;

		return return_mat;
	}

	_4x4matrix __fastcall Transpose(_4x4matrix input)//transpose mat
	{
		_4x4matrix return_mat;
#ifndef _M_X64
		for (register int i = 0; i < 16; i++)
			return_mat.x[i] = input.x[(i%4)*4+int(i/4)];
#else
		return_mat.data_row[0] = input.data_row[0];
		return_mat.data_row[1] = input.data_row[1];
		return_mat.data_row[2] = input.data_row[2];
		return_mat.data_row[3] = input.data_row[3];
		_MM_TRANSPOSE4_PS(return_mat.data_row[0],
			return_mat.data_row[1],
			return_mat.data_row[2],
			return_mat.data_row[3]);
#endif
		return return_mat;
	}

	factorial::factorial()
	{
		Last_Fact = 0;
		data = (float*)malloc((Max_Facts + 1)*sizeof(float));
		data[0] = 1;//0! = 1
	}
	factorial::~factorial()
	{
		free(data);
	}
	float factorial::get(unsigned int N)//calculate facts
	{
		if (N > Max_Facts)//invalid value
			return 0;
		if (N > Last_Fact)//hasnt yet been calculated
			for (register unsigned int i = Last_Fact + 1; i <= N; i++)//calculate from Last_Fact+ to N
				data[i] = data[i - 1] * i;
		Last_Fact = N;//just got calculated
		return data[N];
	}

	float fact(unsigned int N)//calculate fact
	{
		static factorial facts;//will contain the LUT
		return facts.get(N);
	}

	ALP::ALP()
	{
		Functions = (float*)malloc(sizeof(float)*Max_Band*(Max_Band + 1) / 2 * Max_Calculated_Vlaues);//allocate enough memory
		XVlues = (float*)malloc(sizeof(float) * Max_Calculated_Vlaues);//allocate enough memory
		last_L = 0;//fisrt band is set
		for (register unsigned long int k = 0; k < Max_Calculated_Vlaues; k++)
			Functions[k] = 1;//set the first band
	}
	ALP::~ALP()
	{
		free(Functions);
		free(XVlues);
	}
	void ALP::Calculate_Function(unsigned long int L, unsigned long int M, unsigned long int X)//calculate the function to the point givven
	{
		unsigned long int current_index = Calculate_Coords(last_L + 1, 0, 0);//the starting index
		unsigned long int temp_offset[2], temp_coeeffs[3];
		for (register unsigned long int i = last_L + 1; i <= L; i++)//go through the bands
		{
			temp_offset[0] = i * Max_Calculated_Vlaues;
			temp_offset[1] = temp_offset[0] + temp_offset[0] - Max_Calculated_Vlaues;
			temp_coeeffs[0] = 2 * i - 1;
			for (register unsigned long int j = 0; j < i - 1; j++)//go though functions in bands
			{
				temp_coeeffs[1] = i + j - 1;
				temp_coeeffs[2] = i - j;
				for (register unsigned long int k = 0; k < Max_Calculated_Vlaues; k++)//calculate the values for the function
				{
					Functions[current_index] = (XVlues[k] * float(temp_coeeffs[0]) * float(Functions[current_index - temp_offset[0]]) - float(temp_coeeffs[1])*float(Functions[current_index - temp_offset[1]])) / float(temp_coeeffs[2]);
					current_index++;
				}
			}
			temp_coeeffs[0] = 2 * i - 1;
			for (register unsigned long int k = 0; k < Max_Calculated_Vlaues; k++)//calculate the values for the function
			{
				Functions[current_index] = XVlues[k] * float(temp_coeeffs[0]) * float(Functions[current_index - temp_offset[0]]);
				current_index++;
			}
			temp_offset[0] += Max_Calculated_Vlaues;
			for (register unsigned long int k = 0; k < Max_Calculated_Vlaues; k++)//calculate the values for the function
			{
				Functions[current_index] = -float(sqrt(float(1.0f - XVlues[k] * XVlues[k]))) * float(temp_coeeffs[0]) * float(Functions[current_index - temp_offset[0]]);
				current_index++;
			}
		}
		last_L = L;
	}
	unsigned long int ALP::Calculate_Coords(unsigned long int L, unsigned long int M, unsigned long int X)
	{
		unsigned long int result = (L*(L + 1) / 2 + M)*Max_Calculated_Vlaues + X;//get the index
		return result;
	}
	float ALP::Get_Function(unsigned long int L, unsigned long int M, unsigned long int X)//get the function value
	{
		if (L >= Max_Band || M > L || X >= Max_Calculated_Vlaues)//invalid input
			return 0.0f;
		if (L > last_L)//the value is not calculated yet
			Calculate_Function(L, M, X);
		return Functions[Calculate_Coords(L, M, X)];

	}

	double calcALP(int l, int m, double x)
	{
		// evaluate an Associated Legendre Polynomial P(l,m,x) at x 
		double pmm = 1.0;
		if (m>0) {
			double somx2 = sqrt((1.0 - x)*(1.0 + x));
			double fact = 1.0;
			for (int i = 1; i <= m; i++) {
				pmm *= (-fact) * somx2;
				fact += 2.0;
			}
		}
		if (l == m) return pmm;
		double pmmp1 = x * (2.0*m + 1.0) * pmm;
		if (l == m + 1) return pmmp1;
		double pll = 0.0;
		for (int ll = m + 2; ll <= l; ++ll) {
			pll = ((2.0*ll - 1.0)*x*pmmp1 - (ll + m - 1.0)*pmm) / (ll - m);
			pmm = pmmp1;
			pmmp1 = pll;
		}
		return pll;
	}

	double calcK(int l, int m)
	{
		// renormalisation constant for SH function 
		double temp = ((2.0*l + 1.0)*fact(l - m)) / (4.0*PI*fact(l + m));
		return sqrt(temp);
	}

	double calcSH(int l, int m, double theta, double phi)
	{
		const double sqrt2 = sqrt(2.0);
		if (m == 0) return calcK(l, 0)*calcALP(l, m, cos(theta));
		else if (m>0) return sqrt2*calcK(l, m)*cos(m*phi)*calcALP(l, m, cos(theta));
		else return sqrt2*calcK(l, -m)*sin(-m*phi)*calcALP(l, -m, cos(theta));
	}
}