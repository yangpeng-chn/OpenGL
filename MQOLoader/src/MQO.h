#include "PNG.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <gl\gl.h>	
#include <gl\glu.h>	
#include <gl\glaux.h>
using namespace std;
//3�̃x�N�g��
struct Vector3f{
	float x;
	float y;
	float z;
}vec3d;
//4�̃x�N�g��
struct Vector4f{
	float x;
	float y;
	float z;
	float w;
}vec4d;
//4�̃J���[
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
//UV���W
struct UV{
	float u;//u�l
	float v;//v�l
}vec2d;
struct Vector4I{
	int x;
	int y;
	int z;
	int w;
};
//�|���S���f�[�^
struct Triangle{
	Vector3f TriVer;
	Vector3f TriNor;
	UV TriUV;
}Tri;
//�|���S���f�[�^
struct Quadrangle{
	Vector3f QuadVer;
	Vector3f QuadNor;
	UV QuadUV;
}Quad;
//�}�e���A���\����
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
}mtl;
//���f���N���X
class MODEL{
protected:
	bool LoadMaterialFromFile(char* FileName);//�}�e���A�����[�h
	vector <MATERIAL> Material;
	vector <TEXTURE*> TexData;//�e�N�X�`���f�[�^
	vector<GLuint> TexID;//�e�N�X�`��ID
	GLuint TexID2;
	TEXTURE* tex;
	//GLuint	texture[1];
	AUX_RGBImageRec *TextureImage[1];
	GLuint textu;
public:
	MODEL();
	MODEL(char* FileName);//�R���X�g���N�^
	bool OBJ_Load(char* FileName);//���[�h
	void Draw();
	AUX_RGBImageRec *LoadBMP(char *Filename);
	int LoadGLTextures();
	GLuint LoadTexture( const char * filename );
};
MODEL::MODEL(){
}
MODEL::MODEL(char* FileName){
	//LoadGLTextures();
	textu = LoadTexture("model/1.bmp");
	OBJ_Load(FileName);
}


GLuint MODEL::LoadTexture( const char * filename )
{

	GLuint texture;

	int width, height;

	unsigned char * data;

	FILE * file;

	file = fopen( filename, "rb" );

	if ( file == NULL ) return 0;
	width = 1024;
	height = 512;
	data = (unsigned char *)malloc( width * height * 3 );
	//int size = fseek(file,);
	fread( data, width * height * 3, 1, file );
	fclose( file );

	for(int i = 0; i < width * height ; ++i)
	{
		int index = i*3;
		unsigned char B,R;
		B = data[index];
		R = data[index+2];

		data[index] = R;
		data[index+2] = B;

	}


	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
	free( data );

	return texture;
}
AUX_RGBImageRec * MODEL::LoadBMP(char *Filename)
{
	FILE *File=NULL;							

	if (!Filename)								
	{
		return NULL;							
	}

	File=fopen(Filename,"r");					

	if (File)									
	{
		fclose(File);							
		return auxDIBImageLoad(Filename);		
	}

	return NULL;								
}

int MODEL::LoadGLTextures()
{
	int Status=FALSE;									// Status Indicator״ָ̬ʾƁE

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture���������Ĵ洢�ռ�E


	memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL���ͼ���¼��ȷ��������Ϊ��

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0]=LoadBMP("model/1.bmp"))  //������ȁE�ͼ��������ת��Ϊ����E����ɹ�
	{
		Status=TRUE;									// Set The Status To TRUE

		//glGenTextures(1, &texture[0]);					// Create The Texture //����һ���������� ��������E

		// Typical Texture Generation Using Data From The Bitmap
		//glBindTexture(GL_TEXTURE_2D, texture[0]); 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//��С�ñ�ԭʼ�����
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//�Ŵ�ñ�ԭʼ������ЁE  GL_LINEARΪ���õ��˲���ʽ
	}

	if (TextureImage[0])									// If Texture Exists//֮ǰ��λͼ
	{
		if (TextureImage[0]->data)							// If Texture Image Exists �����ρE
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return Status;						
}
//�`��
void MODEL::Draw(){
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	for(int i=0;i<(signed)Material.size();i++){
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(const GLfloat *)&Material[i].MaterialColor.ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(const GLfloat *)&Material[i].MaterialColor.diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(const GLfloat *)&Material[i].MaterialColor.specular);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material[i].Shininess);
		if(Material[i].TexNo>0){
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, TexID[Material[i].TexNo-1]);
			glBindTexture(GL_TEXTURE_2D, texture[0]); 
		}else{
			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if(Material[i].Tridata.size()>1){
			glVertexPointer(3, GL_FLOAT,sizeof(Tri) , &Material[i].Tridata[0].TriVer.x);
			glNormalPointer(GL_FLOAT,sizeof(Tri),&Material[i].Tridata[0].TriNor.x);
			if(Material[i].TexNo>0)
				glTexCoordPointer(2, GL_FLOAT, sizeof(Tri), &Material[i].Tridata[0].TriUV.u);
			glDrawArrays(GL_TRIANGLES,0,Material[i].Tridata.size());
		}
		if(Material[i].Quaddata.size()>1){
			glVertexPointer(3, GL_FLOAT,sizeof(Quad) , &Material[i].Quaddata[0].QuadVer.x);
			glNormalPointer(GL_FLOAT,sizeof(Quad),&Material[i].Quaddata[0].QuadNor.x);
			if(Material[i].TexNo>0)
				glTexCoordPointer(2, GL_FLOAT, sizeof(Quad), &Material[i].Quaddata[0].QuadUV.u);
			glDrawArrays(GL_QUADS,0,Material[i].Quaddata.size());
		}
		glPopMatrix();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Bufferһֱ��
	glLoadIdentity();									// Reset The View //һֱ��
	glTranslatef(0.0f,0.0f,-5.0f);  //��ȁE�Ļ5����λ

	

	glBindTexture(GL_TEXTURE_2D, textu); //ѡ������E

	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	// Bottom Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();

	
			
}

