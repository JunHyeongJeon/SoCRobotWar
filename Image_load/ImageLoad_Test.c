#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <list>
#include <vector>

#include "uart_api.h"
#include "cam_disp.h"

#define DISABLE_IMG_IRQ		0x24402
#define RD_IMG_DATA			0x24403
#define DISPLAY_IMG_DATA	0x24404
#define DISPLAY_IMG_DRAW	0x24405	
#define CLEAR_SCREEN		0x24407
#define GRAPHIC_FLIP	0x24410
int devfb;

typedef unsigned short _us;
typedef unsigned short* _pus;

_us _Red	= 0xF800;
_us _Green	= 0x07E0;
_us _Blue	= 0x001F;
_us _Black   = 0x0000;
_us _White   = 0xFFFF;

void Delay(int delay_time)
{
	while(delay_time)
		delay_time--;
}

typedef struct __draw_raw_value {
	int InitDX;
	int InitDY;
	int EndX;
	int EndY;
	int InitSX;
	int InitSY;
	int dxSx;
	int dxSy;
	int dySx;
	int dySy;
	int imgbuf_en;
	unsigned short *imgbuf;
} DrawRaw_value;

static DrawRaw_value draw_value;

struct __shade_color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

int draw_rotate_value(int cdx, int cdy, int ctx, int cty, float zoom, unsigned int angle)
{
	float MySin,MyCos;

	int InitDX, InitDY;
	int EndX, EndY;
	int InitSX, InitSY;
	int dxSx, dxSy;
	int dySx, dySy;
	int dx,dy;
	int x,y;
	float a;
	int cosa;
	int sina;
	int rhzoom;

	x = y = 0;
	dx = 320;
	dy = 480;

	long tx,ty;

	angle %= 360;
	a = angle*(3.1415926535f*2)/360.0f;

	MySin = (float)sin( a );
	MyCos = (float)cos( a );

	tx = (-cdx/zoom) * MyCos + (-cdy/zoom) * MySin;
	ty = (cdx/zoom) * MySin + (-cdy/zoom) * MyCos;

	if( zoom<=0 )	rhzoom = 0;
	else			rhzoom = (int)((float)(1<<9)/zoom);
	cosa = (int)(rhzoom * cos( a ));
	sina = (int)(rhzoom * sin( a ));

	if( dx<=0 || dy<=0 ) 
		return -1;

	InitDX	= x;
	InitDY	= y;
	EndX	= x+dx-1;
	EndY	= y+dy-1;

	InitSX	= (x+tx+ctx)*512;
	dxSx	= cosa;
	dxSy	= -sina;

	InitSY	= (y+ty+cty)*512;
	dySx	= sina;
	dySy	= cosa;

	draw_value.InitDX=InitDX;
	draw_value.InitDY=InitDY;
	draw_value.EndX=EndX;
	draw_value.EndY=EndY;
	draw_value.InitSX=InitSX;
	draw_value.InitSY=InitSY;
	draw_value.dxSx=dxSx;
	draw_value.dxSy=dxSy;
	draw_value.dySx=dySx;
	draw_value.dySy=dySy;
	
//	printf("InitDX=%d\n", InitDX);
//	printf("InitDY=%d\n", InitDY);
//	printf("EndX=%d\n", EndX);
//	printf("EndY=%d\n", EndY);
//	printf("InitSX=%d\n", InitSX);
//	printf("InitSY=%d\n", InitSY);
//	printf("dxSx=%d\n", dxSx);
//	printf("dxSy=%d\n", dxSy);
//	printf("dySx=%d\n", dySx);
//	printf("dySy=%d\n", dySy);

	return 0;
}

int draw_img_from_buffer(unsigned short *Buffer, int CenterX, int CenterY, float zoom, int rotate_angle)
{
	draw_rotate_value(CenterX,CenterY,90,60,zoom,rotate_angle);
	draw_value.imgbuf = Buffer;

	if( ioctl(devfb, DISPLAY_IMG_DRAW, &draw_value) ) {
		printf("Draw Image to LCD -> fail!!!\n");
		exit(1);
	}	
}

void ClearScreen(unsigned char r, unsigned char g, unsigned char b)
{
	struct __shade_color clear_shade;

	clear_shade.r = r;
	clear_shade.g = g;
	clear_shade.b = b;

	if( ioctl(devfb, CLEAR_SCREEN, &clear_shade) ) {
		printf("Clear Screen -> fail!!!\n");
		exit(1);
	}	
}

static int gFlip(void)
{
    return ioctl(devfb, GRAPHIC_FLIP, 0);
}

int ImageProcess_Open(void)
{
	int fb;
	fb=open("/mnt/f0/dev_module/imgproc", O_RDWR);
	if(fb < 0) {
		printf("/mnt/f0/dev_module/imgproc open failed!!\n");
		return -1;
	}

	devfb = fb;

	return 0;	
}

