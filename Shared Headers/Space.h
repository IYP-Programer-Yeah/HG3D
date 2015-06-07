#ifndef _Space_h
#define _Space_h
/*#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <allocators>
#include <process.h>*/
#include <math.h>
#define pi 3.1415926535897932384626433832795
bool nobet(int f,int s,int no)
{
if (f>s)
{
if (no>s&&no<f)
return 1;
return 0;
}
else if (s>f)
{
if (no>f&&no<s)
return 1;
return 0;
}
else if(no==f||no==s)
return 1;
return 0;
}
class point
{
public:
long double fx,fy,fz,px,py;
long double x,y,z;
long double nx,ny,nz;
long double cx,cy;
void __fastcall build(long double ix,long double iy,long double iz)
{
x=ix;
y=iy;
z=iz;
cx=0;
cy=0;
}
void __fastcall move(long double ax=0,long double ay=0,long double az=0)
{
x=x+ax;
y=y+ay;
z=z+az;
}
void __fastcall rotate(point from,long double ax=0,long double ay=0,long double az=0)
{
long double dx,dy,dz;
dx=x-from.x;
dy=y-from.y;
dz=z-from.z;
x=from.x+(dx*(cos(ay)*cos(az))+dy*(-1*cos(ax)*sin(az)+sin(ax)*sin(ay)*cos(az))+dz*(sin(ax)*sin(az)+cos(ax)*sin(ay)*cos(az)));
y=from.y+(dx*(cos(ay)*sin(az))+dy*(cos(ax)*cos(az)+sin(ax)*sin(ay)*sin(az))+dz*(-1*sin(ax)*cos(az)+cos(ax)*sin(ay)*sin(az)));
z=from.z+(dx*-1*sin(ay)+dy*sin(ax)*cos(ay)+dz*cos(ax)*cos(ay));
}
void __fastcall operator =(point intery)
{
build (intery.x,intery.y,intery.z);
}
};
class _3x3matrix
{
public:
long double x[3][3];
 _3x3matrix ()
{
#pragma omp paralell for
for (register int i=0;i<9;i++)
x[i/3][i%3]=0;
}
_3x3matrix __fastcall operator * (_3x3matrix input)
{
_3x3matrix ret;
#pragma omp paralell for
for (register int i=0;i<9;i++)
for (register int j=0;j<3;j++)
ret.x[i/3][i%3]=ret.x[i/3][i%3]+x[i/3][j]*input.x[j][i%3];
return ret;
}
point __fastcall operator * (point input)
{
point ret;
ret.build(input.x*x[0][0]+input.y*x[0][1]+input.z*x[0][2],input.x*x[1][0]+input.y*x[1][1]+input.z*x[1][2],input.x*x[2][0]+input.y*x[2][1]+input.z*x[2][2]);
return ret;
}
void __fastcall operator = (_3x3matrix input)
{
#pragma omp paralell for
for (register int i=0;i<9;i++)
x[i/3][i%3]=input.x[i/3][i%3];
}
void __fastcall build(long double ax,long double ay,long double az)
{
#pragma omp paralell
{
x[0][0]=cos(ay)*cos(az);
x[0][1]=-1*cos(ax)*sin(az)+sin(ax)*sin(ay)*cos(az);
x[0][2]=sin(ax)*sin(az)+cos(ax)*sin(ay)*cos(az);
x[1][0]=cos(ay)*sin(az);
x[1][1]=cos(ax)*cos(az)+sin(ax)*sin(ay)*sin(az);
x[1][2]=-1*sin(ax)*cos(az)+cos(ax)*sin(ay)*sin(az);
x[2][0]=-1*sin(ay);
x[2][1]=sin(ax)*cos(ay);
x[2][2]=cos(ax)*cos(ay);
}
}
};
class _4x4matrix
{
public:
long double x[4][4];
_4x4matrix ()
{
#pragma omp paralell for
for (register int i=0;i<16;i++)
x[i/4][i%4]=0;
}
_4x4matrix __fastcall operator * (_4x4matrix input)
{
_4x4matrix ret;
#pragma omp paralell for
for (register int i=0;i<16;i++)
for (register int j=0;j<4;j++)
ret.x[i/4][i%4]=ret.x[i/4][i%4]+x[i/4][j]*input.x[j][i%4];
return ret;
}
point __fastcall operator * (point input)
{
point ret;
double w;
#pragma omp paralell for
for (register int i = 0; i < 4; i++)
{
ret.x += x[i][0] * input.x;
ret.y += x[i][1] * input.y;
ret.z += x[i][2] * input.z;
w += x[i][3];
}
ret.x /= w;
ret.y /= w;
ret.z /= w;
return ret;
}
void __fastcall operator = (_4x4matrix input)
{
#pragma omp paralell for
for (register int i=0;i<16;i++)
x[i/4][i%4]=input.x[i/4][i%4];
}
void __fastcall operator = (float input[16])
{
#pragma omp paralell for
for (register int i=0;i<16;i++)
x[i/4][i%4]=(long double)input[i];
}
};
class vector
{
public:
long double x,y,z;
void __fastcall build(point p2,point p1)
{
x=p1.x-p2.x;
y=p1.y-p2.y;
z=p1.z-p2.z;
}
void __fastcall build(long double ix,long double iy,long double iz)
{
x=ix;
y=iy;
z=iz;
}
void __fastcall operator =(vector intery)
{
build(intery.x,intery.y,intery.z);
}
vector __fastcall operator +(vector intery)
{
vector ret;
ret.build(intery.x+x,intery.y+y,intery.z+z);
return ret;
}
vector __fastcall operator *(long double intery)
{
vector ret;
ret.build(intery*x,intery*y,intery*z);
return ret;
}
vector __fastcall operator /(long double intery)
{
vector ret;
ret.build(x/intery,y/intery,z/intery);
return ret;
}
long double __fastcall getsize()
{
return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}
};
long double __fastcall inner(vector v1,vector v2)
{
return (v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);
}
vector __fastcall outer(vector v1,vector v2)
{
vector temple;
temple.build(v1.y*v2.z-v2.y*v1.z,v1.z*v2.x-v2.z*v1.x,v1.x*v2.y-v2.x*v1.y);
return temple;
}
int point_regaring_to_frustum(_4x4matrix projection, point ipoint)
{
point projected = projection*ipoint;
if (nobet(1, -1, projected.x) && nobet(1, -1, projected.y))
{
if(nobet(1, -1, projected.z))
return 1;
if (-1> projected.z)
return 0;
if (projected.z>1)
return 2;
}
else
{
if(nobet(1, -1, projected.z))
return 4;
if (-1> projected.z)
return 3;
if (projected.z>1)
return 5;
}
}
#endif
