//Edit by zq
//All right reserved
//zq0003@163.com
//Version 1.6s For GitHub
//20170510
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "MemAllot_Cpp.h"
#include "AES_Cpp_ForGit.h"

using namespace std;
namespace ZQs_tools{

CAES::CAES()
{
	Ver = 1;
	SubVer = 6;
	m_Nk = NK;
	m_Nb = NB;
	m_Nr = NR;
	MemAllot_New_2D(il_temp, NB, 4, unsigned char)
	MemAllot_New_2D(pBlo, NB, 4, unsigned char)
	MemAllot_New_2D(m_pKey, NB*(NR + 1), 4, unsigned char)
	MemAllot_New_1D(Buf, BLOLEN, unsigned char)
}
CAES::~CAES()
{
	MemAllot_Del_2D(il_temp, NB)
	MemAllot_Del_2D(pBlo, NB)
	MemAllot_Del_1D(Buf)
	MemAllot_Del_2D(m_pKey, NB*(NR + 1))
}

//=====================================================================================================
//	Function: GetFileTypeInfo
//	in:
//		char *pFilePath
//	out:
//		FileTypeInfo *pFileTypeInfo
//=======================================================================================================

void CAES::GetFileTypeInfo(char *pFilePath, FileTypeInfo *pFileTypeInfo)
{
	int FilePathstrLen;
	char *pFileType;
	pFileTypeInfo->HeaderByteNum = 0;
	pFileTypeInfo->TailerByteNum = 0;
	FilePathstrLen = strlen((const char *)pFilePath);
	pFileType = pFilePath + FilePathstrLen;//将pFileType定位于pInFilePath的末尾
	if (FilePathstrLen >= 2) {
		pFileType -= 2;
		if (!strcmp((const char *)pFileType, "et")) {
			pFileTypeInfo->HeaderByteNum = 516;
			pFileTypeInfo->TailerByteNum = 16;
		}
		else if (!strcmp((const char *)pFileType, "rm")) {
			pFileTypeInfo->HeaderByteNum = 4;
		}
	}
	if (FilePathstrLen >= 3) {
		pFileType--;
		if (!strcmp((const char *)pFileType, "doc")) {
			pFileTypeInfo->HeaderByteNum = 512;
			pFileTypeInfo->TailerByteNum = 48;
		}
		else if (!strcmp((const char *)pFileType, "xls")) {
			pFileTypeInfo->HeaderByteNum = 512;
			pFileTypeInfo->TailerByteNum = 8;
		}
		else if (!strcmp((const char *)pFileType, "wps")) {
			pFileTypeInfo->HeaderByteNum = 528;
			pFileTypeInfo->TailerByteNum = 8;
		}
		else if (!strcmp((const char *)pFileType, "avi") \
			|| !strcmp((const char *)pFileType, "dwg") \
			|| !strcmp((const char *)pFileType, "mid") \
			|| !strcmp((const char *)pFileType, "mov") \
			|| !strcmp((const char *)pFileType, "mpg") \
			|| !strcmp((const char *)pFileType, "psd") \
			|| !strcmp((const char *)pFileType, "pst") \
			|| !strcmp((const char *)pFileType, "pwl") \
			|| !strcmp((const char *)pFileType, "qdf") \
			|| !strcmp((const char *)pFileType, "ram") \
			|| !strcmp((const char *)pFileType, "tif") \
			|| !strcmp((const char *)pFileType, "wav") \
			|| !strcmp((const char *)pFileType, "wpd")
			) {
			pFileTypeInfo->HeaderByteNum = 4;
		}
		else if (!strcmp((const char *)pFileType, "bmp")) {
			pFileTypeInfo->HeaderByteNum = 2;
		}
		else if (!strcmp((const char *)pFileType, "gif")) {
			pFileTypeInfo->HeaderByteNum = 4;
			pFileTypeInfo->TailerByteNum = 2;
		}
		else if (!strcmp((const char *)pFileType, "zip")) {
			pFileTypeInfo->HeaderByteNum = 7;
			pFileTypeInfo->TailerByteNum = 2;
		}
		else if (!strcmp((const char *)pFileType, "jpe")\
			|| !strcmp((const char *)pFileType, "jpg")) {
			pFileTypeInfo->HeaderByteNum = 3;
			pFileTypeInfo->TailerByteNum = 2;
		}
		else if (!strcmp((const char *)pFileType, "png")) {
			pFileTypeInfo->HeaderByteNum = 4;
			pFileTypeInfo->TailerByteNum = 4;
		}
		else if (!strcmp((const char *)pFileType, "asf") \
			|| !strcmp((const char *)pFileType, "dbx")) {
			pFileTypeInfo->HeaderByteNum = 8;
		}
		else if (!strcmp((const char *)pFileType, "eml")) {
			pFileTypeInfo->HeaderByteNum = 14;
		}
		else if (!strcmp((const char *)pFileType, "mdb")) {
			pFileTypeInfo->HeaderByteNum = 10;
		}
		else if (!strcmp((const char *)pFileType, "pdf") \
			|| !strcmp((const char *)pFileType, "rar")) {
			pFileTypeInfo->HeaderByteNum = 7;
		}
		else if (!strcmp((const char *)pFileType, "rtf") \
			|| !strcmp((const char *)pFileType, "xml")) {
			pFileTypeInfo->HeaderByteNum = 5;
		}
		else {};
	}
	if (FilePathstrLen >= 4) {
		pFileType--;
		if (!strcmp((const char *)pFileType, "docx")) {
			pFileTypeInfo->HeaderByteNum = 437;
			pFileTypeInfo->TailerByteNum = 4;
		}
		else if (!strcmp((const char *)pFileType, "xlsx")) {
			pFileTypeInfo->HeaderByteNum = 48;
			pFileTypeInfo->TailerByteNum = 4;
		}
		else if (!strcmp((const char *)pFileType, "html")) {
			pFileTypeInfo->HeaderByteNum = 5;
		}
		else if (!strcmp((const char *)pFileType, "jpeg")) {
			pFileTypeInfo->HeaderByteNum = 3;
			pFileTypeInfo->TailerByteNum = 2;
		}
		else {};
	}
}
/*
void CAES::SetMem(int nk, int nb, int nr)
{
    m_Nk = nk;
	m_Nb = nb;
	m_Nr = nr;
}*/

//===============================================================================
//	function: Key Expansion
//	in:
//		const unsigned char *const Key
//	out:
//		unsigned char **g_pKey
//	note:
//	bug:
//===============================================================================
void CAES::KeyIni(const unsigned char *const Key)
{
	int i,j;
	unsigned char temp[4]={0};
	for(i=0;i<m_Nk;i++){
		for(j=0;j<4;j++){
			m_pKey[i][j]=Key[i*4+j];
		}
	}
	for(i=m_Nk;i<m_Nb*(m_Nr+1);i++){
		for(j=0;j<4;j++){
			m_pKey[i][j]=m_pKey[i-1][j];//因为g_pKey[i][j]在temp[i]完成使命前均未使用，故用g_pKey[i][j]代替了temp[i]
		}
		if(i%m_Nk==0){
			//SubWord(RotWord())^Rcon()
			for(j=0;j<4;j++){	
				m_pKey[i][j]=g_AESSBox[(m_pKey[i-1][(j+1)%4]&0xf0)>>4][m_pKey[i-1][(j+1)%4]&0x0f]^g_AESRcon[i/m_Nk][j];
			}
		}
		else if( (m_Nk>6)&&(i%m_Nk==4) ){
			//SubWord();
			for(j=0;j<m_Nb;j++){
				m_pKey[i][j]=g_AESSBox[(m_pKey[i-1][j]&0xf0)>>4][m_pKey[i-1][j]&0x0f];
			}
		}
		for(j=0;j<4;j++){
			m_pKey[i][j]^=m_pKey[i-m_Nk][j];
		}
	}
}

//===============================================================================
//	Function: SubBytes
//	in: 
//		unsigned char *(InOut[])
//	out:
//		unsigned char *(InOut[])
//	note: 
//		因为input bytes 到 State array 有行列的转换，而此处未转换，
//      所以i对于NB、j对应4
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
inline void CAES::SubBytes(unsigned char *(InOut[]))
{
	int il_i,il_j;
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			InOut[il_i][il_j]=g_AESSBox[(InOut[il_i][il_j]&0xf0)>>4][InOut[il_i][il_j]&0x0f];
		}
	}
}

