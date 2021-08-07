#ifndef _VECTOR_3D_DOT_H
#define _VECTOR_3D_DOT_H
#include <math.h>


class Vector3D
{
private:
	float x,y,z,w;

public:
	///////////////////////////////////////////
	//
	//  empty CTOR
	//
	//
	Vector3D() {
       x=y=z=w=0;
	}
	///////////////////////////////////////
	//
	//  CTOR with x,y,z
	//
	Vector3D(float px, float py, float pz,float pw=0)
	{
         x=px;
		 y=py;
		 z=pz;
		 w=pw;
    }
	////////////////////////////////
	//
	//  Copy ctor
	//
	//
    Vector3D( const Vector3D& pv )
	{
        x = pv.x;
		y = pv.y;
		z = pv.z;
		w = pv.w;

	}
	//////////////////////////////////////////
	//
	// Assignment operator
	//
	//
	//
	Vector3D& operator = (const Vector3D& pv )
	{
          x = pv.x;
		  y = pv.y;
		  z = pv.z;
		  w = pv.w;
		  return *this;
     }
	///////////////////////////////////////////
	//
	//  Set value to the object
	//
	void Set( float px = 0.0f , float py = 0.0f , float pz = 0.0f ,float pw=0.0f)
	{
         x = px;
		 y = py;
		 z = pz;
		 w = pw;
	}
	//////////////////////////////////////////
	//
	//  Set the parameters from Vector
	//
	//
	void Set( const Vector3D& pv )
	{
       x = pv.x;
	   y = pv.y;
	   z = pv.z;
	   w = pv.w;


	}
	/////////////////////////////////////
	// find the length of the vector
	//  
	//
	//
    float Length()
	{
       return( (float)sqrt(x*x + y*y + z*z) );
	}
	//////////////////////////////////////////
	//
	//  Normalize the vector
	//
	//
	//
    void Normalize()
	{
      float ln = Length();
	  x /= ln;
	  y /= ln;
	  z /= ln;
	}
	/////////////////////////////////////
	//
	// Dot Product
	//
	//
    float Dot(const Vector3D& pv)
	{

       return ( x*pv.x + y*pv.y + z*pv.z );
	}
	////////////////////////////////////
	//
	// 
	// Cross product
	//
    Vector3D *Cross( const Vector3D& pv )
	{
       Vector3D *retval = new Vector3D();
	   retval->x = y*pv.z - z*pv.y;
	   retval->y = z*pv.x - x*pv.z;
	   retval->z = x*pv.y - y*pv.x;
	   return retval;
	}

	///////////////////////////////////////
	//
	//
	//
	//
	//
	bool operator == (const Vector3D& pv )
	{
		return ( x == pv.x && y == pv.y && pv.z == z )?true:false;

	}
	/////////////////////////////////////
	//
	//
	//
    void Neg()
	{
        x = -x;
		y = -y;
		z = -z;
    }
	/////////////////////////////////
	//
	//
	//
    void Scale( float s )
	{
        x = x*s;
		y = y*s;
		z = z*s;
	}
	///////////////////////////////////////////
	//
	//
	//
    void ScalarDiv(float s)
	{
         Scale( 1/s);
    }
	/////////////////////////////////////////
	//
	//
	//
	//
	void Add( const Vector3D& pv )
	{
          x = x + pv.x;
		  y = y + pv.y;
		  z = z + pv.z;
    }
	////////////////////////////////
	//
	//
	//
    void Sub( const Vector3D& pv )
	{
        x = x - pv.x;
		y = y - pv.y;
		z = z - pv.z;
        
	}




};



#endif
