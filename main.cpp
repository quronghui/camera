#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "camera.h"

#define DEV_NAME_LENGTH		50                         // This is for dev_name = "/dev/video.
//#define NUM_FRAM		10                            //This is photo quantity.

int main(int argc, char ** argv) {
    //char *dev_name = "/dev/video0";	//ITU         // 1. But what is meaning about the ITU.
    //char *dev_name = "/dev/video1";	//UVC
    char *dev_name = NULL;
    FILE * outf = 0;
    unsigned int image_size;

    int camera_type = 0; //0:ITU, 1:UVC	            // 2. If I change it camera_type = 1 the will have other things? --can not photo
    int width=640;                                  // We can change the 
    int height=480;

    if(argc != 2)                                   //3. we will put into three parameters.like this: ./camera /dev/video0 640x480. So we don't have myself name?-name is 64 line
    {
	   printf("usage: ./camera  NUM_FRAM\n");
	
	   return 0;
    }

	dev_name = (char *)malloc(sizeof(char) * DEV_NAME_LENGTH);
	if(!dev_name)
	{
		printf("malloc mem error\n");
		return -1;
	}

    memset(dev_name, 0, sizeof(char) * DEV_NAME_LENGTH);//为新申请的内存做初始化工. This is Init's work, so we think it that clear.
	strcpy(dev_name, "/dev/video0");                            // This is copy funcations. It is will copy the second parrameter, about is "/dev/video0"

	if(!strcmp(dev_name, "/dev/video1"))           //4. When the strcpy funcation later. The dev_name is  /dev/video*. Maybe change other ways.
	{
		camera_type = 1;
	}
	else
	{
		camera_type = 0;
	}
    
/*	if(!strcmp(argv[2], "640x480"))
	{
		width = 640;
    		height = 480;
	}
	else if(!strcmp(argv[2], "800x600"))
	{
		width = 800;
        height = 600;
	}
	else
	{
		width = 640;
                height = 480;
	}
*/
    int NUM_FRAM = atoi(argv[1]);
    outf = fopen("camera.yuv", "wb");              //5. This is file name. So wo need setting it.'wb' Only writer binary
    
    Camera *camera;                             //6. Why? 
    unsigned char image[width*height*2];        //7.  Why?----width * hight * 3 / 2byte

    clock_t starttime, endtime;
    double totaltime;

    camera=new Camera(dev_name,width,height, camera_type);

    if(!camera->OpenDevice()){                                          // OpenDevice() include all init and start_capturing.
	   printf("fun (1) OpenDevice is open failed: %s, line = %d\n", __FUNCTION__, __LINE__);
        return -1;
    }

	printf("funfun (1) OpenDevice is open success :%s, line = %d\n", __FUNCTION__, __LINE__);
    image_size=camera->getImageSize();
	printf("fun (2) getImageSize get the file size:%s, line = %d\n", __FUNCTION__, __LINE__);
    starttime = clock();

    //int frames=50;
    unsigned int writesize=0;
	printf("fun(3) the frame is the photo quality :%s, line = %d\n", __FUNCTION__, __LINE__);
    for(int i=0;i<NUM_FRAM;i++){                                // 7.Setting is about photo?
        if(!camera->GetBuffer(image)){                          // 8.When we put third parameter, unsigned char image[width*height*2];
		  printf("fun(4) GetBuffer is open failed. :%s, line = %d\n", __FUNCTION__, __LINE__);
            break;
        }
		printf("fun(4) GetBuffer is open success:%s, line = %d\n", __FUNCTION__, __LINE__);
        writesize=fwrite(image,1,image_size,outf);//向文件写数据  // 9. Find the camera->getImageSize() Way?
        //fflush(outf);
        //image_size
        printf("this image's byte is :%d, this image_size's byte is %d.\n", image , image_size);

        printf("the value is the %d have many writesize:%d\n",i,writesize);
    }

	printf("fun(5) is already write all frame in file :%s, line = %d\n", __FUNCTION__, __LINE__);
    endtime = clock();                                         
    totaltime = (double)( (endtime - starttime)/(double)CLOCKS_PER_SEC );//总时间
    printf("time :%f, rate :%f\n",totaltime,NUM_FRAM/totaltime);
    camera->CloseDevice();
    fclose(outf);
    return 0;
}