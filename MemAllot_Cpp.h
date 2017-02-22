//Edit by zq
//All right reserved
//zq0003@163.com
//Version 2.1s
//20170220
#if !defined(MEMALLOT_CPP_H__ZQ0913__INCLUDED_)
#define MEMALLOT_CPP_H__ZQ0913__INCLUDED_

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#include <iostream>
using std::cout;

//----------------------------------------------------------
//	宏功能：分配满足bit数组长度Dim1的最短的char型数组
//----------------------------------------------------------	
#define MemAllot_New_1D_BitVecChar(BitVec,Dim1) {\
    if((Dim1)<1) {\
        cout<<"Error: BitVector dimension <= 0 .";\
        break;\
    }\
    else {\
        BitVec=new char[(((Dim1)-1)/8+1)];}\
    }\
}

//-------------//
// 1-D Matrix  //
//-------------//
#define MemAllot_New_1D(Matrix,Dim,Type) {\
    if (((Dim)<=0)||(Matrix=new Type[Dim])==NULL){\
        cout<<"Error: Matrix dimension1 <= 0 or operation new failed.";\
    }\
}
#define MemAllot_Del_1D(Matrix) {\
    if(Matrix!=NULL){\
        delete[] (Matrix);\
        Matrix=NULL;\
    }\
}
#define MemAllot_Zero_1D(Matrix,Dim) {\
    for(int di=0;di<Dim;di++){\
        Matrix[di]=0;\
    }\
}

//-------------//
// 2-D Matrix  //
//-------------//
/*
#define MemAllot_New_2D(Matrix,Dim1,Dim2,Type) {\
			Type *d_pVec;\
			int d_i;\
			if((Dim1)<=0||(Dim2)<=0||(d_pVec=new Type[(Dim1)*(Dim2)])==NULL) {\
				ASSERT(_T("Error:维数小于零或内存不足!"));\
			}\
			if((Matrix=new Type* [Dim1])==NULL) {\
				ASSERT(_T("Error:内存不足!"));\
			}\
			for(d_i=0;d_i<(Dim1);d_i++) {\
				Matrix[d_i]=d_pVec+d_i*(Dim2);\
			}\
		}*/
#define MemAllot_New_2D(Matrix,Dim1,Dim2,Type) {\
    if((Dim1)<=0||(Dim2)<=0){\
        cout<<"Error: Matrix dimension1 <= 0 or Matrix dimension2 <= 0.";\
    }\
    int d_i;\
    if((Matrix=new Type*[(Dim1)])==NULL){\
        cout<<"Error: Operation new failed.";\
    }\
    else{\
        for(d_i=0;d_i<Dim1;d_i++){\
            if((Matrix[d_i]=new Type[(Dim2)])==NULL){\
                cout<<"Error: Operation new failed.";\
            }\
        }\
    }\
}
#define MemAllot_Del_2D(Matrix,Dim1) {\
	if(Matrix!=NULL){\
		int d_i;\
		for(d_i=0;d_i<Dim1;d_i++){\
			delete[] Matrix[d_i];\
		}\
		delete[] Matrix;\
		Matrix = NULL;\
	}\
}
#define MemAllot_Zero_2D(Matrix,Dim1,Dim2) {\
    int d_i,d_j;\
    for(d_i=0;d_i<Dim1;d_i++){\
        for(d_j=0;d_j<Dim2;d_j++){\
            Matrix[d_i][d_j]=0;\
        }\
    }\
}

//-------------//
// 3-D Matrix  //
//-------------//
/*
#define MemAllot_New_3D(Matrix,Dim1,Dim2,Dim3,Type) {\
			Type *_Pnt;int _Cnt,_Dnt;\
            if((Dim1)<=0||(Dim2)<=0||(Dim3<=0)||(_Pnt=new Type[(Dim1)*(Dim2)*(Dim3)])==NULL){\
				ASSERT(_T("Error:维数小于零或内存不足!"));\
			}\
			if((Matrix=new Type** [Dim1])==NULL){\
				ASSERT(_T("Error:内存不足!"));\
			}\
            for(_Cnt=0;_Cnt<(Dim1);_Cnt++){\
				if((Matrix[_Cnt]=new Type* [Dim2])==NULL){\
					ASSERT(_T("Error:内存不足!"));\
				}\
				for(_Dnt=0;_Dnt<(Dim2);_Dnt++){\
					Matrix[_Cnt][_Dnt]=_Pnt+_Cnt*(Dim2)*(Dim3)+_Dnt*(Dim3);\
				}\
			}\
		}*/
