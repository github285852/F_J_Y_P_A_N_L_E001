#ifndef __SCENE_H
#define __SCENE_H



#define EXFile_MAX_LEN		20   //�ļ������
#define EXFile_MAX_NUM		10   //��������ļ�����

#define FRAME_MIN_T			10  //��С���ʱ��

unsigned char EXFile_Browser(char* path,char *ex_name,char *name);

void ImportInit(void);
void ImportTask(void);
void brower_scene_file(void);


#endif


