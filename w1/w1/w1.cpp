///////////////////////////////////////////////////////////////////////////////
//
// This example demonstrates how to directly manipulate the pixels in the image buffer.
// It also gives an overview over the common color formats and how the pixels of each format 
// are organized in the buffer.
//
#define _WIN32_WINNT 0x0500

#include <conio.h>
#include <tisudshl.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "Common/CmdHelper.h"

#include <opencv2\opencv.hpp>
//#include <iostream>

using namespace _DSHOWLIB_NAMESPACE;

MemBufferCollection::tMemBufferPtr	getSingleFrame( Grabber& gr, tColorformatEnum type )
{
	smart_ptr<FrameHandlerSink> pHandlerSink = FrameHandlerSink::create( type, 1 );
	pHandlerSink->setSnapMode( true );

	gr.setSinkType( pHandlerSink );

	gr.startLive( false );
	Sleep( 250 ); // give the device time to adjust automatic settings i.e. auto exposure
	pHandlerSink->snapImages( 1, 2000 );
	gr.stopLive();

	// get pointer to the image data
	return pHandlerSink->getLastAcqMemBuffer();
}

unsigned __int16 byteSwap( unsigned __int16 x )
{
	BYTE* p = (BYTE*)&x;
	return ((unsigned int)p[0] << 8) | p[1];
}

unsigned __int16 YGB0PixelFromInt16Value( unsigned __int16 value )
{
	return byteSwap( value << 6 );
}

unsigned __int16 Int16ValueFromYGB0Pixel( unsigned __int16 pixel )
{
	return byteSwap( pixel ) >> 6;
}

unsigned __int16 YGB1PixelFromInt16Value( unsigned __int16 value )
{
	return byteSwap( value );
}

unsigned __int16 Int16ValueFromYGB1Pixel( unsigned __int16 pixel )
{
	return byteSwap( pixel );
}





cv::Mat getFrame(Grabber &grabber, tColorformatEnum type)
{
	cv::Mat img;
	if(type>0){
		MemBufferCollection::tMemBufferPtr pActiveBuf = getSingleFrame( grabber, type );
		BYTE* pbImgData = pActiveBuf->getPtr();	
		SIZE dim = pActiveBuf->getFrameType().dim;

		cv::Mat rbimg;
		std::vector<cv::Mat> mask(3);
		std::vector<cv::Mat> channel(3);

		switch(type){
		case eRGB8:
		case eY800:
			img=cv::Mat(dim.cy,dim.cx,CV_8UC1,pbImgData);
			return img.clone();
		case eRGB24:
			img=cv::Mat(dim.cy,dim.cx,CV_8UC3,pbImgData);
			return img.clone();
		case eRGB32:
			img=cv::Mat(dim.cy,dim.cx,CV_8UC4,pbImgData);
			return img.clone();
		case eRGB555:
			img=cv::Mat(dim.cy,dim.cx,CV_16UC1,pbImgData);
			//return img.clone();

			mask[0]=cv::Mat(dim.cy,dim.cx,CV_16UC1);
			mask[0].setTo(eRGB555_B);
			cv::bitwise_and(img,mask[0],channel[0]);
			channel[0].convertTo(channel[0],CV_8UC1);

			mask[1]=cv::Mat(dim.cy,dim.cx,CV_16UC1);
			mask[1].setTo(eRGB555_G);
			cv::bitwise_and(img,mask[1],channel[1]);
			channel[1]/=(eRGB555_B+1);
			channel[1].convertTo(channel[1],CV_8UC1);

			mask[2]=cv::Mat(dim.cy,dim.cx,CV_16UC1);
			mask[2].setTo(eRGB555_R);
			cv::bitwise_and(img,mask[2],channel[2]);
			channel[2]/=(eRGB555_G+1);
			channel[2].convertTo(channel[2],CV_8UC1);

			cv::merge(channel,rbimg);
			return rbimg;
		case eRGB565:
			img=cv::Mat(dim.cy,dim.cx,CV_16UC1,pbImgData);
			//return img.clone();

			mask[0]=cv::Mat(dim.cy,dim.cx,CV_16UC1);
			mask[0].setTo(eRGB565_B);
			cv::bitwise_and(img,mask[0],channel[0]);
			channel[0].convertTo(channel[0],CV_8UC1);

			mask[1]=cv::Mat(dim.cy,dim.cx,CV_16UC1);
			mask[1].setTo(eRGB565_G);
			cv::bitwise_and(img,mask[1],channel[1]);
			channel[1]/=(eRGB565_B+1);
			channel[1].convertTo(channel[1],CV_8UC1);

			mask[2]=cv::Mat(dim.cy,dim.cx,CV_16UC1);
			mask[2].setTo(eRGB565_R);
			cv::bitwise_and(img,mask[2],channel[2]);
			channel[2]/=(eRGB565_G+1);
			channel[2].convertTo(channel[2],CV_8UC1);

			cv::merge(channel,rbimg);
			return rbimg;
		case eUYVY:
		case eYGB1:
		case eYGB0:
		case eBY8:
		default:
			return img;
		}

	}

	return img;
}

int main(int argc, char* argv[])
{
	// Initialize the library.
	DShowLib::InitLibrary();


	Grabber grabber;

	// Disable overlay. If it was in the graph, we could not get UYVY images.
	grabber.setOverlayBitmapPathPosition( ePP_NONE );

	if( !setupDeviceFromFile( grabber ) )
	{
		return -1;
	}


	while(true){
		//cv::Mat ii=getFrame(grabber,eRGB8);
		//cv::imshow("RGB565",ii);
		cv::imshow("RGB565",getFrame(grabber,eRGB8));
	}

	cv::imshow("RGB8", getFrame(grabber,eRGB8));
	cv::imshow("Y800", getFrame(grabber,eY800));
	cv::imshow("RGB24", getFrame(grabber,eRGB24));
	cv::imshow("RGB32",getFrame(grabber,eRGB32));
	cv::imshow("RGB555",getFrame(grabber,eRGB555));
	cv::imshow("RGB565",getFrame(grabber,eRGB565));





	//cv::Mat img5550=getFrame(grabber,eRGB555);
	//cv::Mat img555=getFrame(grabber,eRGB555);
	//cv::Mat im=(img555==img5550);
	//cv::Mat ad;
	//cv::absdiff(img555,img5550,ad);
	//cv::imshow("cmp",im);
	//cv::imshow("absdiff",ad);



	cv::waitKey();



	printf("Press any key to continue.");
	getch();

	return 0;
}