//===============================================================================
//	Function: InvSubBytes
//	in: 
//		unsigned char *(InOut[])
//	out:
//		unsigned char *(InOut[])
//	note: 
//		因为input bytes 到 State array 有行列的转换，而此处未转换，
//      所以i对于NB、j对应4
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
inline void CAES::InvSubBytes(unsigned char *(InOut[]))
{
	int il_i,il_j;
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			InOut[il_i][il_j]=g_AESInvSBox[(InOut[il_i][il_j]&0xf0)>>4][InOut[il_i][il_j]&0x0f];
		}
	}
}

//===============================================================================
//	Function: ShiftRows
//	in: 
//		unsigned char *(InOut[])
//	out:
//		unsigned char *(InOut[])
//	note: 
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
inline void CAES::ShiftRows(unsigned char *(InOut[]))
{
	int il_i,il_j;
	//unsigned char **il_temp;
	//MemAllot_New_2D(il_temp, NB, 4, unsigned char)
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			il_temp[il_i][il_j]=InOut[il_i][il_j];
		}
	}
	for(il_i=1;il_i<4;il_i++){//由于最初并未按标准中规定旋转，所以此处按列shift而不是按行
		for(il_j=0;il_j<m_Nb;il_j++){
			InOut[il_j][il_i]=il_temp[(il_j+il_i)%m_Nb][il_i];	
		}
	}
	//MemAllot_Del_2D(il_temp, NB)
}

