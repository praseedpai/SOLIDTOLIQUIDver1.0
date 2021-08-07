#ifndef _C_DRAW_SURFACE_DOT_H
#define _C_DRAW_SURFACE_DOT_H
/////////////////////////////////////////////
//
//
//  Mathematical Ciel
//
//
#define ROUND(a) ((int)((a)+0.5))

///////////////////////////////////////////
//
// COLOR data structure
//    Red , Green , Blue and Alpha ( 255 opaque , 0 - transparent )
//
typedef struct
{
  BYTE r;
  BYTE g;
  BYTE b;
  BYTE a;
}COLOR;
////////////////////////////////////////////
//
//  Types of Polygons
//
#define CONVEX 0
#define NONCONVEX 1
#define COMPLEX   2

///////////////////////////////////////////////
//
// Point data structure
//
//

struct POINT2D
{
  int X;
  int Y;

};

///////////////////////////////////////
//
//
//
struct POLYGON2D
{
    int Length;
	struct POINT2D *Points;
};

////////////////////////////////////////////
//
//
//
//
//
struct HLINE
{
  int Xstart;
  int Ystart;
};

///////////////////////////////////////////
//
//
//
//
struct HLINELIST 
{
  int Length;
  int Ystart;
  HLINE *HLinePtr;

};

//////////////////////////////////////////////
//
//
//
//
struct EDGESTATE
{
  int X;  // X co-ordinate of Edge start
  int StartY; // The scan line where this edge start
  int WholePixelXMove;
  int XDir;
  int ErrorTerm;
  int ErrorTermAdjUp;
  int ErrorTermAdjDown;
  int Count; // # of scan lines
  struct EDGESTATE *Next;
};

////////////////////////////////
//
//
//


class CDrawSurface
{

public:
   int m_x;
   int m_y;
   int m_size;
   bool m_init_val;
   UINT *pixels;
   int m_curr_x;
   int m_curr_y;

   ///////////////////////////////////////
   //
   // Polygon Filler code related data structures
   //
   //
   EDGESTATE *GETPtr;  //Global Get List
   EDGESTATE *AETPtr;  //Active Edge Table




private:

	////////////////////////////////////////////////////
	//
	// Create the Global Edge Table for 
	//
	//
	
	void BuildGET(POLYGON2D *Poly,EDGESTATE * EdgeTable)
	{
      ////////////////////////////////////
	  //  local variables
	  //
      int StartX,StartY,EndX,EndY;
	  int DeltaX,DeltaY;
	  int Width;
      EDGESTATE *NewEdgePtr,*FollowingEdge,**FollowingEdgeLink;
      POINT2D *VertexPtr = Poly->Points ;
	  GETPtr = 0;     

	  for ( int i=0; i< Poly->Length ; ++i )
	  {
          
         StartX = VertexPtr[i].X ;
		 StartY = VertexPtr[i].Y ;

		 if ( i == 0 )
		 {
           EndX = VertexPtr[ Poly->Length - 1 ].X ;
		   EndY = VertexPtr[ Poly->Length - 1 ].Y ; 

		 }
		 else {
           EndX = VertexPtr[ i-1 ].X ;
		   EndY = VertexPtr[ i-1].Y ; 
            
		 }

		 if ( StartY > EndY ) {
             Swap(StartX,EndX );
			 Swap(StartY,EndY );
		 }
        
		  if ((DeltaY = EndY - StartY ) != 0 )
		  {
              NewEdgePtr = EdgeTable++;
			  NewEdgePtr->XDir = ((DeltaX = EndX - StartX ) > 0 ) ? 1 : -1;
			  Width = abs(DeltaX);
			  NewEdgePtr->X = StartX;
			  NewEdgePtr->StartY = StartY;
			  NewEdgePtr->Count = DeltaY;
              NewEdgePtr->ErrorTermAdjDown = DeltaY;
			  if ( DeltaX >= 0 ) 
			  {
				  NewEdgePtr->ErrorTerm = 0;
			  }
			  else {

                  NewEdgePtr->ErrorTerm = -DeltaY + 1;
			  }

              
              if ( DeltaY >= Width )
			  {
                 NewEdgePtr->WholePixelXMove = 0;
				 NewEdgePtr->ErrorTermAdjUp = Width;
              }
			  else {
                 NewEdgePtr->WholePixelXMove = ( Width/DeltaY )*NewEdgePtr->XDir ;
				 NewEdgePtr->ErrorTermAdjUp = Width%DeltaY;

			  }


			  FollowingEdgeLink = &GETPtr;

			  for(;;) 
			  {
                 FollowingEdge = *FollowingEdgeLink;

				 if ( ( FollowingEdge == 0 ) ||
					  ( FollowingEdge->StartY > StartY ) ||
					  (( FollowingEdge->StartY == StartY) &&
					  ( FollowingEdge->X >= StartX ))) 
				 {
                       NewEdgePtr->Next = FollowingEdge;
					   *FollowingEdgeLink = NewEdgePtr;
                       break;

				 }
                     FollowingEdgeLink = &FollowingEdge->Next ;

			  }


		  }



	  }




	}
    ////////////////////////////////////////
	//
	// Sort Active Edge table based on X co-ordinate
	//
	//
	//
	void  XSortAET()
	{
	      EDGESTATE *CurrentEdge , **CurrentEdgePtr , *TempEdge;	  
		  int SwapTrue = 0;
		  //////////////////////////////
		  //
		  //  if Active Edge table is zero
		  //
          if ( AETPtr == 0 )
		       return;


      
		  do {

            SwapTrue = 0;

			CurrentEdgePtr = &AETPtr;

			while ( ( CurrentEdge = *CurrentEdgePtr )->Next != 0 )
			{
                if ( CurrentEdge->X > CurrentEdge->Next->X )
				{
                   TempEdge = CurrentEdge->Next->Next;
				   *CurrentEdgePtr = CurrentEdge->Next ;
                   CurrentEdge->Next->Next = CurrentEdge;
				   CurrentEdge->Next = TempEdge;
                   SwapTrue = 1;


				}
                CurrentEdgePtr = &(*CurrentEdgePtr)->Next ;


			}



		  }
          while ( SwapTrue != 0 );


		  
  


	}

