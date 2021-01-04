//Edit by zq
//zq0003@163.com
//Version 1.3s For GitHub
//20160211
#include "stdafx.h"
#include "CryptBase_Cpp_ForGit.h"

namespace ZQs_tools{
CCryptBase::CCryptBase()
{
}
CCryptBase::~CCryptBase()
{
}
void CCryptBase::SetKey(char *pKeyStr, int StrLen, int KeyBitLen)
{
	int i;
	switch(KeyBitLen){
		case 1024:
			if(StrLen>=128){
				for(i=0;i<128;i++){
					Key128B[i]=pKeyStr[i];
				}
			}
			else{
				for(i=0;i<StrLen;i++){
					Key128B[i]=pKeyStr[i];
				}
				for(i=StrLen;i<128;i++){
					Key128B[i]=1;
				}
			};
			break;
		case 512:
			if(StrLen>=64){
				for(i=0;i<64;i++){
					Key64B[i]=pKeyStr[i];
				}
			}
			else{
				for(i=0;i<StrLen;i++){
					Key64B[i]=pKeyStr[i];
				}
				for(i=StrLen;i<64;i++){
					Key64B[i]=2;
				}
			};
			break;
		case 256:
			if(StrLen>=32){
				for(i=0;i<32;i++){
					Key256bit[i]=pKeyStr[i];
				}
			}
			else{
				for(i=0;i<StrLen;i++){
					Key256bit[i]=pKeyStr[i];
				}
				for(i=StrLen;i<32;i++){
					Key256bit[i]=3;
				}
			};
			break;
		case 192:
			if(StrLen>=24){
				for(i=0;i<24;i++){
					Key192bit[i]=pKeyStr[i];
				}
			}
			else{
				for(i=0;i<StrLen;i++){
					Key192bit[i]=pKeyStr[i];
				}
				for(i=StrLen;i<24;i++){
					Key192bit[i]=4;
				}
			};
			break;
		case 128:
			if(StrLen>=16){
				for(i=0;i<16;i++){
					Key128bit[i]=pKeyStr[i];
				}
			}
			else{
				for(i=0;i<StrLen;i++){
					Key128bit[i]=pKeyStr[i];
				}
				for(i=StrLen;i<16;i++){
					Key128bit[i]=5;
				}
			};
			break;
		case 64:
			if(StrLen>=8){
				for(i=0;i<8;i++){
					Key64bit[i]=pKeyStr[i];
				}
			}
			else{
				for(i=0;i<StrLen;i++){
					Key64bit[i]=pKeyStr[i];
				}
				for(i=StrLen;i<8;i++){
					Key64bit[i]=6;
				}
			};
			break;
		default:break;
	}
}

}