int ReadImageFromFPGA(unsigned int *addr)
{
	unsigned int buf_addr;
	
	if ( ioctl(devfb, RD_IMG_DATA, &buf_addr) ) {
		printf("ioctl RD_IMG_DATA error\n");
		return -1;
	}
	*addr = buf_addr;

	return 0;
}

int ImgDisplayToLCD(void)
{
	draw_rotate_value(160,240,90,60,2.5,90); // FPGA�κ��� �о� �� ����(180x120)�� �߽���(90,60)�� ���������Ͽ� 90�� ȸ���ϰ�, 2.5�� Ȯ���Ͽ� LCD�� �߽���(160,240)�� ��ġ�ϵ��� �����ϴ� �Լ� : comment by yyb[110909]
	if ( ioctl(devfb, DISPLAY_IMG_DATA, &draw_value) ) {
	    printf("ioctl DISPLAY_IMG_DATA error\n");
	    return -1;
	}

	return 0;
}

void ImgDisplayQuit(void)
{
	ioctl(devfb, DISABLE_IMG_IRQ, 0);
}

struct point
{
	int x;
	int y;

	point(int _x,int _y) 
	{
		x = _x;
		y = _y;
	}
};

float GetDistance(_us (*img)[256])
{
	std::vector<point> pointList;

	//��� 10���� �˻��Ѵ�.
	for(int x=10;x<180;x+=10)
	{
		for(int y=110;y>0;y--)
		{
			_us color = img[y][x];
			if(color == 0x00)
			{
				printf("POINT = [%d] [%d] \n",x,y);
				pointList.push_back(point(x,y));
				break;
			}
		}
	}

	while(pointList.size() > 2)
	{
		float fSum = 0.f;

		//���� ���̸� ��� ���Ѵ�.
		for(int i=0;i<pointList.size();++i)
		{
			fSum += pointList[i].y;
		}

		//���� ���̵��� ����� ���Ѵ�.
		float fAverage = fSum / pointList.size();

		//��հ� ũ�� ���̳��� ������ ã�Ƽ� PID��� �Ѵ�.
		float fMax = 0;
		int nMaxIndex = 0;
		for(int i=0;i<pointList.size();++i)
		{
			
			if(pointList[i].y > fMax)
			{
				fMax = pointList[i].y;
				nMaxIndex = i;
			}
		}

		pointList.erase(pointList.begin() + nMaxIndex);
	}

	if(pointList.size() == 2)
	{	
		float fLength = (pointList[0].y + pointList[1].y) / 2.f;
		printf("DISTANCE = [%f] \n",fLength);
		return fLength;
	}

	return 0.f;
}

float GetGradient(_us (*img)[256])
{
	std::vector<point> pointList;
	
	//��� ������ point�� ���Ѵ�.
	for(int x=10;x<180;x+=10)
	{
		for(int y=110;y>0;y--)
		{
			_us color = img[y][x];
			if(color == 0x00)
			{
				printf("POINT = [%d] [%d] \n",x,y);
				pointList.push_back(point(x,y));
				break;
			}
		}
	}

	//point�� 2�� ���ö����� �ݺ��ؼ� PID��� �Ѵ�.
	while(pointList.size() > 2)
	{
		std::vector<float> gradList;
		float fSum = 0;

		//��� ���⸦ ����
		for(int i=0;i<pointList.size() - 1;++i)
		{
			int nDx = pointList[i + 1].x - pointList[i].x;
			int nDy = pointList[i + 1].y - pointList[i].y;
			nDx == 0 ? nDx = 1 : nDx;

			float fGrad = (float)nDy / nDx;
			gradList.push_back(fGrad);
			fSum += fGrad;
		}

		//���� ���� ����� ����
		float fAverage = fSum / (pointList.size() - 1);

		//��հ� ���� ���� ũ�� ���� �� point�� ���ش�.
		float fMax = 0;
		int nMaxIndex = 0;
		for(int i=0;i<gradList.size();++i)
		{
			float fGrad = gradList[i];
			float fLength = abs(fGrad - fAverage);

			if(fLength > fMax)
			{
				fMax = fLength;
				nMaxIndex = i;
			}
		}

		pointList.erase(pointList.begin() + nMaxIndex);
	}

	if(pointList.size() == 2)
	{
		int nDx = pointList[1].x - pointList[0].x;
		int nDy = pointList[1].y - pointList[0].y;
		nDx == 0 ? nDx = 1 : nDx;
		float fGrad = (float)nDy / nDx;

		printf("GRADIENT = [%f] \n",fGrad);
		return fGrad;
	}

	return 0.f;
}

float LineTest(_us (*img)[256])
{
	
}