    /////////////////////////////////////////////////
	//
	//
	//
	//
	//
	void ScanOutAET(int CurrentY, COLOR *col)
	{
         int LeftX; 
		 EDGESTATE *CurrentEdge = AETPtr;

		  while ( CurrentEdge != 0 )
		  {
            LeftX = CurrentEdge->X;
			CurrentEdge = CurrentEdge->Next ;
			DrawHorizontalLine(CurrentY,LeftX,CurrentEdge->X-1,col); 
			CurrentEdge = CurrentEdge->Next ;

		  }




	}

	/////////////////////////////////////////////////////
	//
	// Advance AET 
	//
	//
	//
	void AdvanceAET() {
       EDGESTATE *CurrentEdge , **CurrentEdgePtr;

	   CurrentEdgePtr = &AETPtr;

	   while (( CurrentEdge = *CurrentEdgePtr ) != 0 )
	   {
		   if ( (--(CurrentEdge->Count ) ) == 0 ) {

                 *CurrentEdgePtr = CurrentEdge->Next ;


		   }else {

              CurrentEdge->X+=CurrentEdge->WholePixelXMove ;

			  CurrentEdge->ErrorTerm += CurrentEdge->ErrorTermAdjUp ;

			  if ( CurrentEdge->ErrorTerm > 0 )
			  {
                  CurrentEdge->X += CurrentEdge->XDir ;
				  CurrentEdge->ErrorTerm  -= CurrentEdge->ErrorTermAdjDown ;



			  }

              CurrentEdgePtr = &CurrentEdge->Next ;

		   }


	   }


	}

	/////////////////////////////////////////////////////////////
	//
	//  Move X sorted values from GET to AET
	//
	//
	//
    void MoveXSortedToAET(int CurrentY)
	{
       EDGESTATE *AETEdge , **AETEdgePtr , *TempEdge;

	   int CurrentX;

	   AETEdgePtr = &AETPtr;

	   while ( ( GETPtr != 0 ) && ( GETPtr->StartY == CurrentY ) )
	   {
          CurrentX = GETPtr->X;

		  for(;;) {
               AETEdge = *AETEdgePtr;

			   if ( (AETEdge == 0 ) || ( AETEdge->X >= CurrentX ))
			   {
                  TempEdge = GETPtr->Next ;
				  *AETEdgePtr = GETPtr;
                  GETPtr->Next = AETEdge;
				  AETEdgePtr = &GETPtr->Next ;
				  GETPtr = TempEdge;
                  break;



			   }
			   else {
                   AETEdgePtr = &AETEdge->Next ;

			   }


		  }



	   }



	}

	/////////////////////////////////////////
	//
	//  Swap two points
	//
	//

	void Swap( int& x , int&y )
	{
       int Temp = y;
	   y=x;
	   x=Temp;
     }

	/////////////////////////////////////////////
	//
	//
	//
	//
	void CirclePoints( int cx,int cy,int x, int y, COLOR *col )
	{
      Line(cx-x,cy-y,cx+x,cy-y,col);
      Line(cx-y,cy+x,cx+y,cy+x,col);
	  Line(cx-x,cy+y,cx+x,cy+y,col);
      Line(cx-y,cy-x,cx+y,cy-x,col);
	}

