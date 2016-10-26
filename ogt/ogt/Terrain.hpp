#pragma once
#include "cmn.h"
#include "ColorMap.hpp"

#define DIMX 256
#define DIMZ 256
#define SIZE 20


typedef struct
{
	double x;
	double y;
	double z;
} Vector;


class Terrain	
{
protected:
	Vector	terrain[DIMX*2][DIMZ*2];	// 地形
	GLuint m_Texture[DIMX][DIMZ]; //保存纹理对象的名称

public:
	Terrain(){};
	~Terrain(){};

	void InitTerrain(int maxh)
	{
		int x,z;
		double xi,zi;

		for(z=0;z<DIMZ*2;z++)
		{
			zi=(double)z-((double)DIMZ);
			zi=zi*SIZE;
			for(x=0;x<DIMX*2;x++)
			{
				xi=(double)x-((double)DIMX);
				xi=xi*SIZE;
				terrain[x][z].x=xi;
				if(x<DIMX){
					if(z<DIMZ){
						terrain[x][z].y=/*rand()%maxh; */(rand()*maxh)/RAND_MAX;
					}
					else{
						terrain[x][z].y=terrain[x][z-DIMZ].y;
					}
				}
				else{
					if(z<DIMZ){
						terrain[x][z].y=terrain[x-DIMX][z].y; 
					}
					else{
						terrain[x][z].y=terrain[x-DIMX][z-DIMZ].y;
					}
				}
				terrain[x][z].z=zi;
			}
		}


	};


	void Init(CString fileName)
	{

		char * buffer=new char[fileName.GetLength()+1]; 
		wcstombs(buffer,fileName.GetBuffer(),fileName.GetLength()+1);
		cv::Mat srcimg=cv::imread(buffer);

		int xl=srcimg.cols/DIMX;
		int zl=srcimg.rows/DIMZ;

		glGenTextures(1, m_Texture[0]);
		LoadTexture(fileName,m_Texture[0][0]);

		cv::Mat tmp(srcimg);
		cv::flip(tmp,srcimg,-1);

		int x,z;
		double xi,zi;
		for(x=0;x<DIMX*2;x++)
		{
			xi=(double)x-((double)DIMX);
			xi=xi*SIZE;
			if(x<DIMX){

				//glGenTextures(DIMZ, m_Texture[x]);

				for(z=0;z<DIMZ*2;z++)
				{
					zi=(double)z-((double)DIMZ);
					zi=zi*SIZE;

					terrain[x][z].x=xi;

					if(z<DIMZ){

						cv::Mat tempRoi(srcimg,cv::Rect(x*xl,z*zl,xl,zl));

						cv::Mat bwsrc(srcimg.rows,srcimg.cols,CV_8UC1);;
						cv::cvtColor(tempRoi, bwsrc, CV_RGB2GRAY);


						//cv::Mat outimg(128,128,tempRoi.type());
						//cv::resize(tempRoi,outimg,outimg.size());

						//int ti=tempRoi.depth();

						//cv::imwrite("temp.bmp",
							//tempRoi
							//outimg
							//);

						//LoadTexture(L"temp.bmp",m_Texture[x][z]);

						//terrain[x][z].y=tempRoi.at< cv::Vec<unsigned char, 3> >(0,0)[1];

						terrain[x][z].y=bwsrc.at< unsigned char >(0,0);
						//terrain[x][z].y=/*rand()%maxh; */(rand()*maxh)/RAND_MAX;
					}
					else{
						terrain[x][z].y=terrain[x][z-DIMZ].y;
					}

					terrain[x][z].z=zi;

					printf("%d,%d\n",x,z);
				}

			}
			else{
				for(z=0;z<DIMZ*2;z++)
				{
					zi=(double)z-((double)DIMZ);
					zi=zi*SIZE;

					terrain[x][z].x=xi;

					if(z<DIMZ){
						terrain[x][z].y=terrain[x-DIMX][z].y; 
					}
					else{
						terrain[x][z].y=terrain[x-DIMX][z-DIMZ].y;
					}
					terrain[x][z].z=zi;

					//printf("%d,%d\n",x,z);
				}

			}
		}


	};


