//Edit by zq
//zq0003@163.com
//Version 1.3s For GitHub
//20160211
#if !defined(CryptBase_Cpp_ForGit_H__ZQ0913__INCLUDED_)
#define	CryptBase_Cpp_ForGit_H__ZQ0913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace ZQs_tools{
class CCryptBase
{
public:
	CCryptBase();
	~CCryptBase();
	void SetKey(char *pKeyStr, int StrLen, int KeyBitLen);

protected:
	unsigned char Key64bit[8];
	unsigned char Key128bit[16];
	unsigned char Key192bit[24];
	unsigned char Key256bit[32];
	unsigned char Key512b[64];
	unsigned char Key1024b[128];

private:
	//unsigned char Key8B[8];
	//unsigned char Key24B[24];
	unsigned char Key32B[32];
	unsigned char Key64B[64];
	unsigned char Key128B[128];
};

}
#endif