	////////////////////////////////////////////
	//
	//
	//
	//
	void EllipsePoints(int xc, int yc, int x , int y , COLOR *col)
	{
        PutPixel(xc+x , yc + y , col );
		PutPixel(xc-x , yc + y , col );
		PutPixel(xc+x , yc - y , col );
		PutPixel(xc-x , yc - y , col );



	}


	////////////////////////////////////////////
	//
	//
	//
	//
	void EllipsePoints2(int xc, int yc, int x , int y , COLOR *col)
	{
        Line(xc-x , yc + y ,xc+x,yc+y, col );
		Line(xc+x , yc - y ,xc-x,yc-y, col );
		



	}

	/////////////////////////////////////////////
	//
	//
	//
	//
	void PlotArc1( int x , int y , int sector , COLOR *col , int arcTest[],
		int x_start_test , int x_end_test )
	{
         if ( arcTest[sector] == 0 )
			      return;

		 if (arcTest[sector] == 2 )
			     PutPixel(x,y,col);

		 if ((arcTest[sector] == 1) && (x >= x_start_test ))
		 {
               PutPixel(x,y,col);

		 }

		 if ( (arcTest[sector] == 3) && ( x <= x_end_test ))
			    PutPixel(x,y,col);

		 if ((arcTest[sector]==4) && ( x>=x_start_test ) && ( x<=x_end_test ) )
			    PutPixel(x,y,col);



	}


	/////////////////////////////////////////////
	//
	//
	//
	//
	void PlotArc2( int x , int y , int sector , COLOR *col , int arcTest[],
		int x_start_test , int x_end_test )
	{
         if ( arcTest[sector] == 0 )
			      return;

		 if (arcTest[sector] == 2 )
			     PutPixel(x,y,col);

		 if ((arcTest[sector] == 1) && (x <= x_start_test ))
		 {
               PutPixel(x,y,col);

		 }

		 if ( (arcTest[sector] == 3) && ( x >= x_end_test ))
			    PutPixel(x,y,col);

		 if ((arcTest[sector]==4) && ( x<=x_start_test ) && ( x>=x_end_test ) )
			    PutPixel(x,y,col);



	}


	/////////////////////////////////////////////
	//
	//
	//
	//
	void FillArc1(int xc, int yc, int x , int y , int sector , COLOR *col , int arcTest[],
		int x_start_test , int x_end_test )
	{
         if ( arcTest[sector] == 0 )
			      return;

		 if (arcTest[sector] == 2 )
			     Line2(xc,yc,x,y,col,3);

		 if ((arcTest[sector] == 1) && (x >= x_start_test ))
		 {
               Line2(xc,yc,x,y,col,3);

		 }

		 if ( (arcTest[sector] == 3) && ( x <= x_end_test ))
			    Line2(xc,yc,x,y,col,3);


		 if ((arcTest[sector]==4) && ( x>=x_start_test ) && ( x<=x_end_test ) )
			    Line2(xc,yc,x,y,col,3);




	}


	/////////////////////////////////////////////
	//
	//
	//
	//
	void FillArc2(int xc,int yc, int x , int y , int sector , COLOR *col , int arcTest[],
		int x_start_test , int x_end_test )
	{
         if ( arcTest[sector] == 0 )
			      return;

		 if (arcTest[sector] == 2 )
			     Line2(xc,yc,x,y,col,3);

		 if ((arcTest[sector] == 1) && (x <= x_start_test ))
		 {
               Line2(xc,yc,x,y,col,3);


		 }

		 if ( (arcTest[sector] == 3) && ( x >= x_end_test ))
			    Line2(xc,yc,x,y,col,3);


		 if ((arcTest[sector]==4) && ( x<=x_start_test ) && ( x>=x_end_test ) )
			     Line2(xc,yc,x,y,col,3);



	}

	///////////////////////////////////////////
	//
	//
	// Draw Horizontal Line Seg
	//
	//
	//
    void DrawHorizontalLine( int Y , int LeftX , int RightX , COLOR *col )
	{
       for( int X=LeftX; X<=RightX; X++ )
		    PutPixel(X,Y,col);


	}

	/////////////////////////////////////////////////////
	//
	//
	//
	//


public:
   CDrawSurface( int x , int y )
   {
        m_x = x;
		m_y = y;
		m_size = m_x*m_y;
        pixels = new UINT[ m_size ];
		m_init_val = true;
   }