	void Draw()
	{
		register int x,z;
		register float fk=(float)1/100;
double s,t;
glEnable(GL_TEXTURE_2D);
glBindTexture( GL_TEXTURE_2D, m_Texture[0][0] );

		for(z=0;z<DIMZ*2-1;z++)
		{
			for(x=0;x<DIMX*2-1;x++)
			{

				
			//glBindTexture( GL_TEXTURE_2D, m_Texture[x%DIMX][z%DIMZ] );

			//printf("%d,%d\n",x%DIMX,z%DIMZ);

			glBegin(GL_QUADS);

			//printf("%g,%g\n",(double(x))/DIMX,(double(z))/DIMZ);

			s=(double(x))/DIMX;
			s-=floor(s);
			t=(double(z))/DIMZ;
			t-=floor(t);
			glTexCoord2d( s, t );


			glVertex3f(terrain[x][z].x,terrain[x][z].y,terrain[x][z].z);

						s=(double(x))/DIMX;
			s-=floor(s);
			t=(double(z+1))/DIMZ;
			t-=floor(t);
			glTexCoord2d( s, t );
			//glTexCoord2d( (double(x))/DIMX, (double(z+1))/DIMZ );
			glVertex3f(terrain[x][z+1].x,terrain[x][z+1].y,terrain[x][z+1].z);
						s=(double(x+1))/DIMX;
			s-=floor(s);
			t=(double(z+1))/DIMZ;
			t-=floor(t);
			glTexCoord2d( s, t );

			//glTexCoord2d( (double(x+1))/DIMX, (double(z+1))/DIMZ );
			glVertex3f(terrain[x+1][z+1].x,terrain[x+1][z+1].y,terrain[x+1][z+1].z);
						s=(double(x+1))/DIMX;
			s-=floor(s);
			t=(double(z))/DIMZ;
			t-=floor(t);
			glTexCoord2d( s, t );
			//glTexCoord2d( (double(x+1))/DIMX, (double(z))/DIMZ );
			glVertex3f(terrain[x+1][z].x,terrain[x+1][z].y,terrain[x+1][z].z);
			glEnd();						
			}
		}

glDisable(GL_TEXTURE_2D);	



		//glEnable(GL_TEXTURE_2D);
		//glBindTexture( GL_TEXTURE_2D, m_Texture[0] );



		//glBegin(GL_QUADS);
		//		//glTexCoord2d( 0.0, 0.0 );
		//		glColor3f(0.0f, 1, 0.0f);
		//		glVertex3f(0,0,0);
		//		//glTexCoord2d( 0.0, 1.0 );
		//		glColor3f(0.0f, 1, 0.0f);
		//		glVertex3f(0,0,1);
		//		//glTexCoord2d( 1.0, 1.0 );
		//		glColor3f(0.0f, 1, 0.0f);
		//		glVertex3f(5,0,0);
		//		//glTexCoord2d( 1.0, 0.0 );
		//		glColor3f(0.0f, 1, 0.0f);
		//		glVertex3f(1,0,0);
		//		glEnd();

		//		glColor3f(0.0f, 1, 0.0f);

		//for(z=0;z<DIMZ*2-1;z++)
		//{
		//	for(x=0;x<DIMX*2-1;x++)
		//	{
		//		glBegin(GL_QUADS);
		//		//glTexCoord2d( 0.0, 0.0 );

		//		COLORREF oc;

		//		oc=am.GetColor(terrain[x][z].y/maxh);
		//		glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
		//		//glColor3f(0.0f, Terrain[x][z].y/maxh, 1-Terrain[x][z].y/maxh);
		//		glVertex3f(terrain[x][z].x,terrain[x][z].y,terrain[x][z].z);
		//		//glTexCoord2d( 0.0, 1.0 );
		//		oc=am.GetColor(terrain[x][z+1].y/maxh);
		//		glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
		//		//glColor3f(0.0f, Terrain[x][z+1].y/maxh, 1-Terrain[x][z+1].y/maxh);
		//		glVertex3f(terrain[x][z+1].x,terrain[x][z+1].y,terrain[x][z+1].z);
		//		//glTexCoord2d( 1.0, 1.0 );
		//		oc=am.GetColor(terrain[x+1][z+1].y/maxh);
		//		glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
		//		//glColor3f(0.0f, Terrain[x+1][z+1].y/maxh, 1-Terrain[x+1][z+1].y/maxh);
		//		glVertex3f(terrain[x+1][z+1].x,terrain[x+1][z+1].y,terrain[x+1][z+1].z);
		//		//glTexCoord2d( 1.0, 0.0 );
		//		oc=am.GetColor(terrain[x+1][z].y/maxh);
		//		glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
		//		//glColor3f(0.0f, Terrain[x+1][z].y/maxh, 1-Terrain[x+1][z].y/maxh);
		//		glVertex3f(terrain[x+1][z].x,terrain[x+1][z].y,terrain[x+1][z].z);
		//		glEnd();
		//	}
		//}

		//glDisable(GL_TEXTURE_2D);


	};


	void DrawTerrain(ColorMap am, int maxh)
	{
		register int x,z;
		register float fk=(float)1/100;

		for(z=0;z<DIMZ*2-1;z++)
		{
			for(x=0;x<DIMX*2-1;x++)
			{
				glBegin(GL_QUADS);
				//glTexCoord2d( 0.0, 0.0 );

				COLORREF oc;

				oc=am.GetColor(terrain[x][z].y/maxh);
				glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
				//glColor3f(0.0f, Terrain[x][z].y/maxh, 1-Terrain[x][z].y/maxh);
				glVertex3f(terrain[x][z].x,terrain[x][z].y,terrain[x][z].z);
				//glTexCoord2d( 0.0, 1.0 );
				oc=am.GetColor(terrain[x][z+1].y/maxh);
				glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
				//glColor3f(0.0f, Terrain[x][z+1].y/maxh, 1-Terrain[x][z+1].y/maxh);
				glVertex3f(terrain[x][z+1].x,terrain[x][z+1].y,terrain[x][z+1].z);
				//glTexCoord2d( 1.0, 1.0 );
				oc=am.GetColor(terrain[x+1][z+1].y/maxh);
				glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
				//glColor3f(0.0f, Terrain[x+1][z+1].y/maxh, 1-Terrain[x+1][z+1].y/maxh);
				glVertex3f(terrain[x+1][z+1].x,terrain[x+1][z+1].y,terrain[x+1][z+1].z);
				//glTexCoord2d( 1.0, 0.0 );
				oc=am.GetColor(terrain[x+1][z].y/maxh);
				glColor3ub(GetRValue(oc),GetGValue(oc),GetBValue(oc));
				//glColor3f(0.0f, Terrain[x+1][z].y/maxh, 1-Terrain[x+1][z].y/maxh);
				glVertex3f(terrain[x+1][z].x,terrain[x+1][z].y,terrain[x+1][z].z);
				glEnd();
			}
		}

	};




};