//===============================================================================
//	Function: InvShiftRows
//	in: 
//		unsigned char *(InOut[])
//	out:
//		unsigned char *(InOut[])
//	note: 
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
inline void CAES::InvShiftRows(unsigned char *(InOut[]))
{
	int il_i,il_j;
	//unsigned char **il_temp;
	//MemAllot_New_2D(il_temp, NB, 4, unsigned char)
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			il_temp[il_i][il_j]=InOut[il_i][il_j];
		}
	}
	for(il_i=1;il_i<4;il_i++){
		for(il_j=0;il_j<m_Nb;il_j++){
			InOut[il_j][il_i]=il_temp[(il_j+4-il_i)%m_Nb][il_i];	
		}
	}
	//MemAllot_Del_2D(il_temp, NB)
}

//===============================================================================
//	Function: MixColumns
//	in: 
//		unsigned char *(InOut[])
//	out:
//		unsigned char *(InOut[])
//	note: 
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
inline void CAES::MixColumns(unsigned char *(InOut[]))
{
	int il_i,il_j;
	//unsigned char **il_temp;
	//MemAllot_New_2D(il_temp, NB, 4, unsigned char)
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			il_temp[il_i][il_j]=InOut[il_i][il_j];
		}
	}
	for(il_i=0;il_i<m_Nb;il_i++){
		InOut[il_i][0]=g_MixColumns02[il_temp[il_i][0]]^g_MixColumns03[il_temp[il_i][1]]^il_temp[il_i][2]^il_temp[il_i][3];
		InOut[il_i][1]=il_temp[il_i][0]^g_MixColumns02[il_temp[il_i][1]]^g_MixColumns03[il_temp[il_i][2]]^il_temp[il_i][3];
		InOut[il_i][2]=il_temp[il_i][0]^il_temp[il_i][1]^g_MixColumns02[il_temp[il_i][2]]^g_MixColumns03[il_temp[il_i][3]];
		InOut[il_i][3]=g_MixColumns03[il_temp[il_i][0]]^il_temp[il_i][1]^il_temp[il_i][2]^g_MixColumns02[il_temp[il_i][3]];
	}
	//MemAllot_Del_2D(il_temp, NB)
}