   ////////////////////////////////////////////
   //
   // Virtual destructor
   //
   //
   //
   virtual ~CDrawSurface( )
   {


   }
   //////////////////////////////////////
   //
   //  Clear with a 32 bit color
   //
   //
   //
   void Clear( COLOR *col )
   {
	   UINT Value = ( col->a << 24 | col->r << 16 | col->g << 8 | col->b);
       UINT *p_pixels = pixels;
	   int i=0;
	   while ( i < m_size )
	   {
          *p_pixels++ = Value;
		  i++;

	   }
   }
   //////////////////////////////////////////
   //
   //
   //
   //
   void BlendColor( COLOR *col )
   {
        int i=0;
		UINT *p_pixels = pixels; 
		float piz = (float) ((float)col->a/255);

		while ( i < m_size )
		{
          UINT Pixel = *p_pixels;
          UINT alpha = ( Pixel >> 24 )&0xFF;
		  UINT r     = ( Pixel >> 16 )&0xFF;
		  UINT g     = ( Pixel >> 8  )&0xFF;
		  UINT b     = ( Pixel&0xFF );
		  
		  r     =  (UINT)( col->r*piz +  r  - piz*r );
		  g     =  (UINT)( col->g*piz +  g  - piz*g );
		  b     =  (UINT)( col->b*piz +  b  - piz*b );
		  char *rs =(char *)((void *) p_pixels);
		  *rs++=b;
		  *rs++=g;
		  *rs++=r;
		  *rs++=0xFF;
		  p_pixels++;
		  i++;

		}

   }

   ///////////////////////////////////////////
   //
   //
   //
   //
   inline void PutPixel( int x , int y , COLOR *col )
   {
              
	          if ( ( x <  0 || x > m_x ) ||
                   ( y < 0  || y > m_y ) )
				      return;

	   
	         // int offset =x*m_x*4+ y*4 ;
			  int offset = y*m_y*4 + x*4;

			  int  *p_pixels = (int *) ((char *)pixels + offset);
			  UINT Pixel = *p_pixels;
              UINT alpha = ( Pixel >> 24 )&0xFF;
		      UINT r     = ( Pixel >> 16 )&0xFF;
		      UINT g     = ( Pixel >> 8  )&0xFF;
		      UINT b     = ( Pixel&0xFF );
			  float piz = (float)1/col->a;
		  
		      r     =  col->r; //,(UINT)( col->r*piz +  r  - piz*r );
		      g     =  col->g; //(UINT)( col->g*piz +  g  - piz*g );
		      b     =  col->b; // (UINT)( col->b*piz +  b  - piz*b );
		      char *rs =(char *)((void *) p_pixels);
		     *rs++=b;
		     *rs++=g;
		     *rs++=r;
		     *rs++=0xFF;


   }
   /////////////////////////////////////////
   //
   //
   //
   //
   //
   void Line( int x1 , int y1 , int x2 , int y2,COLOR *col )
   {
        if ( x2 < x1 )
		{
           Swap(x2,x1);
		   Swap(y2,y1);

		}
	   
	   
	    int deltax = x2-x1;
		int deltay = y2-y1;
		int xinc = 1 , yinc =1;
		int x = x1,y=y1;


		if ( deltax < 0 ) { xinc = -1; deltax = -deltax; }
		if ( deltay < 0 ) { yinc = -1; deltay = -deltay; }

		if ( deltay <= deltax )
		{
            int c = 2*deltax,m = 2*deltay;
			int D=0;
			while (1)
			{
               PutPixel( x , y,col);   
			   if ( x == x2 ) 
                     break; 

			   x += xinc;
			   D += m;

			   if ( D > deltax ) 
			   {
                 y+=yinc; D-=c;

			   }
			}


		}
		else {

           int c = 2*deltay,m = 2*deltax;
			int D=0;
			while (1)
			{
               PutPixel( x , y,col);   
			   if ( y == y2 ) 
                     break; 

			   y += yinc;
			   D += m;

			   if ( D > deltay ) 
			   {
                 x+=xinc; D-=c;

			   }
			}





		}
     


   }

   /////////////////////////////////////////
   //
   //
   //
   //
   //
   void Line2( int x1 , int y1 , int x2 , int y2,COLOR *col , int width)
   {
        if ( x2 < x1 )
		{
           Swap(x2,x1);
		   Swap(y2,y1);

		}
	   
	   
	    int deltax = x2-x1;
		int deltay = y2-y1;
		int xinc = 1 , yinc =1;
		int x = x1,y=y1;


		if ( deltax < 0 ) { xinc = -1; deltax = -deltax; }
		if ( deltay < 0 ) { yinc = -1; deltay = -deltay; }

		if ( deltay <= deltax )
		{
            int c = 2*deltax,m = 2*deltay;
			int D=0;
			while (1)
			{
              for(int j=-width/2; j<=width/2; ++j )
                   PutPixel( x , y+j,col);   
			   if ( x == x2 ) 
                     break; 

			   x += xinc;
			   D += m;

			   if ( D > deltax ) 
			   {
                 y+=yinc; D-=c;

			   }
			}


		}
		else {

           int c = 2*deltay,m = 2*deltax;
			int D=0;
			while (1)
			{
                for(int j=-width/2; j<=width/2; ++j )
                   PutPixel( x + j, y,col);   
			   if ( y == y2 ) 
                     break; 

			   y += yinc;
			   D += m;

			   if ( D > deltay ) 
			   {
                 x+=xinc; D-=c;

			   }
			}





		}
     


   }