#define MemAllot_New_3D(Matrix,Dim1,Dim2,Dim3,Type) {\
    if((Dim1)<=0||(Dim2)<=0||(Dim3)<=0){\
        cout<<"Error: Matrix dimension1 <= 0 or Matrix dimension2 <= 0 or Matrix dimension3 <= 0.";\
    }\
    int d_i,d_j;\
    if((Matrix=new Type**[(Dim1)])==NULL){\
        cout<<"Error: Operation new failed.";\
    }\
    else{\
        for(d_i=0;d_i<Dim1;d_i++){\
            if((Matrix[d_i]=new Type*[(Dim2)])==NULL){\
                cout<<"Error: Operation new failed.";\
            }\
            else{\
                for(d_j=0;d_j<Dim2;d_j++){\
                    if((Matrix[d_i][d_j]=new Type[(Dim3)])==NULL){\
                        cout<<"Error: Operation new failed.";\
                    }\
                }\
            }\
        }\
    }\
}
#define MemAllot_Del_3D(Matrix,Dim1,Dim2) {\
	if(Matrix!=NULL){\
		int d_i,d_j;\
		for(d_i=0;d_i<Dim1;d_i++){\
			for(d_j=0;d_j<Dim2;d_j++){\
				delete[] Matrix[d_i][d_j];\
			}\
		}\
		for(d_i=0;d_i<Dim1;d_i++){\
			delete[] Matrix[d_i];\
		}\
		delete[] Matrix;\
		Matrix = NULL;\
	}\
}

#define MemAllot_Zero_3D(Matrix,Dim1,Dim2,Dim3)	{\
			int d_i,d_j,d_k;\
			for(d_i=0;d_i<Dim1;d_i++){\
				for(d_j=0;d_j<Dim2;d_j++){\
					for(d_k=0;d_k<Dim3;d_k++){\
						Matrix[d_i][d_j][d_k]=0;\
					}\
				}\
			}\
		}

//-------------//
// 4-D Matrix  //
//-------------//
#define MemAllot_New_4D(Matrix,Dim1,Dim2,Dim3,Dim4,Type) {\
    Type *d_p;int d_i,d_j,d_k;\
    if((Dim1)<=0||(Dim2)<=0||(Dim3)<=0||(Dim4)<=0||(d_p=new Type[(Dim1)*(Dim2)*(Dim3)*(Dim4)])==NULL){\
        cout<<"Error: Matrix dimension1 <= 0 or operation new failed.";\
    }\
    if((Matrix=new Type*** [(Dim1)])==NULL){\
        cout<<"Error: Operation new failed.";\
    }\
    for(d_i=0;d_i<(Dim1);d_i++){\
        if((Matrix[d_i]=new Type** [Dim2])==NULL){\
            cout<<"Error: Operation new failed.";\
        }\
        for(d_j=0;d_j<(Dim2);d_j++){\
            if((Matrix[d_i][d_j]=new Type* [Dim3])==NULL){\
                cout<<"Error: Operation new failed.";\
            }\
            for(d_k=0;d_k<(Dim3);d_k++){\
                Matrix[d_i][d_j][d_k]=d_p+d_i*(Dim2)*(Dim3)*(Dim4)+d_j*(Dim3)*(Dim4)+d_k*(Dim4);\
            }\
        }\
    }\
}
/*
#define MemAllot_Del_4D(Matrix) {\
			if(Matrix!=NULL) {\
				delete[] (***Matrix);\
				delete[] (**Matrix);\
				delete[] (*Matrix);\
				delete[] (Matrix);\
				Matrix=NULL;\
			}\
		}
*/
#define MemAllot_Zero_4D(Matrix,Dim1,Dim2,Dim3,Dim4)	{\
    int d_i,d_j,d_k,d_l;\
    for(d_i=0;d_i<(Dim1);d_i++){\
        for(d_j=0;d_j<(Dim2);d_j++){\
            for(d_k=0;d_k<(Dim3);d_k++){\
                for(d_l=0;d_l<(Dim4);d_l++){\
                    Matrix[d_i][d_j][d_k][d_l]=0;\
                }\
            }\
        }\
    }\
}

#endif
