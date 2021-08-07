#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "CTransformDrawSurface.h"
#include "ShapeParser.h"
int TestRender(char *Filename) {

	CTransformDrawSurface ds(400,400);
	COLOR clr;
	clr.a = 255;clr.b = 0; clr.g = 0;clr.r = 0;
	ds.Clear(&clr);
	clr.a = 255;clr.b=255; clr.g=0; clr.r=255;
	ds.Line(10,10,100,100,&clr);
                   ds.Circle ( 100,100,100,&clr);
	ds.FilledCircle ( 100,100,100,&clr);
	clr.a = 255;clr.b=255; clr.g=255; clr.r=255;
	ds.FilledRectangle( 100,100,200,200,&clr); 
	clr.a = 255; clr.b=255; clr.g=0; clr.r=0;
	ds.FillTriangle ( 0 , 0 , 200,200 , 250,350,&clr);
	ds.FillEllipse (100,100,75,75,&clr);
	clr.a = 255; clr.b=0; clr.g=255; clr.r=255;
                   ds.DrawArc(100,100,100,0,270,&clr); 
	ds.FillArc(100,100,100,20,189,&clr);
                   ds.SaveToBmp(Filename);
	return 1;
}

int main( int argc, char **argv )
{

	char *filename = (argc > 1 ) ? argv[1] : "test.shp";
	list<TGraphicsNode *>& ls = LoadVectorsFromFile(filename);
                  CTransformDrawSurface ds(400,400);
                    list <TGraphicsNode *> :: iterator it;
                    for(it = ls.begin(); it != ls.end(); ++it)
                    {
                                       (*it)->Render(ds);
                     }
                    ds.SaveToBmp("test.bmp");

}