   /////////////////////////////////////////
   //
   //
   //
   //
   void Circle( int x , int y , int r , COLOR *col )
   {
       int p_x = 0;
	   int p_y = r;
       int E=0;
	   int u=1;
	   int v=2*r-1;

	   while (p_x < p_y )
	   {
           PutPixel(x+p_x,y+p_y,col);
		   PutPixel(x+p_y,y-p_x,col);
		   PutPixel(x-p_x,y-p_y,col);
		   PutPixel(x-p_y,y+p_x,col);
		   p_x++; E+=u; u+=2;
		   if ( v < 2*E ) { p_y--;E-=v;v-=2;}

		   if ( p_x > p_y ) { break; }

		   PutPixel(x+p_y,y+p_x,col);
		   PutPixel(x+p_x,y-p_y,col);
		   PutPixel(x-p_y,y-p_x,col);
		   PutPixel(x-p_x,y+p_y,col);






	   }



   }
   //////////////////////////////////////////////
   //
   //
   //
   //
   void Rectangle( int x, int y, int x_1 , int y_1,COLOR *col )
   {
        if ( x > x_1 )
		{
           Swap(x,x_1);
    	}

		if ( y_1 < y )
		{
           Swap(y,y_1);
		}

        int d_x = x_1 - x;
		int d_y = y_1 - y;
		Line(x,y,x+d_x,y,col);
		Line(x,y,x,y+d_y,col);
		Line(x+d_x,y,x+d_x,y+d_y,col);
		Line(x,y+d_y,x+d_x,y+d_y,col);
		
    }

    ///////////////////////////////////////////////
    //
    //
    //
    //
    //
    void FilledRectangle( int x , int y, int x_1,int y_1,COLOR *col )
	{
        if ( x > x_1 )
		{
           Swap(x,x_1);
    	}

		if ( y_1 < y )
		{
           Swap(y,y_1);
		}

        int d_x = x_1 - x;
		int d_y = y_1 - y;

		int p_y = y;
        int p_x = x; 
		while ( p_y < y_1 )
		{
           Line(p_x,p_y,p_x+d_x,p_y,col);
		   p_y++;
		  

		}
		

	}

