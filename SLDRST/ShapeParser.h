#ifndef _SHAPE_PARSER_DOT_H
#define _SHAPE_PARSER_DOT_H
#include <stdio.h>
#include <string.h>
#include <list>
#include <windows.h>
using namespace std;
#include "CDrawSurface.h"
#include "CTransformDrawSurface.h"
class TGraphicsNode
{
	public:
		virtual void Render(CTransformDrawSurface& r) = 0;
};

class TCircle : public TGraphicsNode {
	int _x;
	int _y;
	double _radius;
	public:
	   TCircle(int x, int y, double radius )
	   {
		_x = x; _y=y; _radius = radius;
	   }
	int GetX() { return _x; }
	int GetY() { return _y; }
	int GetRadius() { return _radius; }
	virtual void Render(CTransformDrawSurface& r)  { printf("Rendering Circle...\n"); }
};

class TRectangle : public TGraphicsNode {
	int _x;
	int _y;
	int _width;
	int _height ;
 public:
	TRectangle(int x, int y, int width, int height ){ _x = x; _y=y; _width = width; _height = height;}
	int GetX() { return _x; }
	int GetY() { return _y; }
	int GetHeight() { return _height; }
                   int GetWidth()  { return _width;  }
	virtual void Render(CTransformDrawSurface& r)  { printf("Rendering Rectangle...\n"); }


};


class ShapeFileParser
{
	char *bfr;
	char *curr_ptr;
	char *file_name;
       public:
	ShapeFileParser( const char *filename )
	{
		file_name = (char *) malloc(strlen(filename)+1);	
		strcpy(file_name,filename);
	}

                   
                   char *skip_white ( char *np )
	{
                        while (*np == ' ' || *np == '\t' )  { np++; }
	      return np;
	}

                   char *next_valid_int(char *np, int *ret_value ) {
                          char *bfr = skip_white(np);
                          if ( !isdigit(*bfr) ) { *ret_value =0; return bfr; }
                          while ( isdigit(*bfr) )
                          {
                                       bfr ++;     
                          }
                          *bfr++ = 0;
                          *ret_value = atoi(np);
                           return bfr;
                   }

                   char *next_valid_double(char *np, double *ret_value ) {
                          char *bfr = skip_white(np);
                          if ( !isdigit(*bfr) ) { *ret_value =0; return bfr; }
                          while ( isdigit(*bfr) || *bfr ==  '.')
                          {
                                       bfr ++;     
                          }
                          *bfr++ = 0;
                          *ret_value = atof(np);
                           return bfr;
                   }

                   TGraphicsNode *ParseCircle(char *linebfr )
                    {
                               int temp_x, temp_y;
                               double radius;
                               char *plinebfr = next_valid_int(linebfr,&temp_x);
                               if ( *plinebfr == 0 ) { return 0; }
                               plinebfr = next_valid_int(plinebfr,&temp_y);
                               if ( *plinebfr == 0 ) { return 0; }
                               plinebfr = next_valid_double(plinebfr,&radius);
                               return new TCircle(temp_x,temp_y, radius );            
                     }

                   TGraphicsNode *ParseRectangle( char *linebfr ) {
                               int temp_x, temp_y;
                               int width, height;
                               char *plinebfr = next_valid_int(linebfr,&temp_x);
                               if ( *plinebfr == 0 ) { return 0; }
                               plinebfr = next_valid_int(plinebfr,&temp_y);
                               if ( *plinebfr == 0 ) { return 0; }
                               plinebfr = next_valid_int(plinebfr,&width);
                               if ( *plinebfr == 0 ) { return 0; }
                               plinebfr = next_valid_int(plinebfr, &height);
                               //---- Return the Graphics Item
                               return new TRectangle (temp_x,temp_y, width, height);        
                   }

                   TGraphicsNode * ParseLine ( char *linebfr )
	{
		if ( strnicmp(linebfr,"RECTANGLE ", 10 ) == 0 )
		{

			return ParseRectangle(linebfr+10);
		}

		else if ( strnicmp(linebfr,"CIRCLE ",7 ) == 0 )
		{
			
                                                          return ParseCircle(linebfr+7);
		}
                                     else { return 0; }

	}

	list<TGraphicsNode *>& Load() {
                           FILE *fp = fopen(file_name,"rt");
	        list<TGraphicsNode *> *mst = new list<TGraphicsNode *>();
	         while (!feof(fp))
	         {
		char linebfr[4096];
		fgets(linebfr,4095,fp);
		char *temp = linebfr;
		while (*temp == ' ' || *temp == '\t')
			temp ++;
		if (*temp == '#'  || *temp == 0 ) { continue; }

		TGraphicsNode * temp2 = ParseLine(temp);
                                     if ( temp2 != 0 ) {  mst->push_back(temp2 ); }

	         }

                           return *mst;
	}

};

list<TGraphicsNode *>&  LoadVectorsFromFile(const char *filename) {
	ShapeFileParser *parser = new ShapeFileParser(filename);
                   list<TGraphicsNode *>& ld = parser->Load();
                   printf("Finished...Loadin");
                   delete parser;
                   return ld;
}

#endif