//===============================================================================
//	Function: InvMixColumns
//	in: 
//		unsigned char *(InOut[])
//	out:
//		unsigned char *(InOut[])
//	note: 
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
inline void CAES::InvMixColumns(unsigned char *(InOut[]))
{
	int il_i,il_j;
	//unsigned char **il_temp;
	//MemAllot_New_2D(il_temp, NB, 4, unsigned char)
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			il_temp[il_i][il_j]=InOut[il_i][il_j];
		}
	}
	for(il_i=0;il_i<m_Nb;il_i++){
		InOut[il_i][0]=g_InvMixColumns0e[il_temp[il_i][0]]^g_InvMixColumns0b[il_temp[il_i][1]]^g_InvMixColumns0d[il_temp[il_i][2]]^g_InvMixColumns09[il_temp[il_i][3]];
		InOut[il_i][1]=g_InvMixColumns09[il_temp[il_i][0]]^g_InvMixColumns0e[il_temp[il_i][1]]^g_InvMixColumns0b[il_temp[il_i][2]]^g_InvMixColumns0d[il_temp[il_i][3]];
		InOut[il_i][2]=g_InvMixColumns0d[il_temp[il_i][0]]^g_InvMixColumns09[il_temp[il_i][1]]^g_InvMixColumns0e[il_temp[il_i][2]]^g_InvMixColumns0b[il_temp[il_i][3]];
		InOut[il_i][3]=g_InvMixColumns0b[il_temp[il_i][0]]^g_InvMixColumns0d[il_temp[il_i][1]]^g_InvMixColumns09[il_temp[il_i][2]]^g_InvMixColumns0e[il_temp[il_i][3]];
	}
	//MemAllot_Del_2D(il_temp, NB)
}

//===============================================================================
//	Function: BufE
//	in: 
//		unsigned char *pBuf
//      int BufL: length in Bytes
//	out:
//		unsigned char *pBuf
//	note:
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
void CAES::BufE(unsigned char *pBuf,int BufL)
{
	int i,j,k,l;
	int BloNum=BufL/(m_Nb*4);
	//unsigned char **pBlo;
	//MemAllot_New_2D(pBlo, NB, 4, unsigned char)
	for(i=0;i<BloNum;i++){
		for(j=0;j<m_Nb;j++){
			for(k=0;k<4;k++){
				pBlo[j][k]=pBuf[i*m_Nb*4+j*4+k]^m_pKey[j][k];
			}
		}
		for(j=0;j<m_Nr-1;j++){
			SubBytes(pBlo);
			ShiftRows(pBlo);
			MixColumns(pBlo);
			for(k=0;k<m_Nb;k++){
				for(l=0;l<4;l++){
					pBlo[k][l]^=m_pKey[(j+1)*m_Nb+k][l];
				}
			}
		}
		SubBytes(pBlo);
		ShiftRows(pBlo);
		for(j=0;j<m_Nb;j++){
			for(k=0;k<4;k++){
				pBuf[i*m_Nb*4+j*4+k]=pBlo[j][k]^m_pKey[m_Nb*m_Nr+j][k];
			}
		}
	}
	//MemAllot_Del_2D(pBlo,NB)
}