    /////////////////////////////////////////
   //
   //
   //
   //
   void FilledCircle( int x , int y , int r , COLOR *col )
   {
       int p_x = 0;
	   int p_y = r;
       int E=0;
	   int u=1;
	   int v=2*r-1;

	   while (p_x < p_y )
	   {
           CirclePoints(x,y,p_x,p_y,col);
		   
		   p_x++; E+=u; u+=2;
		   if ( v < 2*E ) { p_y--;E-=v;v-=2;}

		   if ( p_x > p_y ) { break; }

		  CirclePoints(x,y,p_x,p_y,col);


	   }



   }
   /////////////////////////////////////////
   //
   //
   //
   //
   void DrawTriangle( int x_1 , int y_1 , int x_2 , int y_2, int x_3 , int y_3,COLOR *col )
   {
         Line(x_1,y_1,x_2,y_2,col);
		 Line(x_2,y_2,x_3,y_3,col);
		 Line(x_3,y_3,x_1,y_1,col);


   }
   ///////////////////////////////////////////
   //
   //  Fill a Triangle with Solid Color
   //
   //  There are three types of triangles
   //      Flat Top
   //      Flat bottom
   //      other.
   //  Sort the vertices in increasing order - 0 - top 1 - bottom 2-middle 
   //
   void FillTriangle( int x_1,int y_1 , int x_2 , int y_2,int x_3,int y_3, COLOR *col )
   {
        if ( y_2 < y_1 )
		{
           Swap(y_1,y_2);  // Swap x and y
		   Swap(x_2,x_1);

		}

		if ( y_3 < y_1 )
		{
           Swap(y_3,y_1);
		   Swap(x_3,x_1);
		}

		if ( y_2 < y_3 )
		{
           Swap(y_3,y_2);
		   Swap(x_3,x_2);
		}

		float ledge = x_1;
		float redge = x_1; 

		float derive1 = (float)(x_3 - x_1)/(y_3 - y_1 );
		float derive2 = (float)(x_2 - x_1)/(y_2-y_1);

		float left_d ;
		float right_d;

		if ( derive1 < derive2 )
		{
            left_d = derive1;
			right_d = derive2;

		}
		else {
           left_d = derive2;
		   right_d = derive1;


		}

		/////////////////////////////
		//
		// Render the top of the triangle
		//
		//
		//
        for( int y= y_1; y < y_3; y++ )
		{

            for( int rs = ledge; rs <= redge; ++rs )
			{

               PutPixel(rs,y,col);


			}

            ledge += left_d;
			redge += right_d;

		}
        /////////////////////////////////////////////////////////
		//
		//
		//
		//
		//
        if ( derive1 < derive2 )
		{
            left_d = (float)(x_3 - x_2 ) / ( y_3 - y_2);
			

		}
		else {
           
		   right_d = (float)(x_3 - x_2 ) / (y_3 - y_2);


		}

        for( int y= y_3; y < y_2; y++ )
		{

            for( int rs = ledge; rs <= redge; ++rs )
			{

               PutPixel(rs,y,col);


			}

            ledge += left_d;
			redge += right_d;

		}


   }
   ///////////////////////////////////////////////////////////////////
   //
   //  Draw a ellipse
   //
   //
   //
   void Ellipse( int xc , int yc , int rx,int ry, COLOR *col )
   {

       int Rx2 = rx*rx;  // square of rx
	   int Ry2 = ry*ry;  // square of ry
	   int twoRx2 = 2*Rx2;
	   int twoRy2 = 2*Ry2;

	   int p;
	   int x=0;
	   int y=ry;
	   int px=0;
	   int py=twoRx2*y;

       EllipsePoints(xc,yc,x,y,col);

	   /********* Region #1 *****************/
	   p= ROUND(Ry2 - ( Rx2*ry ) + (0.25*Rx2));

	   while ( px < py )
	   {
            x++;
			px +=twoRy2;

			if ( p < 0 )
			{
				p+=Ry2+px;
			}
			else {
               y--;
			   py -=twoRx2;
			   p+=Ry2+px-py;
			}
            EllipsePoints(xc,yc,x,y,col);

		}

	    p= ROUND(Ry2*(x+0.5)*(x+0.5) + Rx2*(y-1)*(y-1) - Rx2*Ry2);

        while ( y > 0 )
		{
             y--;
			 py -=twoRx2;

			 if ( p > 0 )
			 {
                p+=Rx2-py;
			 }
			 else {
                x++;
				px+=twoRy2;
				p+=Rx2-py+px;
				}
              EllipsePoints(xc,yc,x,y,col);

			 }

                 


		


   }
///////////////////////////////////////////////////////////////////
   //
   //  Draw a ellipse
   //
   //
   //
   void FillEllipse( int xc , int yc , int rx,int ry, COLOR *col )
   {

       int Rx2 = rx*rx;  // square of rx
	   int Ry2 = ry*ry;  // square of ry
	   int twoRx2 = 2*Rx2;
	   int twoRy2 = 2*Ry2;

	   int p;
	   int x=0;
	   int y=ry;
	   int px=0;
	   int py=twoRx2*y;

        EllipsePoints2(xc,yc,x,y,col);

	   /********* Region #1 *****************/
	   p= ROUND(Ry2 - ( Rx2*ry ) + (0.25*Rx2));

	   while ( px < py )
	   {
            x++;
			px +=twoRy2;

			if ( p < 0 )
			{
				p+=Ry2+px;
			}
			else {
               y--;
			   py -=twoRx2;
			   p+=Ry2+px-py;
			}
            EllipsePoints2(xc,yc,x,y,col);

		}

	    p= ROUND(Ry2*(x+0.5)*(x+0.5) + Rx2*(y-1)*(y-1) - Rx2*Ry2);

        while ( y > 0 )
		{
             y--;
			 py -=twoRx2;

			 if ( p > 0 )
			 {
                p+=Rx2-py;
			 }
			 else {
                x++;
				px+=twoRy2;
				p+=Rx2-py+px;
				}
              EllipsePoints2(xc,yc,x,y,col);

			 }

                 


		


   }

