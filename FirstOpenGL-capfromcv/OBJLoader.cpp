/**********************************************************************
�@�@OBJLoader.cpp

�@�@Alias Wavefornt OBJ File Loader

�@�@Date : Nov. 18, 2007
�@�@Version : 1.0
�@�@Author : Pocol
**********************************************************************/

//
//�@include
//
#include <fstream>
#include "OBJLoader.h"
using namespace std;


//--------------------------------------------------------------------------------------------------
//�@�@GetDirectoryName
//�@�@Desc : �����񂩂�f�B���N�g�����擾
//--------------------------------------------------------------------------------------------------
char * GetDirectoryName(const char *filename, char *dest)
{
	char *strings = NULL;
	char *dir;
	dir = new char [strlen(filename)+1];
	strcpy(dir, filename);
	if ( strings = strrchr(dir, '/' ) ) strings[1] = '\0';
	else if ( strings = strrchr(dir, '\\') ) strings[1] ='\0';
	else dir[0] = '\0';
	strcpy(dest, dir);
	return dest;
}

//---------------------------------------------------------------------------------------------------
//�@�@SetDirectory
//�@�@Desc : dest�̑O�Ƀf�B���N�g����t�����C�������Ԃ�
//--------------------------------------------------------------------------------------------------
char * SetDirectoryName( char *dest, char *directory )
{
	char *s = "";
	s = new char [strlen(directory) + strlen(dest) + 1];
	strcpy( s, directory );
	strcat( s, dest );
	return s;
}

////////////////////////////////////////////////////////////////////////
//�@�@OBJMesh class
////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//�@�@OBJMesh
//�@�@Desc : �R���X�g���N�^
//-------------------------------------------------------------------------------------------------
OBJMesh::OBJMesh()
{
	num_vertex = 0;
	num_normal = 0;
	num_face = 0;
	num_material = 0;
	listID = 0;
}

//-------------------------------------------------------------------------------------------------
//�@�@~OBJMesh
//�@�@Desc : �f�X�g���N�^
//-------------------------------------------------------------------------------------------------
OBJMesh::~OBJMesh()
{
}

//-------------------------------------------------------------------------------------------------
//�@�@Release
//�@�@Desc : �����������
//-------------------------------------------------------------------------------------------------
void OBJMesh::Release()
{
	if ( vertex )
	{
		free(vertex);
		vertex = NULL;
	}
	if ( normal )
	{
		free(normal);
		normal = NULL;
	}
	if ( face )
	{
		free(face);
		face = NULL;
	}
	if ( material )
	{
		free(material);
		material = NULL;
	}
	if ( listID != 0 )
	{
		glDeleteLists(listID, 1);
		listID = 0;
	}
	num_vertex = 0;
	num_normal = 0;
	num_face = 0;
	num_material = 0;
}

//--------------------------------------------------------------------------------------------------
//�@�@AddVertex
//�@�@Desc : ���_���W��ǉ�
//--------------------------------------------------------------------------------------------------
void OBJMesh::AddVertex(OBJVertex &vert)
{
	num_vertex++;
	vertex = (OBJVertex*)realloc(vertex, num_vertex*sizeof(OBJVertex));
	vertex[num_vertex-1] = vert;
}

//-------------------------------------------------------------------------------------------------
//�@�@AddNormal
//�@�@Desc : �@���x�N�g����ǉ�
//-------------------------------------------------------------------------------------------------
void OBJMesh::AddNormal(OBJVertex &norm)
{
	num_normal++;
	normal = (OBJVertex*)realloc(normal, num_normal*sizeof(OBJVertex));
	normal[num_normal-1] = norm;
}

//-------------------------------------------------------------------------------------------------
//�@�@AddFace
//�@�@Desc : �ʂ�ǉ�
//-------------------------------------------------------------------------------------------------
void OBJMesh::AddFace(OBJFace &surf)
{
	num_face++;
	face = (OBJFace*)realloc(face, num_face*sizeof(OBJFace));
	face[num_face-1] = surf;
}

//-------------------------------------------------------------------------------------------------
//�@�@AddMaterial
//�@�@Desc : �}�e���A����ǉ�
//-------------------------------------------------------------------------------------------------
void OBJMesh::AddMaterial(OBJMaterial &mat)
{
	num_material++;
	material = (OBJMaterial*)realloc(material, num_material*sizeof(OBJMaterial));
	material[num_material-1] = mat;
}

