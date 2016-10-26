/***********************************************************************
 * OpenCV example
 * By Shiqi Yu 2006
 ***********************************************************************/
 
#include "cv.h"
#include "highgui.h"
 
int main( int argc, char** argv )
{
    IplImage* pImg; //��IplImageָ�
 
    //�d��D��
    if( argc == 2 && 
        (pImg = cvLoadImage( argv[1], 1)) != 0 )//[[��̎��argc==2�Ƿ���Ҫ�ĳ�argc==1���Ҹ���֮������\�гɹ������ţ���]] //  wmzzzz : ��"����"|"debug"|���command arguments �e���녢��(һ��·��:Ҫ���_���ļ�·��) �@�r argc==2 �ͺ�����...����ԇԇ��ӎׂ�
    {
        cvNamedWindow( "Image", 1 );//����ҕ��
        cvShowImage( "Image", pImg );//�@ʾ�D��
 
        cvWaitKey(0); //�ȴ����I
 
        cvDestroyWindow( "Image" );//�N��ҕ��
        cvReleaseImage( &pImg ); //ጷňD��
        return 0;
    }
 
    return -1;
}