   //////////////////////////////////////////
   //
   //
   //  Program to Draw an Arc
   //
   //
   //
   void DrawArc( int x , int y, int r , int sta , int enda,COLOR *col )
   {
       int p_x = 0;
	   int p_y = r;
       int E=0;
	   int u=1;
	   int v=2*r-1;
	   int x_start_test = ROUND(x + r*cos(sta*3.14159/180.0));
	   int x_end_test   = ROUND(x + r*cos(enda*3.14159/180.0));
	   int arcTest[] = { 0 , 0, 0, 0, 0, 0, 0, 0, 0 };

	   int start_sector = (int)(sta/45);
	   int end_sector   = (int)(enda/45);

	   if ( start_sector == end_sector )
	   {
          arcTest[start_sector] = 4; // starts and ends in same sector

	   }
	   else {
          arcTest[start_sector] = 1;
		  arcTest[end_sector]=3;

		  for( int j=start_sector+1; j !=end_sector; j ++ )
		  {
              arcTest[j]=2;
			  if ( j == 8 )
				  j=-1;

		  }



	   }
       //////////////////////////////////////////////
	   //
	   //
	   //
	   //
	   //


	   while (p_x < p_y )
	   {
           //PutPixel(x+p_x,y+p_y,col);
		   //PutPixel(x+p_y,y-p_x,col);
		   //PutPixel(x-p_x,y-p_y,col);
		   //PutPixel(x-p_y,y+p_x,col);
		   PlotArc2(x+p_x,y+p_y,1,col,arcTest,x_start_test,x_end_test);
           PlotArc1(x+p_y,y-p_x,7,col,arcTest,x_start_test,x_end_test);
		   PlotArc1(x-p_x,y-p_y,5,col,arcTest,x_start_test,x_end_test);
		   PlotArc2(x-p_y,y+p_x,3,col,arcTest,x_start_test,x_end_test);

		   p_x++; E+=u; u+=2;
		   if ( v < 2*E ) { p_y--;E-=v;v-=2;}

		   if ( p_x > p_y ) { break; }

		  // PutPixel(x+p_y,y+p_x,col);
		  // PutPixel(x+p_x,y-p_y,col);
		  // PutPixel(x-p_y,y-p_x,col);
		  // PutPixel(x-p_x,y+p_y,col);
          PlotArc2(x+p_y,y+p_x,0,col,arcTest,x_start_test,x_end_test);
		  PlotArc1(x+p_x,y-p_y,6,col,arcTest,x_start_test,x_end_test);
		  PlotArc1(x-p_y,y-p_x,4,col,arcTest,x_start_test,x_end_test);
		  PlotArc2(x-p_x,y+p_y,2,col,arcTest,x_start_test,x_end_test);





	   }



 


   }
  
