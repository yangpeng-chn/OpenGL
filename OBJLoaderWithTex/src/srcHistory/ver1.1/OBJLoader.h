#ifndef OBJLOADER_H
#define OBJLOADER_H
//#include "PNG.h"
#include <GL\glut.h>
#include <gl\gl.h>	
#include <gl\glu.h>	
#include <gl\glaux.h>
#include <iostream>
#include <vector>
#include "SOIL.h"
using namespace std;

struct Color4{
	float r;
	float g;
	float b;
	float a;
};
//4�̔���
struct Reflection4{
	Color4 diffuse;
	Color4 ambient;
	Color4 emission;
	Color4 specular;
};

struct Vector4I{
	int x;
	int y;
	int z;
	int w;
};

struct Vector3f
{
	float x;
	float y;
	float z;
};
//4�̃x�N�g��
struct Vector4f
{
	float x;
	float y;
	float z;
	float w;
};

struct UV
{
	float u;//u�l
	float v;//v�l
};
struct Triangle
{
	Vector3f TriVer;
	Vector3f TriNor;
	UV TriUV;
};
struct Quadrangle
{
	Vector3f QuadVer;
	Vector3f QuadNor;
	UV QuadUV;
};
struct MATERIAL{
	string MaterialName;//�}�e���A����
	Reflection4 MaterialColor;//����
	float Shininess;//shininess
	string TextureName;//�e�N�X�`����
	int TexNo;//�e�N�X�`��NO.
	vector <Triangle> Tridata;//�O�p�ʃf�[�^
	vector <Quadrangle> Quaddata;//�l�p�ʃf�[�^
	vector <unsigned int> TriVerID;//�e��C���f�b�N�X
	vector <unsigned int> TriNorID;
	vector <unsigned int> TriUVID;
	vector <unsigned int> QuadVerID;
	vector <unsigned int> QuadNorID;
	vector <unsigned int> QuadUVID;

	vector<GLuint> vecTexture;
	vector <AUX_RGBImageRec *> vecTextureImg;
};

//���f���N���X
class MODEL{
protected:
	bool LoadMaterialFromFile(char* FileName);//�}�e���A�����[�h
	Vector3f vec3d;
	Vector4f vec4d;
	UV vec2d;
	Triangle Tri;
	Quadrangle Quad;
	vector <MATERIAL> Material;
	//vector <TEXTURE*> TexData;//�e�N�X�`���f�[�^
	//vector<GLuint> vecTexture;//�e�N�X�`��ID
	//GLuint TexID2;
	//TEXTURE* tex;
	
	//vector<GLuint> texture;
public:
	MODEL();
	MODEL(char* FileName);//�R���X�g���N�^
	bool OBJ_Load(char* FileName);//���[�h
	void Draw();
	AUX_RGBImageRec *LoadBMP(const char *Filename);
	bool LoadBMPTextures(const char *Filename,vector<GLuint>& _texID,vector <AUX_RGBImageRec *>& _texImg, int _mitNO);
};

#endif
