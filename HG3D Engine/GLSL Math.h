#pragma once
#include <math.h>
#include <algorithm>
namespace GLSL_Math
{
	struct vec2
	{
		union
		{
			struct
			{
				float x, y;
			};
		};
		vec2() = default;
		vec2(const float ix, const float iy)
		{
			x = ix;
			y = iy;
		}
		vec2(const float n)
		{
			x = n;
			y = n;
		}
		vec2(const vec2 &ixy)
		{
			x = ixy.x;
			y = ixy.y;
		}
		vec2 &operator = (const vec2 &ixy)
		{
			x = ixy.x;
			y = ixy.y;
			return *this;
		}

		vec2 operator +(const vec2 &inp)const
		{
			return vec2(inp.x + x, inp.y + y);
		}
		vec2 operator -(const vec2 &inp)const
		{
			return vec2(x - inp.x, y - inp.y);
		}
		vec2 operator *(const vec2 &inp)const
		{
			return vec2(inp.x * x, inp.y * y);
		}
		vec2 operator /(const vec2 &inp)const
		{
			return vec2(x / inp.x, y / inp.y);
		}

		vec2 operator +(const float inp)const
		{
			return vec2(inp + x, inp + y);
		}
		vec2 operator -(const float inp)const
		{
			return vec2(x - inp, y - inp);
		}
		vec2 operator *(const float inp)const
		{
			return vec2(inp * x, inp * y);
		}
		vec2 operator /(const float inp)const
		{
			return vec2(x / inp, y / inp);
		}
	};
	struct vec3
	{
		union
		{
			struct
			{
				float x, y, z;
			};
			struct
			{
				vec2 xy;
				float r;
			};
			struct
			{
				float s;
				vec2 yz;
			};
		};
		vec3() = default;
		vec3(const float ix, const float iy, const float iz)
		{
			x = ix;
			y = iy;
			z = iz;
		}
		vec3(const float n)
		{
			x = n;
			y = n;
			z = n;
		}
		vec3(const vec2 ixy, const float iz)
		{
			xy = ixy;
			z = iz;
		}
		vec3(const float ix, const vec2 iyz)
		{
			yz = iyz;
			x = ix;
		}
		vec3(const vec3& ixyz)
		{
			x = ixyz.x;
			yz = ixyz.yz;
		}
		vec3 &operator = (const vec3 ixyz)//copy
		{
			x = ixyz.x;
			yz = ixyz.yz;
			return *this;
		}
		explicit operator vec2&()//vec3 to vec2
		{
			return xy;
		}

		vec3 operator +(const vec3 &inp)const
		{
			return vec3(inp.x + x, inp.y + y, inp.z + z);
		}
		vec3 operator -(const vec3 &inp)const
		{
			return vec3(x - inp.x, y - inp.y, z - inp.z);
		}
		vec3 operator *(const vec3 &inp)const
		{
			return vec3(inp.x * x, inp.y * y, inp.z * z);
		}
		vec3 operator /(const vec3 &inp)const
		{
			return vec3(x / inp.x, y / inp.y, z / inp.z);
		}

		vec3 operator +(const float inp)const
		{
			return vec3(inp + x, inp + y, inp + z);
		}
		vec3 operator -(const float inp)const
		{
			return vec3(x - inp, y - inp, z - inp);
		}
		vec3 operator *(const float inp)const
		{
			return vec3(inp * x, inp * y, inp * z);
		}
		vec3 operator /(const float inp)const
		{
			return vec3(x / inp, y / inp, z / inp);
		}
	};

#pragma once
	struct ivec2
	{
		union
		{
			struct
			{
				int x, y;
			};
		};
		ivec2() = default;
		ivec2(const int ix, const int iy)
		{
			x = ix;
			y = iy;
		}
		ivec2(const int n)
		{
			x = n;
			y = n;
		}
		ivec2(const ivec2 &ixy)
		{
			x = ixy.x;
			y = ixy.y;
		}
		ivec2(const vec2 &ixy)
		{
			x = int(ixy.x);
			y = int(ixy.y);
		}
		ivec2 &operator = (const ivec2 &ixy)
		{
			x = ixy.x;
			y = ixy.y;
			return *this;
		}
		operator vec2&()//ivec2 to vec2
		{
			return vec2(float(x), float(y));
		}

		ivec2 operator +(const ivec2 &inp)const
		{
			return ivec2(inp.x + x, inp.y + y);
		}
		ivec2 operator -(const ivec2 &inp)const
		{
			return ivec2(x - inp.x, y - inp.y);
		}
		ivec2 operator *(const ivec2 &inp)const
		{
			return ivec2(inp.x * x, inp.y * y);
		}
		ivec2 operator /(const ivec2 &inp)const
		{
			return ivec2(x / inp.x, y / inp.y);
		}

