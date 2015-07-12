#include "OBJLoader.h"
AUX_RGBImageRec* COBJLoader::LoadBMP(const char *Filename)
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


bool COBJLoader::Load2DTextures(const char *Filename,vector<GLuint>& _texID,vector<AUX_RGBImageRec *>& _texImg, int _mitNO,bool _BMPOnly)
{
	static int counter = -1;
	static int lastMitNO = _mitNO;
	if (lastMitNO == _mitNO)
	{
		counter++;
	}

	bool Status=FALSE;			
	_texID.resize(counter+1);

	if (_BMPOnly)
	{
		_texImg.resize(counter+1);
		if (_texImg[counter]=LoadBMP(Filename)) 
		{
			Status=TRUE;									
			glGenTextures(1, &_texID[counter]);					
			glBindTexture(GL_TEXTURE_2D, _texID[counter]); 
			glTexImage2D(GL_TEXTURE_2D, 0, 3, _texImg[counter]->sizeX, _texImg[counter]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, _texImg[counter]->data);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			printf("texture [%s]	ID [%d]\n",Filename,_texID[counter]);

		}
	}

	else
	{
		if( _texID[counter] = SOIL_load_OGL_texture
			(
			Filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			//SOIL_FLAG_POWER_OF_TWO
			SOIL_FLAG_INVERT_Y
			//| SOIL_FLAG_MIPMAPS
			//| SOIL_FLAG_MULTIPLY_ALPHA
			//| SOIL_FLAG_COMPRESS_TO_DXT
			//| SOIL_FLAG_DDS_LOAD_DIRECT
			//| SOIL_FLAG_NTSC_SAFE_RGB
			//| SOIL_FLAG_CoCg_Y
			//| SOIL_FLAG_TEXTURE_RECTANGLE
			))
		{
			Status=TRUE;
			
			//can use before draw instead of here
			/*glEnable( GL_TEXTURE_2D );//if enable here, the video is dark
			glBindTexture( GL_TEXTURE_2D, _texID[counter] );
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);*/
			printf("texture [%s]	ID [%d]\n",Filename,_texID[counter]);
		}
	}

	return Status;
}
COBJLoader::COBJLoader()
{
	cout << "COBJLoader Created!"<<endl;
}
COBJLoader::COBJLoader(char* FileName)
{
	cout << "COBJLoader Created!"<<endl;
	Load(FileName);
}
//描画
void COBJLoader::Draw()//implemented in GLPainter, not called again
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	for(int i=0;i<(signed)m_vecLocalMtl.size();i++)
	{
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(const GLfloat *)&m_vecLocalMtl[i].MaterialColor.ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(const GLfloat *)&m_vecLocalMtl[i].MaterialColor.diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(const GLfloat *)&m_vecLocalMtl[i].MaterialColor.specular);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,m_vecLocalMtl[i].Shininess);
		if(m_vecLocalMtl[i].TexNo>0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_vecLocalMtl[i].vecTexture[m_vecLocalMtl[i].TexNo-1]); 
		}

		else
		{
			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if(m_vecLocalMtl[i].Tridata.size()>1)
		{
			glVertexPointer(3, GL_FLOAT,sizeof(Triangle) , &m_vecLocalMtl[i].Tridata[0].TriVer.x);
			glNormalPointer(GL_FLOAT,sizeof(Triangle),&m_vecLocalMtl[i].Tridata[0].TriNor.x);
			if(m_vecLocalMtl[i].TexNo>0)
				glTexCoordPointer(2, GL_FLOAT, sizeof(Triangle), &m_vecLocalMtl[i].Tridata[0].TriUV.u);
			glDrawArrays(GL_TRIANGLES,0,m_vecLocalMtl[i].Tridata.size());
		}

		if(m_vecLocalMtl[i].Quaddata.size()>1)
		{
			glVertexPointer(3, GL_FLOAT,sizeof(Quadrangle) , &m_vecLocalMtl[i].Quaddata[0].QuadVer.x);
			glNormalPointer(GL_FLOAT,sizeof(Quadrangle),&m_vecLocalMtl[i].Quaddata[0].QuadNor.x);
			if(m_vecLocalMtl[i].TexNo>0)
				glTexCoordPointer(2, GL_FLOAT, sizeof(Quadrangle), &m_vecLocalMtl[i].Quaddata[0].QuadUV.u);
			glDrawArrays(GL_QUADS,0,m_vecLocalMtl[i].Quaddata.size());
		}
		glPopMatrix();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

//OBJファイル読み込み
bool COBJLoader::Load(char* FileName)
{
	printf("Loading %s...\n",FileName);
	Vector4I Face[3];//一時代入用
	vector <Vector3f> Vertex;//頂点
	vector <Vector3f> Normal;//法線
	vector <UV> uv;//UV

	int Material_No=0;
	char key[255]={0};
	//OBJファイルを開いて内容を読み込む
	FILE* fp=NULL;
	fopen_s(&fp,FileName,"rt");
	if (!fp)
	{
		printf("load OBJ file %s failed\n",FileName);
		return false;
	}
	//読み込み
	fseek(fp,SEEK_SET,0);

	while(!feof(fp))
	{
		//キーワード
		ZeroMemory(key,sizeof(key));
		fscanf_s(fp,"%s ",key,sizeof(key));
		//マテリアル
		if(strcmp(key,"mtllib")==0){
			fscanf_s(fp,"%s ",key,sizeof(key));
			LoadMaterialFromFile(key);
		}
		//頂点
		if(strcmp(key,"v")==0){
			fscanf_s(fp,"%f %f %f",&m_vec3d.x,&m_vec3d.y,&m_vec3d.z);
			Vertex.push_back(m_vec3d);
		}
		//法線
		if(strcmp(key,"vn")==0){ 
			fscanf_s(fp,"%f %f %f",&m_vec3d.x,&m_vec3d.y,&m_vec3d.z);
			Normal.push_back(m_vec3d);
		}
		//テクスチャ
		if(strcmp(key,"vt")==0){
			fscanf_s(fp,"%f %f",&m_vec2d.u,&m_vec2d.v);
			uv.push_back(m_vec2d);
		}
		//マテリアルセット
		if(strcmp(key,"usemtl")==0)
		{
			fscanf_s(fp,"%s ",key,sizeof(key));

			for(int i=0; i < (signed)m_vecLocalMtl.size(); i++)
			{
				if(strcmp(key,m_vecLocalMtl[i].MaterialName.c_str())==0)
					Material_No=i;
			}
		}
		//面のインデックス 0=頂点 1=UV 2=法線
		if(strcmp(key,"f")==0){
			Face[0].w=-1;
			Face[1].w=-1;
			Face[2].w=-1;
			fscanf_s(fp,"%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",&Face[0].x,&Face[1].x,&Face[2].x,&Face[0].y,&Face[1].y,&Face[2].y,&Face[0].z,&Face[1].z,&Face[2].z,&Face[0].w,&Face[1].w,&Face[2].w);
			if((Face[0].w == -1) && (Face[1].w == -1) && (Face[2].w == -1)){
				//三角面
				m_vecLocalMtl[Material_No].TriVerID.push_back(Face[0].x-1);
				m_vecLocalMtl[Material_No].TriVerID.push_back(Face[0].y-1);
				m_vecLocalMtl[Material_No].TriVerID.push_back(Face[0].z-1);
				m_vecLocalMtl[Material_No].TriUVID.push_back(Face[1].x-1);
				m_vecLocalMtl[Material_No].TriUVID.push_back(Face[1].y-1);
				m_vecLocalMtl[Material_No].TriUVID.push_back(Face[1].z-1);
				m_vecLocalMtl[Material_No].TriNorID.push_back(Face[2].x-1);
				m_vecLocalMtl[Material_No].TriNorID.push_back(Face[2].y-1);
				m_vecLocalMtl[Material_No].TriNorID.push_back(Face[2].z-1);
			}else{
				//四角面
				m_vecLocalMtl[Material_No].QuadVerID.push_back(Face[0].x-1);
				m_vecLocalMtl[Material_No].QuadVerID.push_back(Face[0].y-1);
				m_vecLocalMtl[Material_No].QuadVerID.push_back(Face[0].z-1);
				m_vecLocalMtl[Material_No].QuadVerID.push_back(Face[0].w-1);
				m_vecLocalMtl[Material_No].QuadUVID.push_back(Face[1].x-1);
				m_vecLocalMtl[Material_No].QuadUVID.push_back(Face[1].y-1);
				m_vecLocalMtl[Material_No].QuadUVID.push_back(Face[1].z-1);
				m_vecLocalMtl[Material_No].QuadUVID.push_back(Face[1].w-1);
				m_vecLocalMtl[Material_No].QuadNorID.push_back(Face[2].x-1);
				m_vecLocalMtl[Material_No].QuadNorID.push_back(Face[2].y-1);
				m_vecLocalMtl[Material_No].QuadNorID.push_back(Face[2].z-1);
				m_vecLocalMtl[Material_No].QuadNorID.push_back(Face[2].w-1);
			}
		}
	}
	for(int j=0;j<(signed)m_vecLocalMtl.size();j++){
		for(int i=0;i<(signed)m_vecLocalMtl[j].TriVerID.size();i++){
			m_tri.TriVer=Vertex[m_vecLocalMtl[j].TriVerID[i]];
			m_tri.TriNor=Normal[m_vecLocalMtl[j].TriNorID[i]];
			m_tri.TriUV=uv[m_vecLocalMtl[j].TriUVID[i]];
			m_vecLocalMtl[j].Tridata.push_back(m_tri);
		}
		for(int i=0;i<(signed)m_vecLocalMtl[j].QuadVerID.size();i++){
			m_quad.QuadVer=Vertex[m_vecLocalMtl[j].QuadVerID[i]];
			m_quad.QuadNor=Normal[m_vecLocalMtl[j].QuadNorID[i]];
			m_quad.QuadUV=uv[m_vecLocalMtl[j].QuadUVID[i]];
			m_vecLocalMtl[j].Quaddata.push_back(m_quad);
		}
		m_vecLocalMtl[j].TriVerID.clear();
		m_vecLocalMtl[j].TriNorID.clear();
		m_vecLocalMtl[j].TriUVID.clear();
		m_vecLocalMtl[j].QuadVerID.clear();
		m_vecLocalMtl[j].QuadNorID.clear();
		m_vecLocalMtl[j].QuadUVID.clear();
	}
	Vertex.clear();
	Normal.clear();
	uv.clear();
	printf("%s Loaded!\n",FileName);
	return true;
}
bool COBJLoader::LoadMaterialFromFile(char* FileName)
{
	SMaterial mtl;
	FILE* fp=NULL;
	//fopen_s(&fp,FileName,"rt");
	char filePath[256] = "../model/";
	strcat(filePath,FileName);
	fopen_s(&fp,filePath,"rt");
	if (!fp)
	{
		printf("load m_vecLocalMtl file %s failed\n",filePath);
		return false;
	}
	char key[255]={0};
	bool flag=false;
	bool flag2=false;
	ZeroMemory(&m_vec4d,sizeof(Vector4f));
	m_vec4d.w=1.0f;
	mtl.MaterialColor.emission=(const Color4 &)m_vec4d;
	mtl.Shininess=0.0f;
	mtl.TexNo=0;

	fseek(fp,SEEK_SET,0);

	while(!feof(fp))
	{
		fscanf_s(fp,"%s ",key,sizeof(key));
		if(strcmp(key,"newmtl")==0)
		{
			if(flag)
			{
				m_vecLocalMtl.push_back(mtl);
				mtl.TexNo=0;
				mtl.vecTexture.resize(0);
				mtl.vecTextureImg.resize(0);
				mtl.TextureName = "";
			}
			flag=true;
			fscanf_s(fp,"%s ",key,sizeof(key));
			mtl.MaterialName=key;
			flag2=false;
		}

		//Ka　アンビエント
		if(strcmp(key,"Ka")==0)
		{
			fscanf_s(fp,"%f %f %f",&m_vec4d.x,&m_vec4d.y,&m_vec4d.z);
			mtl.MaterialColor.ambient=(const Color4 &)m_vec4d;
		}

		//Kd　ディフューズ
		if(strcmp(key,"Kd")==0)
		{
			fscanf_s(fp,"%f %f %f",&m_vec4d.x,&m_vec4d.y,&m_vec4d.z);
			mtl.MaterialColor.diffuse=(const Color4 &)m_vec4d;
		}

		//Ks　スペキュラー
		if(strcmp(key,"Ks")==0)
		{
			fscanf_s(fp,"%f %f %f",&m_vec4d.x,&m_vec4d.y,&m_vec4d.z);
			mtl.MaterialColor.specular=(const Color4 &)m_vec4d;
		}

		//SHININESS
		if(strcmp(key,"Ns")==0)
		{
			fscanf_s(fp,"%f",&m_vec4d.x);
			mtl.Shininess=m_vec4d.x;
		}

		//map_Kd　テクスチャ
		if(strcmp(key,"map_Kd")==0)
		{
			int mitNO = m_vecLocalMtl.size();

			fscanf_s(fp,"%s ",key,sizeof(key));
			mtl.TextureName = key;
			char texturePath[100] = "../model/";
			strcat(texturePath,mtl.TextureName.c_str());
			mtl.TexNo++;

			if (!Load2DTextures(texturePath,mtl.vecTexture,mtl.vecTextureImg,mitNO,false))
			{
				printf("load texture %s failed\n",texturePath);
			}

		}
	}
	fclose(fp);

	if(flag)
		m_vecLocalMtl.push_back(mtl);

	return true;
}