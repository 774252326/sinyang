/***********************************************************************
 * OpenCV example
 * By Shiqi Yu 2006
 ***********************************************************************/
 
#include "cv.h"
#include "highgui.h"
 
int main( int argc, char** argv )
{
    IplImage* pImg; //聲明IplImage指針
 
    //載入圖像
    if( argc == 2 && 
        (pImg = cvLoadImage( argv[1], 1)) != 0 )//[[此處的argc==2是否需要改成argc==1？我改了之後才能運行成功。求大牛解惑]] //  wmzzzz : 在"屬性"|"debug"|里的command arguments 裡加入參數(一個路徑:要打開的文件路徑) 這時 argc==2 就合理了...可以試試多加幾個
    {
        cvNamedWindow( "Image", 1 );//創建視窗
        cvShowImage( "Image", pImg );//顯示圖像
 
        cvWaitKey(0); //等待按鍵
 
        cvDestroyWindow( "Image" );//銷毀視窗
        cvReleaseImage( &pImg ); //釋放圖像
        return 0;
    }
 
    return -1;
}