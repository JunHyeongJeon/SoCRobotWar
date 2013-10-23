#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include "MyChild.h"


#include "../library/cam_disp.h"

#define DISABLE_IMG_IRQ		0x24402
#define RD_IMG_DATA			0x24403
#define DISPLAY_IMG_DATA	0x24404
#define DISPLAY_IMG_DRAW	0x24405	
#define CLEAR_SCREEN		0x24407
#define GRAPHIC_FLIP	0x24410

int devfb;

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

int draw_img_from_buffer(unsigned short *Buffer, int CenterX, int CenterY, float zoom, int rotate_angle)		//�ʿ�
{
	draw_rotate_value(CenterX,CenterY,90,60,zoom,rotate_angle);
	draw_value.imgbuf = Buffer;

	if( ioctl(devfb, DISPLAY_IMG_DRAW, &draw_value) ) {
		printf("Draw Image to LCD -> fail!!!\n");
		exit(1);
	}
}

void ClearScreen(unsigned char r, unsigned char g, unsigned char b)	// �ʿ�
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

static int gFlip(void)	//�ʿ�
{
    return ioctl(devfb, GRAPHIC_FLIP, 0);
}

int ImageProcess_Open(void)	// �ʿ�
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

int ReadImageFromFPGA(unsigned int *addr)	// �ʿ�
{
	unsigned int buf_addr;
	
	if ( ioctl(devfb, RD_IMG_DATA, &buf_addr) ) {
		printf("ioctl RD_IMG_DATA error\n");
		return -1;
	}
	*addr = buf_addr;

	return 0;
}

int ImgDisplayToLCD(void) // �ʿ�
{
	draw_rotate_value(160,240,90,60,2.5,90); // FPGA�κ��� �о� �� ����(180x120)�� �߽���(90,60)�� ���������Ͽ� 90�� ȸ���ϰ�, 2.5�� Ȯ���Ͽ� LCD�� �߽���(160,240)�� ��ġ�ϵ��� �����ϴ� �Լ� : comment by yyb[110909]
	if ( ioctl(devfb, DISPLAY_IMG_DATA, &draw_value) ) {
	    printf("ioctl DISPLAY_IMG_DATA error\n");
	    return -1;
	}

	return 0;
}

void ImgDisplayQuit(void)	// �ʿ��
{
	ioctl(devfb, DISABLE_IMG_IRQ, 0);
}