//===============================================================================
//	Function: BufD
//	in: 
//		unsigned char *pBuf
//		int BufL
//	out:
//		unsigned char *pBuf
//	note:
//	bug: 
//		输入输出形参为指针且未检查内存是否已分配，这就需要在调用此函数的程序中实现检查，
//      否则可能发生内存溢出
//===============================================================================
void CAES::BufD(unsigned char *pBuf,int BufL)
{
	int i,j,k,l;
	int BloNum=BufL/(m_Nb*4);
	//unsigned char **pBlo;
	//MemAllot_New_2D(pBlo, NB, 4, unsigned char)
	for(i=0;i<BloNum;i++){
		for(j=0;j<m_Nb;j++){
			for(k=0;k<4;k++){
				pBlo[j][k]=pBuf[i*m_Nb*4+j*4+k]^m_pKey[m_Nb*m_Nr+j][k];
			}
		}
		for(j=m_Nr-1;j>0;j--){
			InvShiftRows(pBlo);
			InvSubBytes(pBlo);
			for(k=0;k<m_Nb;k++){
				for(l=0;l<4;l++){
					pBlo[k][l]^=m_pKey[m_Nb*j+k][l];
				}
			}
			InvMixColumns(pBlo);
		}
		InvShiftRows(pBlo);
		InvSubBytes(pBlo);
		for(j=0;j<m_Nb;j++){
			for(k=0;k<4;k++){
				pBuf[i*m_Nb*4+j*4+k]=pBlo[j][k]^m_pKey[j][k];
			}
		}
	}
	//MemAllot_Del_2D(pBlo,NB)
}

//===============================================================================
//	Function: AESFilE
//	in: 
//		char *pInFilePath
//	out:
//		char *pOutFilePath
//	return:
//		0x00 success
//		0x01 error InFile open failed
//		0x02 error OutFile is already exist or create failed
//		0x03 error InFile encrypt data length <= 0
//	note:
//		需要调用父类函数初始化密码
//	bug: 
//===============================================================================
unsigned char CAES::FilE(char *pInFilePath)
{
	int i;
	//unsigned char *Buf;
	unsigned char AddByteNum;
	long long InFLen;
	int BloLen, BloNum, TailBloLen;
	char *pOutFilePath;
	string o_s(pInFilePath);
	//o_s.insert(o_s.rfind(".", o_s.length()), "e");//code below solved the problem of filename dont include "." .
	i = o_s.rfind(".", o_s.length());
	if (i >= 0) { o_s.insert(i, "e"); }
	else { o_s.append("e"); }
	pOutFilePath = (char *)o_s.c_str();
	ifstream o_InF;
	ofstream o_OutF;
	FileTypeInfo *pFileTypeInfo = new FileTypeInfo;
	//FileTypeInfo *pFileTypeInfo = (struct FileTypeInfo*)malloc(sizeof(struct FileTypeInfo));
	o_OutF.open(pOutFilePath, ios::out | ios::binary | ios::_Noreplace | ios::app);
	if (!o_OutF.is_open()) {
		cout << "Error: " << pOutFilePath << "already exist or creat " << pOutFilePath << "failed.\n";
		return 0x02;
	}
	o_InF.open(pInFilePath, ios::in | ios::binary);
	if (!o_InF.is_open()) {
		cout << "Error: open" << pInFilePath << " failed.\n";
		o_OutF.close();
		return 0x01;
	}
	GetFileTypeInfo(pInFilePath, pFileTypeInfo);

	o_InF.seekg(0, ios::end);
	InFLen = (__int64)o_InF.tellg();
	o_InF.seekg(0, ios::beg);
	InFLen -= (__int64)(pFileTypeInfo->HeaderByteNum + pFileTypeInfo->TailerByteNum);
	if (InFLen <= 0) {
		o_InF.close();
		o_OutF.close();
		free(pFileTypeInfo);
		return 0x03;//InFile encrypt data length <= 0
	}
	AddByteNum = (m_Nb * 4 - InFLen % (m_Nb * 4)) % (m_Nb * 4);
	InFLen += (__int64)AddByteNum;
	BloLen = BLOLEN;
	BloNum = (InFLen - 1) / BloLen;
	TailBloLen = (__int64)InFLen % (__int64)BloLen;
	if (TailBloLen == 0) {
		TailBloLen = BloLen;
	}

	//MemAllot_New_2D(m_pKey, NB*(NR + 1), 4, unsigned char);
	if (m_Nk == 4) {
		KeyIni(Key128bit);
	}
	else if (m_Nk == 6) {
		KeyIni(Key192bit);
	}
	else {
		KeyIni(Key256bit);
	}

	//MemAllot_New_1D(Buf, BLOLEN, unsigned char);
	if (pFileTypeInfo->HeaderByteNum>0) {
		o_InF.read((char *)Buf, pFileTypeInfo->HeaderByteNum);
		o_OutF.write((const char *)Buf, pFileTypeInfo->HeaderByteNum);
	}
	o_OutF.write((const char*)&Ver, 1);
	o_OutF.write((const char*)&SubVer, 1);
	o_OutF.write((const char *)&AddByteNum, 1);
	if (BloNum != 0) {
		for (i = 0;i<AddByteNum;i++) {
			srand(time(0));
			*(Buf + i) = (unsigned char)(rand());
		}
		o_InF.read((char *)(Buf + AddByteNum), BloLen - AddByteNum);
		BufE(Buf, BloLen);
		o_OutF.write((const char *)Buf, BloLen);
		for (i = 1;i<BloNum;i++) {
			o_InF.read((char *)Buf, BloLen);
			BufE(Buf, BloLen);
			o_OutF.write((const char *)Buf, BloLen);
		}
		o_InF.read((char *)Buf, TailBloLen);
		BufE(Buf, TailBloLen);
		o_OutF.write((const char *)Buf, TailBloLen);
	}
	else {
		for (i = 0;i<AddByteNum;i++) {
			srand(time(0));
			*(Buf + i) = (unsigned char)(rand());
		}
		o_InF.read((char *)(Buf + AddByteNum), TailBloLen - AddByteNum);
		BufE(Buf, TailBloLen);
		o_OutF.write((const char *)Buf, TailBloLen);
	}
	if (pFileTypeInfo->TailerByteNum > 0) {
		o_InF.read((char *)Buf, pFileTypeInfo->TailerByteNum);
		o_OutF.write((const char *)Buf, pFileTypeInfo->TailerByteNum);
	}
	//MemAllot_Del_1D(Buf);
	//MemAllot_Del_2D(m_pKey, NB*(NR + 1));
	free(pFileTypeInfo);
	o_InF.close();
	o_OutF.close();
	return 0x00;	
}

