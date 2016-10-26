#pragma once

#include "opencv2/opencv.hpp"




cv::Point WarpPerspectiveEx(cv::Mat src,
	cv::OutputArray dst,
	cv::Mat M,
	int flags=1,
	int borderMode=0,
	const cv::Scalar &borderValue=cv::Scalar()
	){

		cv::Point origin(0,0);

		if(src.dims!=2
			|| src.rows <0
			|| src.cols <0)
			return origin;

		cv::Mat osz(4,1,CV_64FC2,cv::Scalar(0,0));
		osz.at<cv::Vec2d>(2,0)[1]=src.rows;
		osz.at<cv::Vec2d>(3,0)[1]=src.rows;
		osz.at<cv::Vec2d>(1,0)[0]=src.cols;
		osz.at<cv::Vec2d>(3,0)[0]=src.cols;


		//for(int j=0;j<osz.rows;j++){
		//	for(int k=0;k<osz.cols;k++){
		//		double a=osz.at<cv::Vec<double,2>>(j,k)[0];
		//		double b=osz.at<cv::Vec<double,2>>(j,k)[1];
		//		TRACE(L"(%g,%g)",a,b);
		//	}
		//	TRACE(L"\n");
		//}


		cv::Mat nsz(4,1,CV_64FC2,cv::Scalar(0,0));

		cv::perspectiveTransform(osz,nsz,M);

		//for(int j=0;j<nsz.rows;j++){
		//	for(int k=0;k<nsz.cols;k++){
		//		double a=nsz.at<cv::Vec<double,2>>(j,k)[0];
		//		double b=nsz.at<cv::Vec<double,2>>(j,k)[1];
		//		TRACE(L"(%g,%g)",a,b);
		//	}
		//	TRACE(L"\n");
		//}


		cv::Vec2d pt=nsz.at<cv::Vec2d>(0,0);

		cv::Vec2d sz=pt;

		if(nsz.at<cv::Vec2d>(1,0)[0]<pt[0])
			pt[0]=nsz.at<cv::Vec2d>(1,0)[0];
		if(nsz.at<cv::Vec2d>(1,0)[0]>sz[0])
			sz[0]=nsz.at<cv::Vec2d>(1,0)[0];
		if(nsz.at<cv::Vec2d>(1,0)[1]<pt[1])
			pt[1]=nsz.at<cv::Vec2d>(1,0)[1];
		if(nsz.at<cv::Vec2d>(1,0)[1]>sz[1])
			sz[1]=nsz.at<cv::Vec2d>(1,0)[1];

		if(nsz.at<cv::Vec2d>(2,0)[0]<pt[0])
			pt[0]=nsz.at<cv::Vec2d>(2,0)[0];
		if(nsz.at<cv::Vec2d>(2,0)[0]>sz[0])
			sz[0]=nsz.at<cv::Vec2d>(2,0)[0];
		if(nsz.at<cv::Vec2d>(2,0)[1]<pt[1])
			pt[1]=nsz.at<cv::Vec2d>(2,0)[1];
		if(nsz.at<cv::Vec2d>(2,0)[1]>sz[1])
			sz[1]=nsz.at<cv::Vec2d>(2,0)[1];

		if(nsz.at<cv::Vec2d>(3,0)[0]<pt[0])
			pt[0]=nsz.at<cv::Vec2d>(3,0)[0];
		if(nsz.at<cv::Vec2d>(3,0)[0]>sz[0])
			sz[0]=nsz.at<cv::Vec2d>(3,0)[0];
		if(nsz.at<cv::Vec2d>(3,0)[1]<pt[1])
			pt[1]=nsz.at<cv::Vec2d>(3,0)[1];
		if(nsz.at<cv::Vec2d>(3,0)[1]>sz[1])
			sz[1]=nsz.at<cv::Vec2d>(3,0)[1];

		origin.x=floor(pt[0]);
		origin.y=floor(pt[1]);

		//for(int j=0;j<M.rows;j++){
		//	for(int k=0;k<M.cols;k++){
		//		double a=M.at<cv::Vec<double,1>>(j,k)[0];
		//		TRACE(L"%g,",a);
		//	}
		//	TRACE(L"\n");
		//}



		//cv::Mat TM = (cv::Mat_<double>(3,1) << -origin.x, -origin.y, 0);
	
		//cv::Mat FM=TM*M.row(2)+M;

		cv::Mat TM = (cv::Mat_<double>(3,3) << 1, 0 ,-origin.x, 0, 1, -origin.y, 0,0,1);
		cv::Mat FM=TM*M;
			//cv::Mat FM=M;

		cv::Size imgsz;
		imgsz.height=ceil(sz[1]);
		imgsz.height+=1-origin.y;
		imgsz.width=ceil(sz[0]);
		imgsz.width+=1-origin.x;


		//for(int j=0;j<TM.rows;j++){
		//	for(int k=0;k<TM.cols;k++){
		//		double a=TM.at<cv::Vec<double,1>>(j,k)[0];
		//		TRACE(L"%g,",a);
		//	}
		//	TRACE(L"\n");
		//}

		//for(int j=0;j<FM.rows;j++){
		//	for(int k=0;k<FM.cols;k++){
		//		double a=FM.at<cv::Vec<double,1>>(j,k)[0];
		//		TRACE(L"%g,",a);
		//	}
		//	TRACE(L"\n");
		//}
		//
		//cv::perspectiveTransform(osz,nsz,FM);

		//for(int j=0;j<nsz.rows;j++){
		//	for(int k=0;k<nsz.cols;k++){
		//		double a=nsz.at<cv::Vec<double,2>>(j,k)[0];
		//		double b=nsz.at<cv::Vec<double,2>>(j,k)[1];
		//		TRACE(L"(%g,%g)",a,b);
		//	}
		//	TRACE(L"\n");
		//}

	

		cv::warpPerspective(src,dst,FM,imgsz,flags,borderMode,borderValue);

		return origin;
}