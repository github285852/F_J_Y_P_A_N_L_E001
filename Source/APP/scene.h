#ifndef __SCENE_H
#define __SCENE_H



#define EXFile_MAX_LEN		20   //�ļ������
#define EXFile_MAX_NUM		10   //��������ļ�����



uint8_t EXFile_Browser(char* path,char *ex_name,char *name);

void ImportInit(void);
void ImportTask(void);
void brower_scene_file(void);
void SceneFlashTask(void);
void SceneFlashInit(void);
void SceneBack(void);



#endif