//OBJ�t�@�C���ǂݍ���
bool MODEL::OBJ_Load(char* FileName){
	Vector4I Face[3];//�ꎞ����p
	vector <Vector3f> Vertex;//���_
	vector <Vector3f> Normal;//�@��
	vector <UV> uv;//UV

	int Material_No=0;
	char key[255]={0};
	//OBJ�t�@�C�����J���ē��e��ǂݍ���
	FILE* fp=NULL;
	//fopen_s(&fp,FileName,"rt");
	if(fopen_s(&fp,FileName, "rt")!=0){return false;}
	//�ǂݍ���
	fseek(fp,SEEK_SET,0);

	while(!feof(fp))
	{
		//�L�[���[�h
		memset(key,0,sizeof(key));
		//ZeroMemory(key,sizeof(key));
		fscanf_s(fp,"%s ",key,sizeof(key));
		//�}�e���A��
		if(strcmp(key,"mtllib")==0){
			fscanf_s(fp,"%s ",key,sizeof(key));
			LoadMaterialFromFile(key);
		}
		//���_
		if(strcmp(key,"v")==0){
			fscanf_s(fp,"%f %f %f",&vec3d.x,&vec3d.y,&vec3d.z);
			Vertex.push_back(vec3d);
		}
		//�@��
		if(strcmp(key,"vn")==0){ 
			fscanf_s(fp,"%f %f %f",&vec3d.x,&vec3d.y,&vec3d.z);
			Normal.push_back(vec3d);
		}
		//�e�N�X�`��
		if(strcmp(key,"vt")==0){
			fscanf_s(fp,"%f %f",&vec2d.u,&vec2d.v);
			uv.push_back(vec2d);
		}
		//�}�e���A���Z�b�g
		if(strcmp(key,"usemtl")==0){
			fscanf_s(fp,"%s ",key,sizeof(key));
			for(int i=0;i<(signed)Material.size();i++){
				if(strcmp(key,Material[i].MaterialName.c_str())==0)Material_No=i;
			}
		}
		//�ʂ̃C���f�b�N�X 0=���_ 1=UV 2=�@��
		if(strcmp(key,"f")==0){
			Face[0].w=-1;
			Face[1].w=-1;
			Face[2].w=-1;
			fscanf_s(fp,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",&Face[0].x,&Face[1].x,&Face[2].x,&Face[0].y,&Face[1].y,&Face[2].y,&Face[0].z,&Face[1].z,&Face[2].z,&Face[0].w,&Face[1].w,&Face[2].w);
			if((Face[0].w == -1) && (Face[1].w == -1) && (Face[2].w == -1)){
				//�O�p��
				Material[Material_No].TriVerID.push_back(Face[0].x-1);
				Material[Material_No].TriVerID.push_back(Face[0].y-1);
				Material[Material_No].TriVerID.push_back(Face[0].z-1);
				Material[Material_No].TriUVID.push_back(Face[1].x-1);
				Material[Material_No].TriUVID.push_back(Face[1].y-1);
				Material[Material_No].TriUVID.push_back(Face[1].z-1);
				Material[Material_No].TriNorID.push_back(Face[2].x-1);
				Material[Material_No].TriNorID.push_back(Face[2].y-1);
				Material[Material_No].TriNorID.push_back(Face[2].z-1);
			}else{
				//�l�p��
				Material[Material_No].QuadVerID.push_back(Face[0].x-1);
				Material[Material_No].QuadVerID.push_back(Face[0].y-1);
				Material[Material_No].QuadVerID.push_back(Face[0].z-1);
				Material[Material_No].QuadVerID.push_back(Face[0].w-1);
				Material[Material_No].QuadUVID.push_back(Face[1].x-1);
				Material[Material_No].QuadUVID.push_back(Face[1].y-1);
				Material[Material_No].QuadUVID.push_back(Face[1].z-1);
				Material[Material_No].QuadUVID.push_back(Face[1].w-1);
				Material[Material_No].QuadNorID.push_back(Face[2].x-1);
				Material[Material_No].QuadNorID.push_back(Face[2].y-1);
				Material[Material_No].QuadNorID.push_back(Face[2].z-1);
				Material[Material_No].QuadNorID.push_back(Face[2].w-1);
			}
		}
	}
	for(int j=0;j<(signed)Material.size();j++){
		for(int i=0;i<(signed)Material[j].TriVerID.size();i++){
			Tri.TriVer=Vertex[Material[j].TriVerID[i]];
			Tri.TriNor=Normal[Material[j].TriNorID[i]];
			Tri.TriUV=uv[Material[j].TriUVID[i]];
			Material[j].Tridata.push_back(Tri);
		}
		for(int i=0;i<(signed)Material[j].QuadVerID.size();i++){
			Quad.QuadVer=Vertex[Material[j].QuadVerID[i]];
			Quad.QuadNor=Normal[Material[j].QuadNorID[i]];
			Quad.QuadUV=uv[Material[j].QuadUVID[i]];
			Material[j].Quaddata.push_back(Quad);
		}
		Material[j].TriVerID.clear();
		Material[j].TriNorID.clear();
		Material[j].TriUVID.clear();
		Material[j].QuadVerID.clear();
		Material[j].QuadNorID.clear();
		Material[j].QuadUVID.clear();
	}
	Vertex.clear();
	Normal.clear();
	uv.clear();
	return true;
}
bool MODEL::LoadMaterialFromFile(char* FileName){
	FILE* fp=NULL;
	//fopen_s(&fp,FileName,"rt");
	if(fopen_s(&fp,"model/ground.mtl", "rt")!=0){return false;}
	char key[255]={0};
	bool flag=false;
	bool flag2=false;
	memset(&vec4d,0,sizeof(Vector4f));
	//ZeroMemory(&vec4d,sizeof(Vector4f));
	vec4d.w=1.0f;
	mtl.MaterialColor.emission=(const Color4 &)vec4d;
	mtl.Shininess=0.0f;
	mtl.TexNo=0;

	fseek(fp,SEEK_SET,0);

	while(!feof(fp))
	{
		//�L�[���[�h�ǂݍ���
		fscanf_s(fp,"%s ",key,sizeof(key));
		//�}�e���A����
		if(strcmp(key,"newmtl")==0)
		{
			if(flag){Material.push_back(mtl);mtl.TexNo=0;}
			flag=true;
			fscanf_s(fp,"%s ",key,sizeof(key));
			mtl.MaterialName=key;
			flag2=false;
		}
		//Ka�@�A���r�G���g
		if(strcmp(key,"Ka")==0)
		{
			fscanf_s(fp,"%f %f %f",&vec4d.x,&vec4d.y,&vec4d.z);
			mtl.MaterialColor.ambient=(const Color4 &)vec4d;
		}
		//Kd�@�f�B�t���[�Y
		if(strcmp(key,"Kd")==0)
		{
			fscanf_s(fp,"%f %f %f",&vec4d.x,&vec4d.y,&vec4d.z);
			mtl.MaterialColor.diffuse=(const Color4 &)vec4d;
		}
		//Ks�@�X�y�L�����[
		if(strcmp(key,"Ks")==0)
		{
			fscanf_s(fp,"%f %f %f",&vec4d.x,&vec4d.y,&vec4d.z);
			mtl.MaterialColor.specular=(const Color4 &)vec4d;
		}
		//SHININESS
		if(strcmp(key,"Ns")==0)
		{
			fscanf_s(fp,"%f",&vec4d.x);
			mtl.Shininess=vec4d.x;
		}
		//map_Kd�@�e�N�X�`��
		if(strcmp(key,"map_Kd")==0)
		{
			fscanf_s(fp,"%s ",key,sizeof(key));
			for(int i=0;i<(signed)Material.size();i++){
				if(strcmp(key,Material[i].TextureName.c_str())==0){
					flag2=true;
					mtl.TexNo=Material[i].TexNo;
					break;
				}
			}
			if(flag2){
			}else{
				mtl.TextureName=key;
				//�e�N�X�`�����쐬
				TexData.push_back(tex);
				TexData[TexData.size()-1] = new TEXTURE(mtl.TextureName.c_str());;
				mtl.TexNo=TexData.size();
				TexID.push_back(TexID2);
				/*glGenTextures(1, (GLuint *)&TexID[TexData.size()-1]);
				glBindTexture(GL_TEXTURE_2D, texture[0]); 
				//glBindTexture(GL_TEXTURE_2D, TexID[TexData.size()-1]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glEnable(GL_TEXTURE_2D);
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,TexData[TexData.size()-1]->Width, TexData[TexData.size()-1]->Height,
					//0,GL_RGBA, GL_UNSIGNED_BYTE, TexData[TexData.size()-1]->image);
				glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
				glDisable(GL_TEXTURE_2D);*/
			}
		}
	}
	fclose(fp);
	if(flag)Material.push_back(mtl);
	return true;
}