CCommProc commProc;
int main(int argc, char **argv)
{
	int x, y;
	char ch;
	int ret = 0;
	unsigned char buff[5] = {0,};
	unsigned int buf_addr;
	unsigned short (*img_buf)[256];
	_us (*img_left)[256];
	_us (*img_right)[256];
	//test
	
	unsigned short (*img_lcd)[256];

	if(argc <= 1) {
		printf("Usage 1 : imgproc_test -rd     <Read Image Data>\n");
		printf("Usage 2 : imgproc_test -dp     <Display to LCD>\n");
		printf("Usage 3 : imgproc_test -dp2    <Display 2 Frame>\n");
		exit(1);
	}
	
	eagle_camera_off();
	ret = ImageProcess_Open(); // �̹��� ó���� ���� ����̹� Open : comment by yyb[110909]
	if (ret < 0) return EXIT_FAILURE;

	Delay(0xffffff);

	if(strcmp("-rd", argv[1]) == 0) {

		commProc.Connect(COMM_TYPE_CAM_TO_MGR);
		commProc.Connect(COMM_TYPE_MGR_TO_CAM);

		PIPE_PROTOCOL protocol = PP_NONE;
		int tempSize = 0;
		char tempSend[4] = {0,};
		char tempRev[3] = {0, 0};
		int RevSize = 3;

		
		CMission* pBari = new CBari();
		CMission* pHurdle = new CHurdle();
		CMission* pTurnel1 = new CTurnel1();
		CMission* pStair = new CStair();
		CMission* pTurnel2 = new CTurnel2();
		CMission* pGolf = new CGolf();
		CMission* pRed = new CRed();
		CMission* pBari2 = new CBari2();
		CMission* pLine = new CLine();
		CMission* pTurn = new CTurn();
		TSEND m_send;
		M_STATE m_state = M_BARI1;
		
		
		draw_value.imgbuf_en = 1;

		while(1){
			////TK Manager�κ��� ����� �޴� �κ�			

			commProc.ReceiveMessage(COMM_TYPE_MGR_TO_CAM, tempRev, RevSize);	

			m_state = (M_STATE)tempRev[1];
			


			////TK ������ ��ºκ�
			ret = ReadImageFromFPGA(&buf_addr); // FPGA�κ��� 1Frame(180x120)�� �̹��� Read : comment by yyb[110909]
			if (ret < 0) return EXIT_FAILURE;
			
			img_buf = (unsigned short (*)[256])buf_addr; // img_buf�� �о� �� �̹��� �����͸� �迭(180x120)�� ó���ϱ� ����  ������ �迭 ���� : comment by yyb[110909]

		
			switch(m_state){
				////TK �ٲ����Һκ�
			case M_BARI1:
				m_send  = pBari->Missioning(img_buf, tempRev[2]);
				break;
			case M_HURDLE:
				m_send = pHurdle->Missioning(img_buf, tempRev[2]);
				break;
			case M_TURNEL1:
				m_send = pTurnel1->Missioning(img_buf, tempRev[2]);
				break;
			case M_STAIR:
				m_send = pStair->Missioning(img_buf, tempRev[2]);
				break;
			case M_TURNEL2:
				m_send = pTurnel2->Missioning(img_buf, tempRev[2]);
				break;
			case M_GOLF:
				m_send = pGolf->Missioning(img_buf, tempRev[2]);
				break;
			case M_RED:
				m_send = pRed->Missioning(img_buf, tempRev[2]);
				break;
			case M_BARI2:
				m_send = pBari2->Missioning(img_buf, tempRev[2]);
				break;
			case M_ALIGN_CENTER:
				m_send = pLine->Missioning(img_buf, tempRev[2]); 
				break;
			case M_TURN:
				m_send = pTurn->Missioning(img_buf, tempRev[2]);
				break;
			}

			//LCDDebug(img_buf,m_state);
			draw_img_from_buffer((unsigned short *)img_buf, 160, 240, 2.6, 90);
			gFlip();			
			//printf("--------TKSEND : M_STATE = %s \n",commProc.GetMStateString(m_state));
			tempSend[0] = PP_CAM_RESULT;
			tempSend[1] = m_send.state;
			tempSend[2] = m_send.step;
			tempSend[3] = m_send.now;
  			/*printf("--------TKSEND : state = %d\n", tempSend[1]);
  			printf("--------TKSEND : step = %d\n", tempSend[2]);
  			printf("--------TKSEND : now = %d\n", tempSend[3]);*/
			tempSize = 4;
			
					
										
			commProc.SendMessage(tempSend, tempSize);

		}
		

		delete pBari;
		delete pHurdle;
		delete pTurnel1;
		delete pStair;
		delete pTurnel2;
		delete pGolf;
		delete pRed;
		delete pBari2;
		delete pTurn;
		delete pLine;
		
		//////////////TKTK

#if 0 //[[ molink_yyb_110909_BEGIN -- FPGA�κ��� �о�� �����͸� ó���ϴ� ������ ���� ��
		for(y=0; y<120; y++) {
			for(x=0; x<180; x++) {
				sprintf((char *)buff, "%04x", img_buf[y][x]);
				
			}
		}
#endif //]] molink_yyb_110909_END -- FPGA�κ��� �о�� �����͸� ó���ϴ� ������ ���� ��

	    printf("\t read 1 frame imgae data completely\n");
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
	
	
	close(devfb);


	return 0;
}
