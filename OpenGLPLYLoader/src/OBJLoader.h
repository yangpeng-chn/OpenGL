#ifndef OBJLOADER_H
#define OBJLOADER_H
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
//4つの反射
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
//4つのベクトル
struct Vector4f
{
	float x;
	float y;
	float z;
	float w;
};

struct UV
{
	float u;//u値
	float v;//v値
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
struct SMaterial
{
	string MaterialName;//マテリアル名
	Reflection4 MaterialColor;//反射
	float Shininess;//shininess
	string TextureName;//テクスチャ名
	int TexNo;//テクスチャNO.
	vector <Triangle> Tridata;//三角面データ
	vector <Quadrangle> Quaddata;//四角面データ
	vector <unsigned int> TriVerID;//各種インデックス
	vector <unsigned int> TriNorID;
	vector <unsigned int> TriUVID;
	vector <unsigned int> QuadVerID;
	vector <unsigned int> QuadNorID;
	vector <unsigned int> QuadUVID;

	vector<GLuint> vecTexture;
	vector <AUX_RGBImageRec *> vecTextureImg;
};

//モデルクラス
class COBJLoader
{
protected:
	bool LoadMaterialFromFile(char* FileName);//マテリアルロード
	Vector3f m_vec3d;
	Vector4f m_vec4d;
	UV m_vec2d;
	Triangle m_tri;
	Quadrangle m_quad;
	vector <SMaterial> m_vecLocalMtl; //need to send
	vector <SMaterial> m_vecRemoteMtl;
public:
	COBJLoader();
	COBJLoader(char* FileName);//コンストラクタ
	bool Load(char* FileName);//ロード
	void Draw();
	AUX_RGBImageRec *LoadBMP(const char *Filename);
	bool LoadBMPTextures(const char *Filename,vector<GLuint>& _texID,vector <AUX_RGBImageRec *>& _texImg, int _mitNO);
	bool Load2DTextures(const char *Filename,vector<GLuint>& _texID,vector <AUX_RGBImageRec *>& _texImg, int _mitNO,bool _BMPOnly);
	const vector <SMaterial>& GetLocalMaterialInfo(){return m_vecLocalMtl;};
	vector <SMaterial>& GetRemoteMaterialInfo(){return m_vecRemoteMtl;};
	bool RemoteMtlEmpty(){return m_vecRemoteMtl.empty();};

};

#endif