		ivec2 operator +(const int inp)const
		{
			return ivec2(inp + x, inp + y);
		}
		ivec2 operator -(const int inp)const
		{
			return ivec2(x - inp, y - inp);
		}
		ivec2 operator *(const int inp)const
		{
			return ivec2(inp * x, inp * y);
		}
		ivec2 operator /(const int inp)const
		{
			return ivec2(x / inp, y / inp);
		}
	};
	struct ivec3
	{
		union
		{
			struct
			{
				int x, y, z;
			};
			struct
			{
				ivec2 xy;
				int r;
			};
			struct
			{
				int s;
				ivec2 yz;
			};
		};
		ivec3() = default;
		ivec3(const int ix, const int iy, const int iz)
		{
			x = ix;
			y = iy;
			z = iz;
		}
		ivec3(const int n)
		{
			x = n;
			y = n;
			z = n;
		}
		ivec3(const ivec2 ixy, const int iz)
		{
			xy = ixy;
			z = iz;
		}
		ivec3(const int ix, const ivec2 iyz)
		{
			yz = iyz;
			x = ix;
		}
		ivec3(const ivec3 &ixyz)
		{
			x = ixyz.x;
			yz = ixyz.yz;
		}
		ivec3(const vec3 &ixyz)
		{
			x = int(ixyz.x);
			yz = ivec2(ixyz.yz);
		}
		ivec3 &operator = (const ivec3 ixyz)
		{
			x = ixyz.x;
			yz = ixyz.yz;
			return *this;
		}
		operator vec3&()//to vec3
		{
			return vec3(float(x), float(y), float(z));
		}
		explicit operator ivec2&()//ivec3 to ivec2
		{
			return xy;
		}

		ivec3 operator +(const ivec3 &inp)const
		{
			return ivec3(inp.x + x, inp.y + y, inp.z + z);
		}
		ivec3 operator -(const ivec3 &inp)const
		{
			return ivec3(x - inp.x, y - inp.y, z - inp.z);
		}
		ivec3 operator *(const ivec3 &inp)const
		{
			return ivec3(inp.x * x, inp.y * y, inp.z * z);
		}
		ivec3 operator /(const ivec3 &inp)const
		{
			return ivec3(x / inp.x, y / inp.y, z / inp.z);
		}

		ivec3 operator +(const int inp)const
		{
			return ivec3(inp + x, inp + y, inp + z);
		}
		ivec3 operator -(const int inp)const
		{
			return ivec3(x - inp, y - inp, z - inp);
		}
		ivec3 operator *(const int inp)const
		{
			return ivec3(inp * x, inp * y, inp * z);
		}
		ivec3 operator /(const int inp)const
		{
			return ivec3(x / inp, y / inp, z / inp);
		}
	};
	vec3 abs(const vec3 &inp)
	{
		return vec3(::abs(inp.x), ::abs(inp.y), ::abs(inp.z));
	}
	vec2 abs(const vec2 &inp)
	{
		return vec2(::abs(inp.x), ::abs(inp.y));
	}
	ivec3 abs(const ivec3 &inp)
	{
		return ivec3(::abs(inp.x), ::abs(inp.y), ::abs(inp.z));
	}
	ivec2 abs(const ivec2 &inp)
	{
		return ivec2(::abs(inp.x), ::abs(inp.y));
	}
	vec3 round(const vec3 &inp)
	{
		return vec3(::round(inp.x), ::round(inp.y),::round(inp.z));
	}
	vec2 round(const vec2 &inp)
	{
		return vec2(::round(inp.x), ::round(inp.y));
	}
	float max(float a, float b)
	{
		return a > b ? a : b;
	}
	float min(float a, float b)
	{
		return a < b ? a : b;
	}
	vec2 max(const vec2 &a, const vec2 &b)
	{
		return vec2(max(a.x, b.x), max(a.y, b.y));
	}
	vec2 min(const vec2 &a, const vec2 &b)
	{
		return vec2(min(a.x, b.x), min(a.y, b.y));
	}
	vec3 max(const vec3 &a, const vec3 &b)
	{
		return vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
	}
	vec3 min(const vec3 &a, const vec3 &b)
	{
		return vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
	}
	int max(int a, int b)
	{
		return a > b ? a : b;
	}
	int min(int a, int b)
	{
		return a < b ? a : b;
	}
	ivec2 max(const ivec2 &a, const ivec2 &b)
	{
		return ivec2(max(a.x, b.x), max(a.y, b.y));
	}
	ivec2 min(const ivec2 &a, const ivec2 &b)
	{
		return ivec2(min(a.x, b.x), min(a.y, b.y));
	}
	ivec3 max(const ivec3 &a, const ivec3 &b)
	{
		return ivec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
	}
	ivec3 min(const ivec3 &a, const ivec3 &b)
	{
		return ivec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
	}

	float dot(const vec3 &a, const vec3 &b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	float length(const vec3 a)
	{
		return sqrt(dot(a, a));
	}
};