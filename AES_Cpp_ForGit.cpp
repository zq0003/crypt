//Edit by zq
//All right reserved
//zq0003@163.com
//Version 1.3s For GitHub
//20160211
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "MemAllot_Cpp.h"
#include "AES_Cpp_ForGit.h"

using namespace std;
namespace ZQs_tools{

CAES::CAES()
{
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
			m_pKey[i][j]=m_pKey[i-1][j];
		}
		if(i%m_Nk==0){
			for(j=0;j<4;j++){	
				m_pKey[i][j]=g_AESSBox[(m_pKey[i-1][(j+1)%4]&0xf0)>>4][m_pKey[i-1][(j+1)%4]&0x0f]^g_AESRcon[i/m_Nk][j];
			}
		}
		else if( (m_Nk>6)&&(i%m_Nk==4) ){
			for(j=0;j<m_Nb;j++){
				m_pKey[i][j]=g_AESSBox[(m_pKey[i-1][j]&0xf0)>>4][m_pKey[i-1][j]&0x0f];
			}
		}
		for(j=0;j<4;j++){
			m_pKey[i][j]^=m_pKey[i-m_Nk][j];
		}
	}
}
inline void CAES::SubBytes(unsigned char *(InOut[]))
{
	int il_i,il_j;
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			InOut[il_i][il_j]=g_AESSBox[(InOut[il_i][il_j]&0xf0)>>4][InOut[il_i][il_j]&0x0f];
		}
	}
}
inline void CAES::InvSubBytes(unsigned char *(InOut[]))
{
	int il_i,il_j;
	for(il_i=0;il_i<m_Nb;il_i++){
		for(il_j=0;il_j<4;il_j++){
			InOut[il_i][il_j]=g_AESInvSBox[(InOut[il_i][il_j]&0xf0)>>4][InOut[il_i][il_j]&0x0f];
		}
	}
}
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
	for(il_i=1;il_i<4;il_i++){
		for(il_j=0;il_j<m_Nb;il_j++){
			InOut[il_j][il_i]=il_temp[(il_j+il_i)%m_Nb][il_i];	
		}
	}
	//MemAllot_Del_2D(il_temp, NB)
}
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

unsigned char CAES::FilE(char *pInFilePath, char *pOutFilePath)
{
	int i;
	//unsigned char *Buf;
	unsigned char AddByteNum;
	long long InFLen;
	int BloLen, BloNum, TailBloLen;
	ifstream o_InF;
	ofstream o_OutF;
	FileTypeInfo *pFileTypeInfo = (struct FileTypeInfo*)malloc(sizeof(struct FileTypeInfo));
	o_OutF.open(pOutFilePath, ios::out | ios::binary | ios::_Noreplace | ios::app);
	if (!o_OutF.is_open()) {
		cout << "Error: " << pOutFilePath << "already exist or creat "<< pOutFilePath <<"failed.\n";
		return 0x02;
	}
	o_InF.open(pInFilePath, ios::in | ios::binary);
	if (!o_InF.is_open()) {
		cout << "Error: open" << pInFilePath << " failed.\n";
		o_OutF.close();
		return 0x01;
	}
	GetFileTypeInfo(pInFilePath, pFileTypeInfo);
		
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

	o_InF.seekg(0, ios::end);
	InFLen = o_InF.tellg();
	o_InF.seekg(0, ios::beg);

	InFLen -= pFileTypeInfo->HeaderByteNum + pFileTypeInfo->TailerByteNum;
	AddByteNum = (m_Nb * 4 - InFLen % (m_Nb * 4)) % (m_Nb * 4);
	InFLen += AddByteNum;
	BloLen = BLOLEN;
	BloNum = (InFLen - 1) / BloLen;
	TailBloLen = InFLen%BloLen;
	if (TailBloLen == 0) {
		TailBloLen = BloLen;
	}

	//MemAllot_New_1D(Buf, BLOLEN, unsigned char);
	o_InF.read((char *)Buf, pFileTypeInfo->HeaderByteNum);
	o_OutF.write((const char *)Buf, pFileTypeInfo->HeaderByteNum);
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
	o_InF.read((char *)Buf, pFileTypeInfo->TailerByteNum);
	o_OutF.write((const char *)Buf, pFileTypeInfo->TailerByteNum);
	//MemAllot_Del_1D(Buf);
	//MemAllot_Del_2D(m_pKey, NB*(NR + 1));
	free(pFileTypeInfo);
	o_InF.close();
	o_OutF.close();
	return 0x00;
}

unsigned char CAES::FilD(char *pInFilePath, char *pOutFilePath)
{
	int i;
	//unsigned char *Buf;
	unsigned char AddByteNum;
	long long InFLen;
	int BloLen, BloNum, TailBloLen;
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

	o_InF.seekg(0, ios::end);
	InFLen = o_InF.tellg();
	o_InF.seekg(0, ios::beg);
	BloLen = BLOLEN;

	InFLen -= pFileTypeInfo->HeaderByteNum + pFileTypeInfo->TailerByteNum + 1;
	BloNum = ( InFLen - 1) / BloLen;
	TailBloLen = InFLen % BloLen;
	if (TailBloLen == 0) {
		TailBloLen = BloLen;
	}
	//MemAllot_New_1D(Buf, BLOLEN, unsigned char);
	o_InF.read((char *)Buf, pFileTypeInfo->HeaderByteNum);
	o_OutF.write((const char *)Buf, pFileTypeInfo->HeaderByteNum);
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
	o_InF.read((char *)Buf, pFileTypeInfo->TailerByteNum);
	o_OutF.write((const char *)Buf, pFileTypeInfo->TailerByteNum);
	//MemAllot_Del_1D(Buf);
	//MemAllot_Del_2D(m_pKey, NB*(NR + 1))
	free(pFileTypeInfo);
	o_InF.close();
	o_OutF.close();
	return 0x00;
}

}