//===============================================================================
//	Function: AESFilD
//	in: 
//		char *pInFilePath
//	out:
//		char *pOutFilePath
//	return:
//		0x00 success;
//		0x01 error InFile open failed
//		0x02 error OutFile already exist or Create failed
//	note: 
//		需要调用父类函数初始化密码
//	bug:
//===============================================================================
unsigned char CAES::FilD(char *pInFilePath)
{
	int i;
	//unsigned char *Buf;
	unsigned char AddByteNum;
	long long InFLen;
	int BloLen, BloNum, TailBloLen;
	char *pOutFilePath;
	string o_s(pInFilePath);
	//o_s.erase(o_s.rfind(".", o_s.length())-1, 1);//code below solved the problem of filename dont include "." .
	i = o_s.rfind(".", o_s.length());
	if (i > 0) { o_s.erase(i - 1, 1); }
	else { o_s.erase(o_s.length() - 1, 1); }
	pOutFilePath = (char *)o_s.c_str();
	ifstream o_InF;
	ofstream o_OutF;
	FileTypeInfo *pFileTypeInfo = (struct FileTypeInfo*)malloc(sizeof(struct FileTypeInfo));
	o_OutF.open(pOutFilePath, ios::out | ios::binary | ios::_Noreplace | ios::app);
	if (!o_OutF.is_open()) {
		cout << "Error: " << pOutFilePath << "already exist or creat " << pOutFilePath << "failed.\n";
		return 0x02;
	}
	o_InF.open(pInFilePath, ios::in | ios::binary);
	if (!o_InF.is_open()) {
		cout << "Error: open" << pInFilePath << " failed.\n";
		o_OutF.close();
		return 0x01;
	}
	GetFileTypeInfo(pInFilePath, pFileTypeInfo);

	o_InF.seekg(0, ios::end);
	InFLen = (__int64)o_InF.tellg();
	o_InF.seekg(0, ios::beg);
	BloLen = BLOLEN;
	InFLen -= (__int64)(pFileTypeInfo->HeaderByteNum + 2 + 1 + pFileTypeInfo->TailerByteNum);//2 Bytes Ver SubVer, 1 Byte AddByteNum
	if (InFLen <= 0) {
		o_InF.close();
		o_OutF.close();
		free(pFileTypeInfo);
		return 0x03;//decrypt section length <=0
	}
	BloNum = (InFLen - 1) / BloLen;
	TailBloLen = (__int64)InFLen % (__int64)BloLen;
	if (TailBloLen == 0) {
		TailBloLen = BloLen;
	}

	//MemAllot_New_2D(m_pKey, NB*(NR + 1), 4, unsigned char);
	if (m_Nk == 4) {
		KeyIni(Key128bit);
	}
	else if (m_Nk == 6) {
		KeyIni(Key192bit);
	}
	else {
		KeyIni(Key256bit);
	}

	//MemAllot_New_1D(Buf, BLOLEN, unsigned char);
	if (pFileTypeInfo->HeaderByteNum>0) {
		o_InF.read((char *)Buf, pFileTypeInfo->HeaderByteNum);
		o_OutF.write((const char *)Buf, pFileTypeInfo->HeaderByteNum);
	}
	o_InF.seekg(2, ios::cur);
	if (BloNum != 0) {
		o_InF.read((char *)&AddByteNum, 1);
		o_InF.read((char *)Buf, BloLen);
		BufD(Buf, BloLen);
		o_OutF.write((const char *)(Buf + AddByteNum), BloLen - AddByteNum);
		for (i = 1;i<BloNum;i++) {
			o_InF.read((char *)Buf, BloLen);
			BufD(Buf, BloLen);
			o_OutF.write((const char *)Buf, BloLen);
		}
		o_InF.read((char *)Buf, TailBloLen);
		BufD(Buf, TailBloLen);
		o_OutF.write((const char *)Buf, TailBloLen);
	}
	else {
		o_InF.read((char *)&AddByteNum, 1);
		o_InF.read((char *)Buf, TailBloLen);
		BufD(Buf, TailBloLen);
		o_OutF.write((const char *)(Buf + AddByteNum), TailBloLen - AddByteNum);
	}
	if (pFileTypeInfo->TailerByteNum>0) {
		o_InF.read((char *)Buf, pFileTypeInfo->TailerByteNum);
		o_OutF.write((const char *)Buf, pFileTypeInfo->TailerByteNum);
	}
	//MemAllot_Del_1D(Buf);
	//MemAllot_Del_2D(m_pKey, NB*(NR + 1))
	free(pFileTypeInfo);
	o_InF.close();
	o_OutF.close();
	return 0x00;
}
unsigned char CAES::GetVersionForFile(char *pInFilePath, unsigned char *tmpVer, unsigned char *tmpSubVer)
{
	ifstream o_InF;
	//FileTypeInfo *pFileTypeInfo = (struct FileTypeInfo*)malloc(sizeof(struct FileTypeInfo));
	FileTypeInfo *pFileTypeInfo = new FileTypeInfo;
	o_InF.open(pInFilePath, ios::in | ios::binary);
	if (!o_InF.is_open()) {//open input file
		cout << "Error: open" << pInFilePath << " failed.\n";
		o_InF.close();
		return 0x01;
	}
	GetFileTypeInfo(pInFilePath, pFileTypeInfo);
	if (pFileTypeInfo->HeaderByteNum>0) {
		o_InF.seekg(pFileTypeInfo->HeaderByteNum, ios::beg);
	}
	o_InF.read((char *)tmpVer, 1);
	o_InF.read((char *)tmpSubVer, 1);
	//free(pFileTypeInfo);
	delete pFileTypeInfo;
	o_InF.close();
	return 0x00;
}
}