//-------------------------------------------------------------------------------------------------
//�@�@Information
//�@�@Desc : ����\��
//-------------------------------------------------------------------------------------------------
void OBJMesh::Information()
{
	cout << "* OBJ�t�@�C�����F" << objFileName << endl;
	cout << "* MTL�t�@�C�����F" << mtlFileName << endl;
	cout << "* ���_���F" << num_vertex << endl;
	cout << "* �@�����F" << num_normal << endl;
	cout << "* �ʁ@���F" << num_face << endl;
	cout << "* �ގ����F" << num_material << endl;
	cout << endl;
}

//-------------------------------------------------------------------------------------------------
//�@�@LoadOBJFile
//�@�@Desc : OBJ�t�@�C���̓ǂݍ���
//-------------------------------------------------------------------------------------------------
bool OBJMesh::LoadOBJFile(const char *filename)
{
	ifstream file;
	int cmi = 0;	
	char tmp_char[OBJ_NAME_LENGTH];
	char buf[OBJ_BUFFER_LENGTH];
	char *pbuf;
	float min_size = 0.0;
	float max_size = 0.0;
	bool size_flag = false;
	
	//�@�I�u�W�F�N�g�t�@�C�������R�s�[
	strcpy(objFileName, filename);

	//�@�t�@�C�����J��
	file.open(filename, ios::in);
	if ( !file.is_open() )
	{
		cout << "Error : �w�肳�ꂽOBJ�t�@�C�����J���܂���ł���\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	//�@�t�@�C���̖��[�܂Ń��[�v
	while ( !file.eof() )
	{
		OBJVertex tmp_vert(0.0, 0.0, 0.0);
		OBJVertex tmp_norm(0.0, 0.0, 0.0);
		OBJFace tmp_face;
		float tmp_float=0.0;

		//�@1�s�ǂݎ��
		file.getline(buf, sizeof(buf));

		//�@�o�b�t�@��1�����ڂŔ���
		switch ( buf[0] )
		{
		case 'v':
			//�@�o�b�t�@��2�����ڂŔ���
			switch ( buf[1] )
			{
			//�@Vertex
			case ' ':
				//�@���_���W��ǂݎ��
				if ( sscanf(buf+2, "%f %f %f %f", &tmp_vert.x, &tmp_vert.y, &tmp_vert.z, &tmp_float) != 4 )
				{
					if ( sscanf(buf+2, "%f %f %f", &tmp_vert.x, &tmp_vert.y, &tmp_vert.z) != 3 )
					{
						cout << "Error : ���_���W�̐����s���ł�\n";
						return false;
					}
				}
				//�@�����l�̐ݒ�
				if ( !size_flag )
				{
					min_size = tmp_vert.x;
					max_size = tmp_vert.x;
					size_flag = true;
				}
				//�@�ő�E�ŏ��̔�r
				for ( int i=0; i<3; i++ )
				{
					if ( min_size > tmp_vert.v[i] ) min_size = tmp_vert.v[i];
					if ( max_size < tmp_vert.v[i] ) max_size = tmp_vert.v[i];
				}
				//�@���_���W��ǉ�
				AddVertex(tmp_vert);
				break;

			//�@Normal
			case 'n':
				//�@�@���x�N�g���̓ǂݎ��
				if ( sscanf(buf+2, "%f %f %f", &tmp_norm.x, &tmp_norm.y, &tmp_norm.z) != 3)
				{
					cout << "Error : �@���x�N�g���̐����s���ł�\n";
					return false;
				}
				//�@�@���x�N�g����ǉ�
				AddNormal(tmp_norm);
				break;
			}
			break;

		//�@face
		case 'f':
			pbuf = buf;
			//�@�󔒂̐��ŗv�f�����������邩�J�E���g
			while ( *pbuf )
			{
				if ( *pbuf == ' ' ) tmp_face.element++;
				pbuf++;
			}
			//�@�v�f��3�����Ȃ�ʂ��\���ł��Ȃ�
			if ( tmp_face.element < 3 )
			{
				cout << "Error : �ʂ��\�����邽�߂̗v�f�����s���ł�\n";
				return false;
			}
			switch ( tmp_face.element )
			{
			//�@�O�p�`
			case 3:
				tmp_face.type = GL_TRIANGLES;
				break;

			//�@�l�p�`
			case 4:
				tmp_face.type = GL_QUADS;
				break;

			//�@���p�`
			default:
				tmp_face.type = GL_POLYGON;
				break;
			}
			//�@�C���f�b�N�X�p�̃��������m��
			tmp_face.vertex_index = new int [tmp_face.element];
			tmp_face.normal_index = new int [tmp_face.element];
			pbuf = buf;
			for ( int i=0; i<tmp_face.element; i++ )
			{
				pbuf = strchr(pbuf, ' ');
				pbuf++;

				//�@�\���v�f�̓ǂݎ��
				if ( sscanf(pbuf, "%d/%d/%d", &tmp_face.vertex_index[i], &tmp_float, &tmp_face.normal_index[i] ) != 3 )
				{
					if ( sscanf(pbuf, "%d//%d", &tmp_face.vertex_index[i], &tmp_face.normal_index[i] ) != 2 )
					{
						if ( sscanf(pbuf, "%d/%d", &tmp_face.vertex_index[i], &tmp_float ) != 2 )
						{
							sscanf(pbuf, "%d", &tmp_face.vertex_index[i]);
							tmp_face.use_normal = false;
						}
						else
						{
							tmp_face.use_normal = false;
						}
					}
					else
					{
						tmp_face.use_normal = true;
					}
				}
				else
				{
					tmp_face.use_normal = true;
				}
				//�@�z��̔ԍ��ƍ��킹��
				tmp_face.vertex_index[i]--;
				if ( tmp_face.use_normal ) tmp_face.normal_index[i]--;
			}
			//�@�}�e���A���C���f�b�N�X���i�[
			tmp_face.material_index = cmi;
			//�@�ʂ�ǉ�
			AddFace(tmp_face);
			break;

		//�@usemtl
		case 'u':
			//�@�}�e���A������ǂݎ��
			strcpy(tmp_char, " ");
			sscanf(buf, "usemtl %s", &tmp_char);
			//�@�}�e���A�������猟��
			for ( int i=0; i<num_material; i++ )
			{
				//�@���O����v������}�e���A���ԍ����i�[
				if ( strcmpi(material[i].name, tmp_char) == 0 ) cmi = i;
			}
			break;

		//�@mtllib
		case 'm':
			//�@�}�e���A���t�@�C������ǂݎ��
			strcpy(tmp_char, " ");
			sscanf(buf, "mtllib %s", &tmp_char);
			//�@�}�e���A���t�@�C���̓ǂݍ���
			if ( !LoadMTLFile(
				SetDirectoryName(tmp_char, directoryName)	//�@�f�B���N�g����t��
				))
				return false;
			break;
		
		default:
			break;
		}
	}

	//�@�T�C�Y�����p�ϐ�
	size = max_size - min_size;
	
	//�@�t�@�C�������
	file.close();

	return true;
}

//-------------------------------------------------------------------------------------------------
//�@�@LoadMTLFile
//�@�@Desc : MTL�t�@�C���̓ǂݍ���
//-------------------------------------------------------------------------------------------------
bool OBJMesh::LoadMTLFile(const char *filename)
{
	ifstream file;
	int cmi = -1;
	char buf[OBJ_BUFFER_LENGTH];
	float tmp_float=0.0f;
	OBJMaterial tmp_mat;

	//�@�}�e���A���t�@�C�������R�s�[
	strcpy(mtlFileName, filename); 

	//�@�t�@�C�����J��
	file.open(filename, ios::in);
	if ( !file.is_open() )
	{
		cout << "Error : �w�肳�ꂽMTL�t�@�C�����J���܂���ł���\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	//�@�t�@�C���̖��[�܂Ń��[�v
	while ( !file.eof() )
	{
		//�@1�s�ǂݎ��
		file.getline(buf, sizeof(buf));

		//�@�o�b�t�@��1�����ڂŔ��f
		switch ( buf[0] )
		{
		//�@newmtl
		case 'n':
			cmi++;
			if ( cmi != 0 ) AddMaterial(tmp_mat);
			sscanf(buf, "newmtl %s", tmp_mat.name);
			break;

		//�@Ka, Kd, Ks
		case 'K':
			switch ( buf[1] )
			{
			//�@Ambient
			case 'a':
				sscanf(buf, "Ka %f %f %f", &tmp_mat.ambient.r, &tmp_mat.ambient.g, &tmp_mat.ambient.b);
				tmp_mat.color = tmp_mat.diffuse;
				break;

			//�@Diffuse
			case 'd':
				sscanf(buf, "Kd %f %f %f", &tmp_mat.diffuse.r, &tmp_mat.diffuse.g, &tmp_mat.diffuse.b);
				break;

			//�@Specular
			case 's':
				sscanf(buf, "Ks %f %f %f", &tmp_mat.specular.r, &tmp_mat.specular.g, &tmp_mat.specular.b);
				break;
			}
			break;

		//�@d
		case 'd':
			if ( sscanf(buf, "d %f", &tmp_float) == 1 )
			{
				tmp_mat.color.a = tmp_float;
				tmp_mat.ambient.a = tmp_float;
				tmp_mat.diffuse.a = tmp_float;
				tmp_mat.specular.a = tmp_float;
				tmp_mat.emission.a = tmp_float;
			}
			break;

		//�@Tr
		case 'T':
			if ( buf[1] == 'r' )
			{
				 sscanf(buf, "Tr %f %f %f", &tmp_mat.emission.r, tmp_mat.emission.g, tmp_mat.emission.b);
			}
			break;

		//�@Ni
		case 'N':
			if ( buf[1] == 'i' )
			{
				sscanf(buf, "Ni %f", &tmp_mat.shininess);
			}
			break;

		//�@�Y���Ȃ�
		default:
			break;
		}
	}

	//�@�Ō�ɓǂ݂Ƃ����}�e���A����ǉ�
	AddMaterial(tmp_mat);
	
	//�@�t�@�C�������
	file.close();

	return true;
}

//-------------------------------------------------------------------------------------------------
//�@�@Load
//�@�@Desc : �t�@�C���̃��[�h
//-------------------------------------------------------------------------------------------------
bool OBJMesh::Load(const char *objfilename)
{
	//�@
	Release();

	//�@��x���������m��
	vertex = (OBJVertex*)malloc(1*sizeof(OBJVertex));
	normal = (OBJVertex*)malloc(1*sizeof(OBJVertex));
	face = (OBJFace*)malloc(1*sizeof(OBJFace));
	material = (OBJMaterial*)malloc(1*sizeof(OBJMaterial));

	//�@�f�B���N�g����؂蔲��
	GetDirectoryName(objfilename, directoryName);
	
	//�@OBJ�t�@�C���̃��[�h
	if ( !LoadOBJFile(objfilename) ) return false;

	return true;
}

//-------------------------------------------------------------------------------------------------
//�@�@Render
//�@�@Desc : �`�揈��
//-------------------------------------------------------------------------------------------------
void OBJMesh::Render(float scale)
{
	int pre_mat = -1, cur_mat = 0;
	
	for ( int i=0; i<num_face; i++ )
	{
		//�@�}�e���A��������Ƃ�
		if ( num_material > 0 )
		{
			//�@�C���f�b�N�X���i�[
			cur_mat = face[i].material_index;

			//�@�O�ƈقȂ�F�̂Ƃ�
			if ( pre_mat != cur_mat )
			{
				//�@Object Color
				glColor4fv(material[cur_mat].color);

				//�@Ambient Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material[cur_mat].ambient);
				
				//�@Diffuse Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material[cur_mat].diffuse);
				
				//�@Specular Color
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material[cur_mat].specular);

				//�@Emission
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material[cur_mat].emission);
				
				//�@Shininess
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material[cur_mat].shininess);
				
				//�@�X�V
				pre_mat = cur_mat;
			}
		}

		//�@�ʂ̕`����J�n
		glBegin(face[i].type);
		for ( int j=0; j<face[i].element; j++ )
		{
			//�@�@���x�N�g��
			if ( face[i].use_normal ) glNormal3fv(normal[face[i].normal_index[j]]);
			
			//�@���_
			glVertex3fv(	vertex[face[i].vertex_index[j]]*scale/size);
		}
		//�@�ʂ̕`����I��
		glEnd();
	}

}

//--------------------------------------------------------------------------------------------------
//�@�@MakeDisplayList
//�@�@Desc : �f�B�X�v���C���X�g���쐬
//--------------------------------------------------------------------------------------------------
GLuint OBJMesh::MakeDisplayList(float scale)
{
	//�@���X�gID�𐶐�
	listID = glGenLists(1);

	//�@�f�B�X�v���C���X�g�̍쐬�J�n
	glNewList(listID, GL_COMPILE);

	//�@�`��
	Render(scale);

	//�@�f�B�X�v���C���X�g�̍쐬�I��
	glEndList();

	//�@���X�gID��Ԃ�
	return listID;
}