    //////////////////////////////////////////
   //
   //
   //  Program to Draw an Arc
   //
   //
   //
   void FillArc( int x , int y, int r , int sta , int enda,COLOR *col )
   {
       int p_x = 0;
	   int p_y = r;
       int E=0;
	   int u=1;
	   int v=2*r-1;
	   sta +=90;
	   enda +=90;

	   if ( sta < 0 )
		    sta += 360;
	   if (sta > 360 )
		    sta -=360;

	   if (enda > 360 )
		    enda -=360;

	   if ( enda < 0 )
		    enda +=360;
	   int x_start_test = ROUND(x + r*cos(sta*3.14159/180.0));
	   int x_end_test   = ROUND(x + r*cos(enda*3.14159/180.0));
	   int arcTest[] = { 0 , 0, 0, 0, 0, 0, 0, 0, 0 };

	   int start_sector = (int)(sta/45);
	   int end_sector   = (int)(enda/45);

	   if ( start_sector == end_sector )
	   {
          arcTest[start_sector] = 4; // starts and ends in same sector

	   }
	   else {
          arcTest[start_sector] = 1;
		  arcTest[end_sector]=3;

		  for( int j=start_sector+1; j !=end_sector; j ++ )
		  {
              arcTest[j]=2;
			  if ( j == 8 )
				  j=-1;

		  }



	   }
       //////////////////////////////////////////////
	   //
	   //
	   //
	   //
	   //


	   while (p_x < p_y )
	   {
           //PutPixel(x+p_x,y+p_y,col);
		   //PutPixel(x+p_y,y-p_x,col);
		   //PutPixel(x-p_x,y-p_y,col);
		   //PutPixel(x-p_y,y+p_x,col);
		   FillArc2(x,y,x+p_x,y+p_y,1,col,arcTest,x_start_test,x_end_test);
           FillArc1(x,y,x+p_y,y-p_x,7,col,arcTest,x_start_test,x_end_test);
		   FillArc1(x,y,x-p_x,y-p_y,5,col,arcTest,x_start_test,x_end_test);
		   FillArc2(x,y,x-p_y,y+p_x,3,col,arcTest,x_start_test,x_end_test);

		   p_x++; E+=u; u+=2;
		   if ( v < 2*E ) { p_y--;E-=v;v-=2;}

		   if ( p_x > p_y ) { break; }

		  // PutPixel(x+p_y,y+p_x,col);
		  // PutPixel(x+p_x,y-p_y,col);
		  // PutPixel(x-p_y,y-p_x,col);
		  // PutPixel(x-p_x,y+p_y,col);
          FillArc2(x,y,x+p_y,y+p_x,0,col,arcTest,x_start_test,x_end_test);
		  FillArc1(x,y,x+p_x,y-p_y,6,col,arcTest,x_start_test,x_end_test);
		  FillArc1(x,y,x-p_y,y-p_x,4,col,arcTest,x_start_test,x_end_test);
		  FillArc2(x,y,x-p_x,y+p_y,2,col,arcTest,x_start_test,x_end_test);





	   }



 


   }
   /////////////////////////////////////////////////////////
   //
   //  fill the polygon structure with the color
   //
   //
   void FillPolygon( POLYGON2D *Poly , COLOR *col )
   {
       EDGESTATE *EdgeTable;  // Local variable
	   int CurrentY;

	   //
	   //  if sufficient # of vertexes are not there 
	   //  return;
	   if ( Poly->Length < 3 )
		     return;
       
       EdgeTable  = ( EDGESTATE *) malloc(sizeof(EDGESTATE)*Poly->Length );

	   if ( EdgeTable == 0 )
		     return;

	   // build the global edge table
       //
	   // GET List is sorted in the ascending Y order
	   // and inside Y , it is sorted in X order also
	   // 
	   BuildGET(Poly,EdgeTable);  

	   /////////////////////////////
	   //
	   // Set Active Edge Table to Empty
	   //
       AETPtr = 0;

	   ////////////////////////////////////
	   //
	   //  Take the first Scanline since it is Y sorted
	   //  first element will contain that 
	   //
	   CurrentY = GETPtr->StartY ;

	   //////////////////////////////////////////////
	   //
	   //  Process As long as GETPtr or AET is not empty 
	   //
	   //
	   while ( ( GETPtr  != 0 ) || (AETPtr != 0 ) )
	   {
          ////////////////////////////////////////
          // Copy the current scanline intersection to
          // AET without destroying the X sorting order of AET
          MoveXSortedToAET(CurrentY);
		  ///////////////////////////////////////
		  // 
		  // Scan convert the spans for the current line using color 
		  // using odd / even rule 
		  ScanOutAET(CurrentY,col);
		  ///////////////////////////////////////////
		  //
		  // Remove the edges which finishes on the current scan line 
		  // bump  each of the edges one scan line 
		  //
		  AdvanceAET();
		  /////////////////////////////////////////
		  // Restore the X sorting order of Scan line 
		  //
		  XSortAET();

		  ////////////////////////////////////
		  //
		  // Bump the current Scan line 
		  //
		  CurrentY++;


	   }
       free(EdgeTable); // free the memory
       return;  // Finished scan converting 

   }
   ////////////////////////////////////////////////
   //
   //  Render
   //
   //
   //
   int Render( HDC dc )
   {
	   BITMAPINFO bmi;
	   LPVOID pvBits;
       ZeroMemory(&bmi, sizeof(BITMAPINFO));

       bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
       bmi.bmiHeader.biWidth = m_x;
       bmi.bmiHeader.biHeight = -m_y;
       bmi.bmiHeader.biPlanes = 1;
       bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
       bmi.bmiHeader.biCompression = BI_RGB;
       bmi.bmiHeader.biSizeImage = m_x * m_y * 4;
       HBITMAP  hbitmap = CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
       memcpy(pvBits,pixels,m_x*m_y*4);

	   StretchDIBits(dc,
                         // destination rectangle
                         0, 0, m_x, m_y,
                         // source rectangle
                         0, 0, m_x,m_y,
                         pvBits,
                        &bmi,
                         DIB_RGB_COLORS,
                         SRCCOPY);

      return 1;

   }

   int SaveToBmp(char *Filename ) {

       BITMAPINFO bmi;
       BITMAPFILEHEADER bmfHdr;
       BITMAPINFOHEADER bi;
       LPVOID pvBits;
       ZeroMemory(&bmi, sizeof(BITMAPINFO)); 
       bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
       bmi.bmiHeader.biWidth = m_x;
       bmi.bmiHeader.biHeight = -m_y;
       bmi.bmiHeader.biPlanes = 1;
       bmi.bmiHeader.biBitCount = 32;         // four 8-bit components
       bmi.bmiHeader.biCompression = BI_RGB;
       bmi.bmiHeader.biSizeImage = m_x * m_y * 4;

         bmfHdr.bfType = 0x4D42; // "BM"
         DWORD dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_x*m_y*4;
          bmfHdr.bfSize = dwDIBSize;
          bmfHdr.bfReserved1 = 0;
          bmfHdr.bfReserved2 = 0;
          bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
                                             (DWORD)sizeof(BITMAPINFOHEADER);

         
         //-----Open a File
         HANDLE   fh = CreateFile(Filename, GENERIC_WRITE, 
                                    0, NULL, CREATE_ALWAYS,
                                     FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 
                                     NULL);

          if (fh == INVALID_HANDLE_VALUE) { return -1; }

          
          DWORD dwWritten = 0;
          WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
          WriteFile(fh, (LPSTR)&bmi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
          WriteFile(fh, (LPSTR)pixels, dwDIBSize, &dwWritten, NULL); 
          CloseHandle(fh);
          return 1;

   }





};


#endif