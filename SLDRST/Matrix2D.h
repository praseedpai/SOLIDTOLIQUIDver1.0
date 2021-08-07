#ifndef _MATRIX_2D_DOT_H
#define _MATRIX_2D_DOT_H

class Matrix2D 
{
private :
	double m11,m12,m13;
	double m21,m22,m23;
	double m31,m32,m33;

	///////////////////////////////
	//
	//
	//
	void SetZero()
	{

       m11=m12=m13=0.0;
       m21=m22=m23=0.0;
	   m31=m32=m33=0.0;
	}


public:

	////////////////////
	//
	//
	//

	Matrix2D()
	{
       SetZero();
	}
    ///////////////////////////////
	//
	// 
	//
	//
    void SetIdentity()
	{
        SetZero();
        m11=1.0;
		m22=1.0;
		m33=1.0;
	}

	////////////////////////////////////////////
	//
	//
	//
	void PreMultiply(const Matrix2D& m )
	{
        double c11,c12,c13;
		double c21,c22,c23;
		double c31,c32,c33;

		////////////////////////
		// Top Row
		//
		//
        c11 = m.m11*m11 + m.m12*m21+m.m13*m31;
		c12 = m.m11*m12 + m.m12*m22+m.m13*m32;
		c13 = m.m11*m13 + m.m12*m23+m.m13*m33;

		///////////////////////////////
		//
		// 2nd row
		//
        c21 = m.m21*m11 + m.m22*m21+m.m23*m31;
		c22 = m.m21*m12 + m.m22*m22+m.m23*m32;
		c23 = m.m21*m13 + m.m22*m23+m.m23*m33;

		//////////////////////////////////////////
		//
		// 3rd row
        //
        c31 = m.m31*m11 + m.m32*m21+m.m33*m31;
		c32 = m.m31*m12 + m.m32*m22+m.m33*m32;
		c33 = m.m31*m13 + m.m32*m23+m.m33*m33;

		m11=c11;
		m12=c12;
		m13=c13;
		m21=c21;
		m22=c22;
		m23=c23;
		m31=c31;
		m32=c32;
		m33=c33;


	}

	//////////////////////////////////////
	//
	//
	//
	void mul( const Matrix2D& m )
	{
		double c11,c12,c13;
		double c21,c22,c23;
		double c31,c32,c33;

		////////////////////////
		// Top Row
		//
		//
        c11 = m11*m.m11 + m12*m.m21+m13*m.m31;
		c12 = m11*m.m12 + m12*m.m22+m13*m.m32;
		c13 = m11*m.m13 + m12*m.m23+m13*m.m33;

		///////////////////////////////
		//
		// 2nd row
		//
        c21 = m21*m.m11 + m22*m.m21+m23*m.m31;
		c22 = m21*m.m12 + m22*m.m22+m23*m.m32;
		c23 = m21*m.m13 + m22*m.m23+m23*m.m33;

		//////////////////////////////////////////
		//
		// 3rd row
        //
        c31 = m31*m.m11 + m32*m.m21+m33*m.m31;
		c32 = m31*m.m12 + m32*m.m22+m33*m.m32;
		c33 = m31*m.m13 + m32*m.m23+m33*m.m33;

		m11=c11;
		m12=c12;
		m13=c13;
		m21=c21;
		m22=c22;
		m23=c23;
		m31=c31;
		m32=c32;
		m33=c33;



	}

	//////////////////////////////////////////////
	//
	//
	//
	void Shear( double k )
	{
        Matrix2D *mat = new Matrix2D();
		mat->SetIdentity ();
#if 0
		mat->m12=k;
		mul(*mat);
#else
        mat->m21=k;
		PreMultiply (*mat);
#endif

		delete mat;


	}

	//////////////////////////////////////////
	//
	// 
	//
	//
    void Scale( double xscale , double yscale )
	{
        Matrix2D *mat = new Matrix2D();
		mat->SetIdentity ();
		mat->m11=xscale;
		mat->m22=yscale;
		mul(*mat);
		delete mat;




	}
    ////////////////////////////////////////////////
	//
	//
	//
	void Rotate( double rot )
	{
		double angle = rot*3.14159/180.0;
		Matrix2D *mat = new Matrix2D();
		mat->SetIdentity ();

#if 0
		mat->m11=cos(angle);
		mat->m12=-sin(angle);
		mat->m21=sin(angle);
		mat->m22=cos(angle);
		mul(*mat);
#else
        mat->m11=cos(angle);
		mat->m12=sin(angle);
		mat->m21=-sin(angle);
		mat->m22=cos(angle);
		PreMultiply (*mat);

#endif

		
		delete mat;
          

	}

	//////////////////////////////////////////////////
	//
	//
	//
	//
    void Translate( double xtrans , double ytrans )
	{
        Matrix2D *mat = new Matrix2D();
		mat->SetIdentity ();

#if 0
		mat->m31=xtrans;
		mat->m32=ytrans;
		mul(*mat);
#else

        mat->m13=xtrans;
		mat->m23=ytrans;
		PreMultiply(*mat);
#endif

		delete mat;

	}
	////////////////////////////////////////////////
	//
	//
	//
	//
	void Transform( double& x , double& y )
	{
      double t_x = x;
	  double t_y = y;

#if 0

	  x = t_x*m11 + t_y*m21 + m31;
	  y = t_x*m12 + t_y*m22 + m32;
#else
      x = t_x*m11 + t_y*m12 + m13;
	  y = t_x*m12 + t_y*m22 + m23;
#endif



	}

};

#endif
