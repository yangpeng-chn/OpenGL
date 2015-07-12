#ifndef PNG_H
#define PNG_H

#include "lodepng.h"

//�e�N�X�`���N���X
class TEXTURE{
protected:
	LodePNG_Decoder decoder;//�f�R�[�_
	unsigned char* buffer;//�o�b�t�@
	size_t buffersize, imagesize;//�T�C�Y
public:
	TEXTURE();
	TEXTURE(const char* FileName);//�R���X�g���N�^
	void LOAD_PNG(const char* FileName);//PNG�ǂݍ���
	unsigned char* image;//�C���[�W�|�C���^
	unsigned int Width,Height;//�摜�T�C�Y
};
TEXTURE::TEXTURE(){
}
TEXTURE::TEXTURE(const char* FileName){
	LOAD_PNG(FileName);
}
void TEXTURE::LOAD_PNG(const char* FileName){
	LodePNG_Decoder_init(&decoder);
	//���[�h
	LodePNG_loadFile(&buffer, &buffersize, FileName);
	//�f�R�[�h
	LodePNG_decode(&decoder, &image, &imagesize, buffer, buffersize);
	//��,����
	Width = decoder.infoPng.width;Height = decoder.infoPng.height;
}
#endif