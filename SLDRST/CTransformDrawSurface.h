#ifndef _C_TRANSFORM_DRAW_SURFACE
#define _C_TRANSFORM_DRAW_SURFACE
#include "CDrawSurface.h"
#include "Matrix2D.h"


class CTransformDrawSurface : public CDrawSurface
{

private:
    Matrix2D tr_mat;



public:
	///////////////////////////////////////////
	//
	// Constructor
	//
	//
	CTransformDrawSurface( int x , int y ) : CDrawSurface(x,y)
	{
       tr_mat.SetIdentity ();

	}

	/////////////////////////////////////////////////
	//
	//
	//
	//
	//
	virtual ~CTransformDrawSurface()
	{


	}

    /////////////////////////////////////////
	//
	//
	//
	//
	void ResetTransform()
	{
        tr_mat.SetIdentity ();


	}

   


    //////////////////////////////////////////
	//
	// 
	//
	//
    void Scale( double xscale , double yscale )
	{
       tr_mat.Scale (xscale,yscale);

	}
    ////////////////////////////////////////////////
	//
	//
	//
	void Rotate( double rot )
	{
		tr_mat.Rotate( rot);
          

	}

	//////////////////////////////////////////////////
	//
	//
	//
	//
    void Translate( double xtrans , double ytrans )
	{
       tr_mat.Translate(xtrans,ytrans);

	}
	////////////////////////////////////////////////
	//
	//
	//
	//
	void Transform( double& x , double& y )
	{
        tr_mat.Transform ( x,y);

	}

	/////////////////////////////////////////////
	//
	//
	//
	void Shear( double k )
	{
         tr_mat.Shear ( k );

	}


};


#endif
