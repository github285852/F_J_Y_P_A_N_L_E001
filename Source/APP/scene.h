#ifndef __SCENE_H
#define __SCENE_H



#define EXFile_MAX_LEN		20   //文件名最大
#define EXFile_MAX_NUM		10   //保存最大文件个数

#define FRAME_MIN_T			10  //最小间隔时间

unsigned char EXFile_Browser(char* path,char *ex_name,char *name);

void ImportInit(void);
void ImportTask(void);
void brower_scene_file(void);


#endif