int main(int argc, char **argv)
{
	int x, y;
	char ch;
	int ret;
    unsigned char buff[5] = {0,};
	unsigned int buf_addr;
	unsigned short (*img_buf)[256];
	
	if(argc <= 1) {
		printf("Usage 1 : imgproc_test -rd     <Read Image Data>\n");
		printf("Usage 2 : imgproc_test -dp     <Display to LCD>\n");
		printf("Usage 3 : imgproc_test -dp2    <Display 2 Frame>\n");
		exit(1);
	}
	eagle_camera_off(); // Ŀ�ο��� ���۵Ǵ� Camera OFF : comment by yyb[110909]
	
	//[[ molink_yyb_110909_BEGIN -- �κ� ��ü���� ����� ���� UART �ʱ�ȭ
	ret = uart_open();
	if (ret < 0) return EXIT_FAILURE;
	uart_config(UART1, 115200, 8, UART_PARNONE, 1);
	//]] molink_yyb_110909_END -- �κ� ��ü���� ����� ���� UART �ʱ�ȭ

	ret = ImageProcess_Open(); // �̹��� ó���� ���� ����̹� Open : comment by yyb[110909]
	if (ret < 0) return EXIT_FAILURE;

	Delay(0xffffff);

	if(strcmp("-rd", argv[1]) == 0) {

		ret = ReadImageFromFPGA(&buf_addr); // FPGA�κ��� 1Frame(180x120)�� �̹��� Read : comment by yyb[110909]
		if (ret < 0) return EXIT_FAILURE;
		img_buf = (unsigned short (*)[256])buf_addr; 

		//TEST
		GetDistance(img_buf);
		GetGradient(img_buf);		
	}
	else if(strcmp("-dp", argv[1]) == 0) {
		ret = ImgDisplayToLCD(); // FPGA�� ���� �о�� �̹����� LCD�� �ǽð�����  Display : comment by yyb[110909]
		if (ret < 0) return EXIT_FAILURE;
		
		printf("\nPress Enter Key to STOP the test !!!");
		
		ch = getchar();
		ImgDisplayQuit(); // LCD Display ���� : comment by yyb[110909]
		printf("\nTest is Stopped\n");
	}
	else if(strcmp("-dp2", argv[1]) == 0) {
		ClearScreen(255, 255, 255); // LCD ȭ���� ������� Clear : comment by yyb[110909]
		gFlip(); // �׷��� ó���� ������ LCD�� ������ : comment by yyb[110909]
		ClearScreen(255, 255, 255);
		gFlip();
		ClearScreen(255, 255, 255);
		gFlip();
		ClearScreen(255, 255, 255);
		gFlip();
		printf("Clear Screen!\n");

		ret = ReadImageFromFPGA(&buf_addr);
		if (ret < 0) return EXIT_FAILURE;

		printf("Image Load from FPGA!\n");

		draw_value.imgbuf_en = 0; // �о� �� �����͸� ó������ �ʰ� �״�� �ٽ� LCD�� ������ �� ���� : comment by yyb[110909]
		ClearScreen(255, 255, 255);
		draw_img_from_buffer((unsigned short *)buf_addr, 160, 130, 1.8, 0); // buf_addr�� ��� �ִ� ������ 1.8�� Ȯ���Ͽ� 0�� ȸ���ϰ� �߽��� (160, 130)�� �Ͽ�  Display : comment by yyb[110909]
//		gFlip();
		printf("1. Draw Image to LCD!\n");
//    }


		printf("\nPress Enter Key to load image again !!!");
		ch = getchar();
		
		ret = ReadImageFromFPGA(&buf_addr);
		if (ret < 0) return EXIT_FAILURE;
		img_buf = (unsigned short (*)[256])buf_addr;
		printf("Image Load from FPGA!\n");

		//////////////////////////////////////////////////////////////////
		// img_buf�� �����Ϳ� ���� �̹��� ó�� ������ �� ��ġ���� �̷���� //
		//////////////////////////////////////////////////////////////////
		
		draw_value.imgbuf_en = 1; // �о� �� �����͸� ó���ϰ�, ó���� �����͸� LCD�� ������ �� ���� : comment by yyb[110909]
		draw_img_from_buffer((unsigned short *)buf_addr, 160, 360, 1.8, 0);
		gFlip();
		printf("2. Draw Image to LCD!\n");

		printf("\nPress Enter Key to STOP the test !!!");
		ch = getchar();
		printf("\nTest is Stopped\n");
		
	}
	else {
		printf("Usage 1 : imgproc_test -rd     <Read Image Data>\n");
		printf("Usage 2 : imgproc_test -dp     <Display to LCD>\n");
		printf("Usage 3 : imgproc_test -dp2    <Display 2 Frame>\n");
		exit(-1);
	}
	
	uart_close();
	close(devfb);

	eagle_camera_on(); // Ŀ�ο��� ���۵Ǵ� Camera ON : comment by yyb[110909]

	return(0);
}
