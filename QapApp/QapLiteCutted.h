#pragma once
//-------------------------------------------//
//   {<| author : Adler3D(Kashin Ivan) |>}   //
//   {<| e-mail : Adler3D@gmail.com    |>}   //
//-------------------------------------------//
// Warning! This is result of code generation. Don't try to read it!
//-------------------------------------------//
#if(defined(_CSTDLIB_)||defined(_WINDOWS_))
//  static_assert(false,"fail");
#endif
//-------------------------------------------//
//#define NOMENUS
//#define NOHELP
#define NOMENUS
#define NOHELP
#define NOPROFILER
#define NOKANJI
#define NOWH
#define NOSERVICE
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

//#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")
//#include <ws2tcpip.h>

#include <windows.h>
#undef DrawText
#include <d3d9.h>
#pragma comment(lib,"d3d9")
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <array>
#include <bitset>
#include <time.h>
#include <math.h>
//#include <shellapi.h>
//#pragma comment(lib,"shell32")
using std::string;
using std::vector;
using std::map;
using std::fstream;
using std::iostream;
using std::stringstream;
using std::array;
typedef double real;
typedef INT64 int64;
typedef UINT64 uint64;
typedef unsigned int uint;
typedef unsigned char uchar;
inline bool SysQapAssert(const string&exp,bool&ignore,const string&filename,const int line,const string&funcname);
inline bool SysQapDebugMsg(const string&msg,bool&ignore,const string&filename,const int line,const string&funcname);
#ifndef DEF_BUILD_MICRO
  #if(defined(_DEBUG)||defined(QAP_DEBUG))
    #define QapDbgBreak()__debugbreak();
    #define QapAssert(_Expression)if(!bool(_Expression)){static bool ignore=false;if(SysQapAssert((#_Expression),ignore,__FILE__,__LINE__,__FUNCSIG__))QapDbgBreak();}  
    #define QapDebugMsg(_Message){static bool ignore=false;if(SysQapDebugMsg((_Message),ignore,__FILE__,__LINE__,__FUNCSIG__))QapDbgBreak();}
    #define QapNoWay(){QapDebugMsg("no way!");}
  #else
    #define QapAssert(_Expression)if(bool(_Expression)){};
    #define QapDebugMsg(_Message)
    #define QapNoWay()
  #endif
#endif //DEF_BUILD_MICRO//

struct t_${
  template<class TYPE>static void set(TYPE&,const t_$&){}
  template<class TYPE>static void set(TYPE&a,TYPE&b){a=b;}
  template<class TYPE>static void set(TYPE&a,const TYPE&b){a=b;}
  template<class TYPE>static void set(TYPE*&a,nullptr_t b){a=b;}
  template<class TYPE>static void set(TYPE&a,typename TYPE::OwnerClass*b){a=b;}
  static void set(int&a,int b){a=b;}
  static void set(int64&a,int64 b){a=b;}
  static void set(uint64&a,uint64 b){a=b;}
  static void set(unsigned&a,unsigned b){a=b;}
  static void set(char&a,char b){a=b;}
  static void set(uchar&a,uchar b){a=b;}
  static void set(real&a,real b){a=b;}
  static void set(real&a,int b){a=b;}
  static void set(float&a,real b){a=b;}
  static void set(float&a,int b){a=b;}
  template<int n>
  static void set(string&a,const char(&b)[n]){a=b;}
};

class QapKeyboard
{
public:
  struct TKeyState
  {
    static const int MAX_KEY=263;
    bool data[MAX_KEY];
    TKeyState()
    {
      SetToDef();
    }
    void SetToDef()
    {
      for (int i=0;i<MAX_KEY;i++)data[i]=false;
    }
    bool&operator[](int index)
    {
      if(!InDip<int>(0,index,MAX_KEY-1))
      {
        QapDebugMsg("bad index");
        static bool tmp;
        return tmp;
      }
      return data[index];
    }
    bool&operator()(int index)
    {
      if(!InDip<int>(0,index,MAX_KEY-1))
      {
        QapDebugMsg("bad index");
        static bool tmp;
        return tmp;
      }
      return data[index];
    }
    const bool&operator()(int index)const
    {
      if(!InDip<int>(0,index,MAX_KEY-1))
      {
        QapDebugMsg("bad index");
        static bool tmp;
        return tmp;
      }
      return data[index];
    }
    const bool&operator[](int index)const
    {
      if(!InDip<int>(0,index,MAX_KEY-1))
      {
        QapDebugMsg("bad index");
        static bool tmp;
        return tmp;
      }
      return data[index];
    }
  };
public:
public:
  typedef QapKeyboard SelfClass;
public:
  int LastKey=0;
  char LastChar=0;
  bool News=false;
  TKeyState Down;
  TKeyState Changed;
public:
  void KeyUpdate(int Key,bool Value)
  {
    if(Value)LastKey=Key;
    Down[Key]=Value;
    Changed[Key]=true;
  }
  void CharUpdate(char c)
  {
    LastChar=c;
    News=true;
  }
  void Sync()
  {
    this->News=false;
    Changed.SetToDef();
  }
  bool OnDown(int index)const
  {
    return Changed[index]&&Down[index];
  }
  bool OnUp(int index)const
  {
    return Changed[index]&&!Down[index];
  }
  vec2d get_dir_from_wasd_and_arrows(bool wasd=true,bool arrows=true)const
  {
    vec2d dp=vec2d_zero;
    auto dir_x=vec2d(1,0);
    auto dir_y=vec2d(0,1);
    #define F(dir,key_a,key_b)if((arrows&&Down[key_a])||(wasd&&(Down[key_b]))){dp+=dir;}
    F(-dir_x,VK_LEFT,'A');
    F(+dir_x,VK_RIGHT,'D');
    F(+dir_y,VK_UP,'W');
    F(-dir_y,VK_DOWN,'S');
    #undef F
    return dp;
  }
  bool operator()(const string&hotkey)const{
    if(hotkey.size()==1)return OnDown(hotkey[0]);
    auto arr=split(hotkey,"+");
    QapAssert(arr.size()==2&&arr[1].size()==1);
    bool ok=false;
    auto f=[&](const char*p,int key){if(LowerStr(p)==LowerStr(arr[0]))ok=Down(key);};
    f("ALT",VK_MENU);
    f("CTRL",VK_CONTROL);
    f("SHIFT",VK_SHIFT);
    return ok&&OnDown(arr[1][0]);
  }
};
enum TMouseButton
{
  mbLeft=257,
  mbRight=258,
  mbMiddle=259,
};

#include "qap_sys.inl"

class QapD3DPresentParameters
{
public:
public:
  typedef QapD3DPresentParameters SelfClass;
public:
  D3DPRESENT_PARAMETERS pp;
public:
  void DoReset()
  {
    {
    }
    (this->pp)=(Zero(pp));
  }
public:
  QapD3DPresentParameters(const QapD3DPresentParameters&)
  {
    QapDebugMsg("QapD3DPresentParameters"" is noncopyable");
    DoReset();
  };
  QapD3DPresentParameters()
  {
    DoReset();
  };
public:
public:
  template<class TYPE>
  static TYPE&Zero(TYPE&inout)
  {
    ZeroMemory(&inout,sizeof(inout));
    return inout;
  }
  D3DPRESENT_PARAMETERS&SetToDef(HWND hWnd,bool VSync=true)
  {
    Zero(pp);
    TScreenMode SM=GetScreenMode();
    pp.BackBufferWidth=SM.W;
    pp.BackBufferHeight=SM.H;
    pp.BackBufferFormat=D3DFMT_X8R8G8B8;
    pp.BackBufferCount=1;
    pp.SwapEffect=D3DSWAPEFFECT_COPY;
    pp.PresentationInterval=VSync?D3DPRESENT_INTERVAL_ONE:D3DPRESENT_INTERVAL_IMMEDIATE;
    pp.hDeviceWindow=hWnd;
    pp.Windowed=true;
    pp.FullScreen_RefreshRateInHz=pp.Windowed?0:SM.Freq;
    return pp;
  }
};
class QapD3D9
{
public:
public:
  typedef QapD3D9 SelfClass;
public:
  IDirect3D9* pD3D;
public:
  void DoReset()
  {
    {
    }
    (this->pD3D)=(nullptr);
  }
public:
  QapD3D9(const QapD3D9&)
  {
    QapDebugMsg("QapD3D9"" is noncopyable");
    DoReset();
  };
  QapD3D9()
  {
    DoReset();
  };
public:
public:
  QapD3D9(QapD3D9&&Ref)
  {
    DoReset();
    oper_set(std::move(Ref));
  }
  ~QapD3D9()
  {
    Free();
  }
public:
  void oper_set(QapD3D9&&Ref)
  {
    Free();
    this->pD3D=Ref.pD3D;
    Ref.pD3D=nullptr;
  }
public:
  void operator=(QapD3D9&&Ref)
  {
    oper_set(std::move(Ref));
  }
public:
  void Init()
  {
    Free();
    pD3D=Direct3DCreate9(D3D_SDK_VERSION);
    if(!pD3D)
    {
      QapDebugMsg("Cannot create Direct3D9");
      return;
    }
  }
  void Free()
  {
    if(pD3D)pD3D->Release();
  }
};
class QapD3D9Resource
{
public:
  virtual void OnLost()=0;
  virtual void OnReset()=0;
};
class QapD3D9ResourceList
{
public:
public:
  typedef QapD3D9ResourceList SelfClass;
public:
  vector<QapD3D9Resource*> Arr;
  bool Lost;
public:
  void DoReset()
  {
    {
    }
    (this->Arr)=(vector<QapD3D9Resource*>());
    (this->Lost)=(false);
  }
public:
  QapD3D9ResourceList(const QapD3D9ResourceList&)
  {
    QapDebugMsg("QapD3D9ResourceList"" is noncopyable");
    DoReset();
  };
  QapD3D9ResourceList()
  {
    DoReset();
  };
public:
public:
  QapD3D9ResourceList(QapD3D9ResourceList&&Ref)
  {
    DoReset();
    oper_set(std::move(Ref));
  }
  ~QapD3D9ResourceList()
  {
    Free();
  }
public:
  void oper_set(QapD3D9ResourceList&&Ref)
  {
    Free();
    {
      this->Arr=std::move(Ref.Arr);
    }
    {
      this->Lost=std::move(Ref.Lost);
    }
  }
public:
  void operator=(QapD3D9ResourceList&&Ref)
  {
    oper_set(std::move(Ref));
  }
public:
  void Free()
  {
    Lost=false;
    QapAssert(Arr.empty());
  }
public:
  void OnLost()
  {
    Lost=true;
    for (int i=0;i<Arr.size();i++)
    {
      auto&ex=Arr[i];
      ex->OnLost();
    }
  }
  void OnReset()
  {
    Lost=false;
    for (int i=0;i<Arr.size();i++)
    {
      auto&ex=Arr[i];
      ex->OnReset();
    }
  }
public:
  bool HasRes(QapD3D9Resource*pRes,int*pIndex=nullptr)
  {
    for (int i=0;i<Arr.size();i++)
    {
      auto&ex=Arr[i];
      if(ex!=pRes)continue;
      if(pIndex)*pIndex=i;
      return true;
    }
    if(pIndex)*pIndex=-1;
    return false;
  }
  void Reg(QapD3D9Resource*pRes)
  {
    QapAssert(bool(pRes));
    QapAssert(!HasRes(pRes));
    Arr.push_back(pRes);
  }
  void UnReg(QapD3D9Resource*pRes)
  {
    int i=-1;
    bool flag=HasRes(pRes,&i);
    if(!flag)
    {
      QapAssert(flag);
      return;
    }
    std::swap(Arr[i],Arr.back());
    Arr.pop_back();
  }
};
struct QapMat4:public D3DMATRIX{
  QapMat4(){}
  QapMat4(
    float m00,float m01,float m02,float m03,
    float m10,float m11,float m12,float m13,
    float m20,float m21,float m22,float m23,
    float m30,float m31,float m32,float m33
  ){
    m[0][0]=m00; m[0][1]=m01; m[0][2]=m02; m[0][3]=m03;
    m[1][0]=m10; m[1][1]=m11; m[1][2]=m12; m[1][3]=m13;
    m[2][0]=m20; m[2][1]=m21; m[2][2]=m22; m[2][3]=m23;
    m[3][0]=m30; m[3][1]=m31; m[3][2]=m32; m[3][3]=m33;
  }
  friend QapMat4 operator+(const QapMat4&m,const QapMat4&n){
    QapMat4 O;
    for(int i=0;i<4;i++)for(int j=0;j<4;j++)O.m[i][j]=m.m[i][j]+n.m[i][j];
    return O;
  }
  friend QapMat4 operator-(const QapMat4&m,const QapMat4&n){
    QapMat4 O;
    for(int i=0;i<4;i++)for(int j=0;j<4;j++)O.m[i][j]=m.m[i][j]-n.m[i][j];
    return O;
  }
  friend QapMat4 operator-(const QapMat4&m){
    QapMat4 O;
    for(int i=0;i<4;i++)for(int j=0;j<4;j++)O.m[i][j]=-m.m[i][j];
    return O;
  }
  friend QapMat4 operator*(const QapMat4&m,const QapMat4&n){
    return QapMat4(
      m.m[0][0]*n.m[0][0]+m.m[0][1]*n.m[1][0]+m.m[0][2]*n.m[2][0]+m.m[0][3]*n.m[3][0],
      m.m[0][0]*n.m[0][1]+m.m[0][1]*n.m[1][1]+m.m[0][2]*n.m[2][1]+m.m[0][3]*n.m[3][1],
      m.m[0][0]*n.m[0][2]+m.m[0][1]*n.m[1][2]+m.m[0][2]*n.m[2][2]+m.m[0][3]*n.m[3][2],
      m.m[0][0]*n.m[0][3]+m.m[0][1]*n.m[1][3]+m.m[0][2]*n.m[2][3]+m.m[0][3]*n.m[3][3],
      m.m[1][0]*n.m[0][0]+m.m[1][1]*n.m[1][0]+m.m[1][2]*n.m[2][0]+m.m[1][3]*n.m[3][0],
      m.m[1][0]*n.m[0][1]+m.m[1][1]*n.m[1][1]+m.m[1][2]*n.m[2][1]+m.m[1][3]*n.m[3][1],
      m.m[1][0]*n.m[0][2]+m.m[1][1]*n.m[1][2]+m.m[1][2]*n.m[2][2]+m.m[1][3]*n.m[3][2],
      m.m[1][0]*n.m[0][3]+m.m[1][1]*n.m[1][3]+m.m[1][2]*n.m[2][3]+m.m[1][3]*n.m[3][3],
      m.m[2][0]*n.m[0][0]+m.m[2][1]*n.m[1][0]+m.m[2][2]*n.m[2][0]+m.m[2][3]*n.m[3][0],
      m.m[2][0]*n.m[0][1]+m.m[2][1]*n.m[1][1]+m.m[2][2]*n.m[2][1]+m.m[2][3]*n.m[3][1],
      m.m[2][0]*n.m[0][2]+m.m[2][1]*n.m[1][2]+m.m[2][2]*n.m[2][2]+m.m[2][3]*n.m[3][2],
      m.m[2][0]*n.m[0][3]+m.m[2][1]*n.m[1][3]+m.m[2][2]*n.m[2][3]+m.m[2][3]*n.m[3][3],
      m.m[3][0]*n.m[0][0]+m.m[3][1]*n.m[1][0]+m.m[3][2]*n.m[2][0]+m.m[3][3]*n.m[3][0],
      m.m[3][0]*n.m[0][1]+m.m[3][1]*n.m[1][1]+m.m[3][2]*n.m[2][1]+m.m[3][3]*n.m[3][1],
      m.m[3][0]*n.m[0][2]+m.m[3][1]*n.m[1][2]+m.m[3][2]*n.m[2][2]+m.m[3][3]*n.m[3][2],
      m.m[3][0]*n.m[0][3]+m.m[3][1]*n.m[1][3]+m.m[3][2]*n.m[2][3]+m.m[3][3]*n.m[3][3]
    );
  }
  friend QapMat4 operator*(const QapMat4&m,float x){
    QapMat4 O;
    for(int i=0;i<4;i++)for(int j=0;j<4;j++)O.m[i][j]=x*m.m[i][j];
    return O;
  }
};
inline QapMat4 Transpose(const QapMat4&m){
  return QapMat4(
    m.m[0][0],m.m[1][0],m.m[2][0],m.m[3][0],
    m.m[0][1],m.m[1][1],m.m[2][1],m.m[3][1],
    m.m[0][2],m.m[1][2],m.m[2][2],m.m[3][2],
    m.m[0][3],m.m[1][3],m.m[2][3],m.m[3][3]
  );
}
inline QapMat4 RotateZ(float angle){
  float c=cosf(angle),s=sinf(angle);
  return QapMat4(
    +c,+s,+0,+0,
    -s,+c,+0,+0,
    +0,+0,+1,+0,
    +0,+0,+0,+1
  );
}
inline QapMat4 Translate(float x,float y,float z){
  return QapMat4(
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    x,y,z,1
  );
}
inline QapMat4 Scale(float x,float y,float z){
  return QapMat4(
    x,0,0,0,
    0,y,0,0,
    0,0,z,0,
    0,0,0,1
  );
}
inline QapMat4 Identity4(){
  return QapMat4(
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
  );
}
class QapD3DDev9
{
public:
public:
  typedef QapD3DDev9 SelfClass;
public:
  QapD3DPresentParameters PresParams;
  IDirect3DDevice9* pDev;
  QapD3D9ResourceList Resources;
  bool DrawPass;
public:
  void DoReset()
  {
    {
    }
    (this->PresParams)=(QapD3DPresentParameters());
    (this->pDev)=(nullptr);
    (this->Resources)=(QapD3D9ResourceList());
    (this->DrawPass)=(false);
  }
public:
  QapD3DDev9(const QapD3DDev9&)
  {
    QapDebugMsg("QapD3DDev9"" is noncopyable");
    DoReset();
  };
  QapD3DDev9()
  {
    DoReset();
  };
public:
public:
  QapD3DDev9(QapD3DDev9&&Ref)
  {
    DoReset();
    oper_set(std::move(Ref));
  }
  ~QapD3DDev9()
  {
    Free();
  }
public:
  void oper_set(QapD3DDev9&&Ref)
  {
    Free();
    {
      this->PresParams=std::move(Ref.PresParams);
    }
    {
      this->pDev=std::move(Ref.pDev);
      Ref.pDev=nullptr;
    }
    {
      this->Resources=std::move(Ref.Resources);
    }
  }
public:
  void operator=(QapD3DDev9&&Ref)
  {
    oper_set(std::move(Ref));
  }
public:
  void Init(HWND hWnd,QapD3D9&D3D)
  {
    QapAssert(D3D.pD3D);
    Free();
    D3D.pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&PresParams.pp,&pDev);
    if(!pDev)
    {
      QapDebugMsg("Cannot create Direct3DDevice9");
      return;
    }
  }
  void Free()
  {
    if(pDev)pDev->Release();
  }
public:
  void ResetStates()
  {
    pDev->SetRenderState(D3DRS_LIGHTING,false);
    pDev->SetRenderState(D3DRS_ZENABLE,false);
    pDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
    pDev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
    pDev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
    pDev->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
    pDev->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
    pDev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
  }
public:
  bool BeginScene()
  {
    if(Resources.Lost)
    {
      EndScene();
      return false;
    };
    pDev->BeginScene();
    DrawPass=true;
    ResetStates();
    return true;
  }
  bool EndScene()
  {
    DrawPass=false;
    if(Resources.Lost)
    {
      HRESULT hr=pDev->TestCooperativeLevel();
      if(hr!=D3DERR_DEVICENOTRESET)return false;
      pDev->Reset(&PresParams.pp);
      Resources.OnReset();
      return false;
    };
    pDev->EndScene();
    return !Resources.Lost;
  }
  bool Present(const RECT*pSource=nullptr,const RECT*pDest=nullptr)
  {
    if(!Resources.Lost)
    {
      Resources.Lost=D3DERR_DEVICELOST==pDev->Present(pSource,pDest,NULL,NULL);
      if(Resources.Lost)Resources.OnLost();
    }
    return !Resources.Lost;
  }
public:
  enum BlendType
  {
    BT_NONE,
    BT_SUB,
    BT_ADD,
  };
  enum AlphaType
  {
    AM_NONE,
    AM_GEQUAL_ONE,
    AM_NEQUAL_MAX,
  };
public:
  void Blend(BlendType Mode)
  {
    pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,Mode!=BT_NONE);
    if(Mode==BT_NONE)return;
    if(Mode==BT_SUB)
    {
      pDev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
      pDev->SetTextureStageState(0, D3DTSS_COLOROP,  D3DTOP_MODULATE);
      pDev->SetTextureStageState(0, D3DTSS_COLORARG1,D3DTA_TEXTURE);
      pDev->SetTextureStageState(0, D3DTSS_COLORARG2,D3DTA_DIFFUSE);
      pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE);
      pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
      pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
      pDev->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
      pDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
      pDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
      return;
    }
    if(Mode==BT_ADD)
    {
      pDev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
      pDev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
      return;
    }
  }
  void Alpha(AlphaType Mode)
  {
    pDev->SetRenderState(D3DRS_ALPHATESTENABLE,Mode!=AM_NONE);
    if(Mode==AM_NONE)return;
    if(Mode==AM_GEQUAL_ONE)
    {
      pDev->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
      pDev->SetRenderState(D3DRS_ALPHAREF,1);
      return;
    }
    if(Mode==AM_NEQUAL_MAX)
    {
      pDev->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_NOTEQUAL);
      pDev->SetRenderState(D3DRS_ALPHAREF,255);
      return;
    }
  }
public:
  static void OrthoLH(D3DMATRIX&out,float w,float h,float zn,float zf)
  {
    QapMat4 mat=QapMat4(
          2.0/w,0,0,0,
          0,2.0/h,0,0,
          0,0,1/(zf-zn),0,
          0,0,-zn/(zf-zn),1
        );
    out=mat*Identity4();
  }
  void Set2D(vec2d CamPos=vec2d(0.0,0.0),real zoom=1.0,real ang=0.0,vec2i*pSize=nullptr)
  {
    auto&pp=PresParams.pp;
    vec2i v=pSize?*pSize:vec2i(pp.BackBufferWidth,pp.BackBufferHeight);
    QapMat4 matProj(Identity4());
    QapMat4 matView(Identity4());
    if((v.x%2)||(v.y%2))
    {
      if(v.x%2)CamPos.x+=0.5f;
      if(v.y%2)CamPos.y+=0.5f;
    }
    QapMat4 matWorld(Translate(-CamPos.x,-CamPos.y,0)*Scale(zoom,zoom,1.0)*RotateZ(ang));
    OrthoLH(matProj,float(v.x),float(v.y),-1.0,1.0);
    pDev->SetTransform(D3DTS_PROJECTION,&matProj);
    pDev->SetTransform(D3DTS_VIEW,&matView);
    pDev->SetTransform(D3DTS_WORLD,&matWorld);
  }
  void Clear2D(const QapColor&Color)
  {
    pDev->Clear(0,NULL,D3DCLEAR_TARGET,Color,1.0f,0);
  }
};

template<class TYPE>
struct vector_view{
  const TYPE*p;
  unsigned n;
  vector_view<TYPE>():p(nullptr),n(0){}
  const TYPE&operator[](int id)const{return p[id];}
  int size()const{return n;}
  bool empty()const{return !n;}
  vector_view<TYPE>(const vector<TYPE>&ref,int start,int n):p(nullptr),n(0)
  {
    if(!n)return;
    QapAssert(qap_check_id(ref,start));
    QapAssert(qap_check_id(ref,start+n-1));
    p=n?&ref[start]:nullptr;
    this->n=n;
  }
  vector_view<TYPE>(const vector<TYPE>&ref):p(nullptr),n(0)
  {
    if(!ref.size())return;
    p=&ref[0];n=ref.size();
  }
  template<size_t N>vector_view(const array<TYPE,N>&ref):p(&ref[0]),n(N){}
  template<size_t N>vector_view<TYPE>(const TYPE(&ref)[N]):p(&ref[0]),n(N){}
};

template<class TYPE>
vector_view<TYPE> make_view(vector<TYPE>&ref,int start,int n)
{
  vector_view<TYPE> vv;if(!n)return vv;
  QapAssert(qap_check_id(ref,start));
  QapAssert(qap_check_id(ref,start+n-1));
  vv.p=n?&ref[start]:nullptr;
  vv.n=n;
  return vv;
}
template<class TYPE>
vector_view<TYPE> make_view(vector<TYPE>&ref)
{
  vector_view<TYPE> vv={nullptr,0};if(!ref.size())return vv;
  vv.p=&ref[0];
  vv.n=ref.size();
  return vv;
}

template<class TYPE>
const vector_view<TYPE> make_view(const vector<TYPE>&ref){return ref;}

template<class TYPE,size_t N>
const vector_view<TYPE> make_view(const array<TYPE,N>&ref){return ref;}

template<class TYPE,size_t N>
const vector_view<TYPE> make_view(const TYPE(&ref)[N]){return ref;}

#include "t_quad.inl"
#pragma once
#pragma warning(disable:4482)
class QapDev
{
public:
  class TDynamicResource:public QapD3D9Resource
  {
  public:
    typedef QapDev OwnerClass;
    void operator=(OwnerClass*pOwner)
    {
      this->pOwner=pOwner;
    }
  public:
    TDynamicResource():pOwner(nullptr),flag(false),pDev(nullptr)
    {
      if(pDev)pDev->Resources.Reg(this);
    };
    ~TDynamicResource()
    {
      if(pDev)pDev->Resources.UnReg(this);
    };
  public:
    QapD3DDev9*pDev;
    void Mount(OwnerClass*pOwner,QapD3DDev9*pDev)
    {
      QapAssert(pOwner);
      QapAssert(pDev);
      this->pOwner=pOwner;
      this->pDev=pDev;
      if(pDev)pDev->Resources.Reg(this);
    }
  public:
    OwnerClass*pOwner;
    bool flag;
    void OnLost()
    {
      if(pOwner)
      {
        flag=bool(pOwner->VB);
        pOwner->Free();
      }
      else
      {
        QapDebugMsg("fail");
        flag=false;
      }
    }
    void OnReset()
    {
      if(pOwner&&flag)pOwner->ReInit();
    }
  };
public:
  struct Ver
  {
    float x,y,z;
    QapColor color;
    float tu,tv;
    Ver():x(0),y(0),z(0),tu(0),tv(0) {}
    Ver(float x,float y,const QapColor&color,float u=0.0,float v=0.0):x(x),y(y),z(0),color(color),tu(u),tv(v) {}
    Ver(const vec2f&pos,const QapColor&color,float u=0.0,float v=0.0):x(pos.x),y(pos.y),z(0),color(color),tu(u),tv(v) {}
    Ver(const vec2f&pos,const QapColor&color,const vec2f&texcoord):x(pos.x),y(pos.y),z(0),color(color),tu(texcoord.x),tv(texcoord.y) {}
    vec2f&get_pos()
    {
      return *(vec2f*)&x;
    }
    vec2f&get_pos()const
    {
      return *(vec2f*)&x;
    }
    vec2f&get_tpos()
    {
      return *(vec2f*)&tu;
    }
    vec2f&get_tpos()const
    {
      return *(vec2f*)&tu;
    }
  };
  struct BatchScope
  {
    QapDev&RD;
    bool flag;
    BatchScope(QapDev&RD):RD(RD)
    {
      flag=!RD.Batching;
      if(flag)RD.BeginBatch();
    }
    ~BatchScope()
    {
      if(flag)RD.EndBatch();
    }
  };
  struct BatchScopeEx
  {
    QapDev&RD;
    bool flag;
    bool drawpass;
    BatchScopeEx(QapDev&RD,bool drawpass):RD(RD)
    {
      if(!drawpass)return;
      flag=!RD.Batching;
      if(flag)RD.BeginBatch();
    }
    ~BatchScopeEx()
    {
      if(!drawpass)return;
      if(flag)RD.EndBatch();
    }
  };
  struct ViewportScope
  {
    QapDev&RD;
    const t_quad&viewport;
    bool auto_clamp;
    int vpos;
    ViewportScope(QapDev&RD,const t_quad&viewport):RD(RD),viewport(viewport),vpos(0)
    {
      vpos=0;
      auto_clamp=RD.auto_clamp;
      if(auto_clamp)vpos=RD.VPos;
      RD.push_viewport(viewport);
    }
    ~ViewportScope()
    {
      if(auto_clamp)
      {
        for(int i=vpos;i<RD.VPos;i++)
        {
          auto&p=RD.VBA[i].get_pos();
          p=RD.viewport.clamp(p);
        }
      }
      RD.pop_viewport();
    }
  };
public:
  typedef QapD3DDev9::BlendType BlendType;
  typedef QapD3DDev9::AlphaType AlphaType;
public:
public:
  typedef QapDev SelfClass;
public:
  TDynamicResource DynRes;
  IDirect3DDevice9* pDev;
  IDirect3DVertexBuffer9* VB;
  IDirect3DIndexBuffer9* IB;
  Ver* VBA;
  int* IBA;
  QapColor color;
  int VPos;
  int IPos;
  int MaxVPos;
  int MaxIPos;
  int DIPs;
  int Verts;
  int Tris;
  BlendType BlendMode;
  AlphaType AlphaMode;
  bool Batching;
  bool Textured;
  transform2f xf;
  transform2f txf;
  vector<t_quad> stack;
  t_quad viewport;
  bool use_viewport;
  bool use_xf;
  bool auto_clamp;
  bool after_reinit;
public:
  void DoReset()
  {
    {
    }
    (this->DynRes)=(this);
    (this->pDev)=(nullptr);
    (this->VB)=(nullptr);
    (this->IB)=(nullptr);
    (this->VBA)=(nullptr);
    (this->IBA)=(nullptr);
    (this->color)=(0xFFFFFFFF);
    (this->VPos)=(0);
    (this->IPos)=(0);
    (this->MaxVPos)=(0);
    (this->MaxIPos)=(0);
    (this->DIPs)=(0);
    (this->Verts)=(0);
    (this->Tris)=(0);
    (this->BlendMode)=(BlendType::BT_SUB);
    (this->AlphaMode)=(AlphaType::AM_NONE);
    (this->Batching)=(false);
    (this->Textured)=(false);
    detail::FieldTryDoReset(this->xf);
    detail::FieldTryDoReset(this->txf);
    //... added ...
    (this->use_viewport=false);
    (this->use_xf=false);
    (this->auto_clamp=false);
    (this->after_reinit=false);
  }
public:
  QapDev(const QapDev&)
  {
    QapDebugMsg("QapDev"" is noncopyable");
    DoReset();
  };
  QapDev()
  {
    DoReset();
  };
public:
protected:
  static const DWORD FVF=D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1;
public:
  ~QapDev()
  {
    Free();
  }
public:
  bool OnReinit(){auto f=after_reinit;after_reinit=false;return f;}
  void ReInit()
  {
    Init(MaxVPos,MaxIPos);after_reinit=true;
  }
  void MountDev(QapD3DDev9&Dev)
  {
    this->pDev=Dev.pDev;
    this->DynRes.Mount(this,&Dev);
  }
  void Init(int VCount,int ICount)
  {
    if(!VB)
    {
      MaxVPos=VCount;
      MaxIPos=ICount;
      pDev->CreateVertexBuffer(VCount*sizeof(Ver),D3DUSAGE_DYNAMIC,FVF,D3DPOOL_DEFAULT,&VB,NULL);
      pDev->CreateIndexBuffer(ICount*sizeof(int),D3DUSAGE_DYNAMIC,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&IB,NULL);
      VBA=0;
      IBA=0;
      VPos=0;
      IPos=0;
      DIPs=0;
      Verts=0;
      Tris=0;
      xf.set_ident();
      txf.set_ident();
    };
  }
  void Free()
  {
    if(VB)
    {
      VB->Release();
      VB=nullptr;
      VBA=nullptr;
    }
    if(IB)
    {
      IB->Release();
      IB=nullptr;
      IBA=nullptr;
    }
    VPos=0;
    IPos=0;
    Batching=false;
    BlendMode=BlendType::BT_SUB;
    AlphaMode=AlphaType::AM_NONE;
  };
public:
  bool DrawPass(){return this->DynRes.pDev->DrawPass;}
  void BeginBatch()
  {
    QapAssert(DrawPass());
    Batching=true;
    VBA=0;
    IBA=0;
    VPos=0;
    IPos=0;
    if(!IB||!VB)return;
    IB->Lock(0,sizeof(int)*MaxIPos,(void **)&IBA,0);
    VB->Lock(0,sizeof(Ver)*MaxVPos,(void **)&VBA,0);
  };
  void EndBatch(bool call_dip=true)
  {
    Batching=false;
    if(!IB||!VB)return;
    IB->Unlock();
    VB->Unlock();
    if(VPos&&IPos)
    {
      if(call_dip)DIP();
    }
    VBA=0;
    IBA=0;
  }
  void DIP()
  {
    pDev->SetFVF(FVF);
    pDev->SetStreamSource(0,VB,0,sizeof(Ver));
    pDev->SetIndices(IB);
    pDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,VPos,0,IPos/3);
    DIPs++;
    Verts+=VPos;
    Tris+=IPos/3;
  }
  bool IsBatching()
  {
    return Batching;
  }
  int GetIPos()
  {
    return IPos;
  }
  int GetVPos()
  {
    return VPos;
  }
  int GetDIPs()
  {
    return DIPs;
  }
  int GetVerts()
  {
    return Verts;
  }
  int GetTris()
  {
    return Tris;
  }
  const QapColor&GetColor()
  {
    return color;
  }
  void push_viewport(const t_quad&quad)
  {
    stack.push_back(viewport);
    viewport=quad;
  }
  void pop_viewport()
  {
    QapAssert(!stack.empty());
    viewport=stack.back();
    stack.pop_back();
  }
  void NextFrame()
  {
    QapAssert(stack.empty());
    DIPs=0;
    Verts=0;
    Tris=0;
    use_xf=false;
    use_viewport=false;
    SetBlendMode(BlendMode);
    SetAlphaMode(AlphaMode);
  }
public:
  void HackMode(bool Textured)
  {
    this->Textured=Textured;
  }
  //template<class QAP_TEX>
  //void BindTex(int Stage,QAP_TEX*pTex)
  //{
  //  QapTex*Tex=pTex;
  //  pDev->SetTexture(Stage,Tex?Tex->Tex:nullptr);
  //  txf.set_ident();
  //  Textured=Tex?true:false;
  //}
  void BindTex(int Stage,const nullptr_t)
  {
    pDev->SetTexture(Stage,nullptr);
    Textured=false;
  }
public:
  inline Ver&AddVertexRaw()
  {
    return VBA[VPos++];
  }
  inline int AddVertex(const Ver&Source)
  {
    QapAssert(VPos<MaxVPos);
    Ver&Dest=VBA[VPos];
    Dest=Source;
    if(use_xf)
    {
      vec2f&v=Dest.get_pos();
      v=xf*v;
    }
    if(use_viewport)
    {
      vec2f&v=Dest.get_pos();
      v+=viewport.pos;
    }
    if(Textured)
    {
      vec2f&v=Dest.get_tpos();
      v=txf*v;
    }
    return VPos++;
  }
  inline int AddVertex(float x,float y,const QapColor&c,float u,float v)
  {
    QapDev::Ver tmp;
    tmp.x=x;
    tmp.y=y;
    tmp.z=0;
    tmp.color=c;
    tmp.tu=u;
    tmp.tv=v;
    auto id=AddVertex(tmp);
    return id;
  }
  inline int AddVertex(const vec2f&pos,const QapColor&c,float u,float v)
  {
    QapDev::Ver tmp;
    tmp.get_pos()=pos;
    tmp.z=0;
    tmp.color=c;
    tmp.tu=u;
    tmp.tv=v;
    auto id=AddVertex(tmp);
    return id;
  }
  inline int AddVertex(const vec2f&pos,const QapColor&c,const vec2f&tpos)
  {
    QapDev::Ver tmp;
    tmp.get_pos()=pos;
    tmp.z=0;
    tmp.color=c;
    tmp.get_tpos()=tpos;
    auto id=AddVertex(tmp);
    return id;
  }
  inline void AddTris(int A,int B,int C)
  {
    QapAssert(IPos+2<MaxIPos);
    QapAssert(A>=0&&A<MaxVPos);
    QapAssert(B>=0&&B<MaxVPos);
    QapAssert(C>=0&&C<MaxVPos);
    IBA[IPos++]=A;
    IBA[IPos++]=B;
    IBA[IPos++]=C;
  };
public:
  inline void SetColor(const QapColor&C)
  {
    color=C;
  }
  inline void SetTransform(transform2f const&val)
  {
    xf=val;
  }
  inline transform2f GetTransform()
  {
    return xf;
  }
  inline void SetTextureTransform(transform2f const&val)
  {
    txf=val;
  }
  inline transform2f GetTextureTransform()
  {
    return txf;
  }
public:
  BlendType GetBlendMode()
  {
    return BlendMode;
  }
  AlphaType GetAlphaMode()
  {
    return AlphaMode;
  }
  void SetBlendMode(BlendType Mode)
  {
    DynRes.pDev->Blend(BlendMode=Mode);
  }
  void SetAlphaMode(AlphaType Mode)
  {
    DynRes.pDev->Alpha(AlphaMode=Mode);
  }
public:
  void DrawRect(const vec2d&a,const vec2d&b,int line_size)
  {
    BatchScope Scope(*this);
    {
      real x=(a.x+b.x)*0.5;
      real w=fabs(a.x-b.x);
      DrawQuad(x,a.y,line_size+w,line_size);
      DrawQuad(x,b.y,line_size+w,line_size);
      real y=(a.y+b.y)*0.5;
      real h=fabs(a.y-b.y);
      DrawQuad(a.x,y,line_size,-line_size+h);
      DrawQuad(b.x,y,line_size,-line_size+h);
    }
    int gg=1;
  }
  void DrawRectAsQuad(const vec2d&pos,const vec2d&size,int line_size)
  {
    auto hs=size*0.5;
    DrawRect(pos-hs,pos+hs,line_size);
  }
  void DrawRectAsQuad(const vec2d&pos,const vec2d&size,real line_size,real ang)
  {
    auto OZ=Vec2dEx(ang,1.0);auto w=size.x;auto h=size.y;
    vec2d PA[]={
      pos+vec2d(-0.5f*w,-0.5f*h).UnRot(OZ),
      pos+vec2d(+0.5f*w,-0.5f*h).UnRot(OZ),
      pos+vec2d(+0.5f*w,+0.5f*h).UnRot(OZ),
      pos+vec2d(-0.5f*w,+0.5f*h).UnRot(OZ)
    };
    DrawPolyLine(PA,line_size,true);
  }
  void DrawQuadWithHalfPixelOffset(int x,int y,int w,int h)
  {
    DrawQuad(float(x)+0.5,float(y)-0.5,float(w),float(h));
  }
  void DrawQuadWithHalfPixelOffset(const vec2i&p,int w,int h)
  {
    DrawQuadWithHalfPixelOffset(p.x,p.y,w,h);
  }
  void AddQuadsIndices(int vpos,int n){
    int p=vpos;
    for(int i=0;i<n;i++){
      AddTris(p+1,p+0,p+3);
      AddTris(p+3,p+2,p+1);
      p+=4;
    }
  }
  void AddQuadVertices(float x,float y,float half_w,float half_h)
  {  
    BatchScope Scope(*this);
    {
      #define F(X,Y,U,V){AddVertex(x+(X##half_w),y+(Y##half_h),color,U,V);}
      //vec2d O(x,y);
      //int n=VPos;
      QapAssert(vec2d(-1,-1).Ort()==vec2d(+1,-1));
      F(-,-,0.0f,1.0f);
      F(+,-,1.0f,1.0f);
      F(+,+,1.0f,0.0f);
      F(-,+,0.0f,0.0f); 
      //AddTris(n+1,n+0,n+3);
      //AddTris(n+3,n+2,n+1);
      #undef F
    }
  }
  void DrawQuad(float x,float y,float w,float h)
  {  
    BatchScope Scope(*this);
    {
      #define F(X,Y,U,V){AddVertex(x+(X)*w,y+(Y)*h,color,U,V);}
      vec2d O(x,y);
      int n=VPos;
      F(-0.5f,-0.5f,0.0f,1.0f);
      F(+0.5f,-0.5f,1.0f,1.0f);
      F(+0.5f,+0.5f,1.0f,0.0f);
      F(-0.5f,+0.5f,0.0f,0.0f); 
      AddTris(n+1,n+0,n+3);
      AddTris(n+3,n+2,n+1);
      #undef F
    }
  }
  void DrawQuad(float x,float y,float w,float h,float a)
  {
    BatchScope Scope(*this);
    {
      vec2d OZ=Vec2dEx(a,1.0);
      vec2d O(x,y);
      int n=VPos;
      {
        AddVertex(vec2f(O+vec2d(-0.5f*w,-0.5f*h).UnRot(OZ)),color,0.0f,1.0f);
      };
      {
        AddVertex(vec2f(O+vec2d(+0.5f*w,-0.5f*h).UnRot(OZ)),color,1.0f,1.0f);
      };
      {
        AddVertex(vec2f(O+vec2d(+0.5f*w,+0.5f*h).UnRot(OZ)),color,1.0f,0.0f);
      };
      {
        AddVertex(vec2f(O+vec2d(-0.5f*w,+0.5f*h).UnRot(OZ)),color,0.0f,0.0f);
      };
      AddTris(n+1,n+0,n+3);
      AddTris(n+3,n+2,n+1);
    }
  }
  void DrawQuad(float x,float y,const vec2d&size,float ang)
  {
    DrawQuad(x,y,size.x,size.y,ang);
  }
  void DrawQuad(const vec2d&pos,const vec2d&size,float ang)
  {
    DrawQuad(pos.x,pos.y,size.x,size.y,ang);
  }
  void DrawQuad(const vec2d&pos,const vec2d&size)
  {
    DrawQuad(pos.x,pos.y,size.x,size.y);
  }
  void DrawQuad(const vec2d&pos,float w,float h,float ang)
  {
    DrawQuad(pos.x,pos.y,w,h,ang);
  }
  void DrawQuad(const vec2d&pos,float w,float h)
  {
    DrawQuad(pos.x,pos.y,w,h);
  }
  void DrawTrigon(const vec2d&A,const vec2d&B,const vec2d&C)
  {
    BatchScope Scope(*this);
    {
      AddTris(
        AddVertex(Ver(A,color)),
        AddVertex(Ver(B,color)),
        AddVertex(Ver(C,color))
      );
    }
  }
  void DrawConvex(const vector<vec2d>&Points)
  {
    BatchScope Scope(*this);
    {
      if(Points.empty())return;
      int c=Points.size();
      int n=VPos;
      for (int i=0;i<c;i++)AddVertex(Points[i],color,0.5,0.5);
      for (int i=2;i<c;i++)AddTris(n,n+i-1,n+i-0);
    }
  }
public:
  void DrawDashLine(const vec2d&a,const vec2d&b,real dash_size,real line_size)
  {
    if(a.dist_to(b)<dash_size){DrawLine(a,b,line_size);return;}
    auto o=(a+b)*0.5;
    auto ob=b-o;
    auto ob05=o+ob.SetMag(dash_size*0.5);
    auto ob15=o+ob.SetMag(dash_size*1.5);
    DrawDashLineFromA(ob05,a,dash_size,line_size);
    if(dash_size*1.5<ob.Mag())DrawDashLineFromA(ob15,b,dash_size,line_size);
  }
  void DrawDashLineFromA(const vec2d&a,const vec2d&b,real dash_size,real line_size)
  {
    real k=dash_size;real inv_k=1.0/k;
    vector<vec2d> arr;int c=int(a.dist_to(b)*inv_k);
    for(int i=0;i<=c;i++)qap_add_back(arr)=a+(b-a).SetMag(i*k);
    if(c%2==0)qap_add_back(arr)=b;
    DrawLineList(arr,line_size);
  }
  void DrawLine(const vec2d&a,const vec2d&b,real line_size){DrawQuad((b+a)*0.5,(b-a).Mag(),line_size,(b-a).GetAng());}
  void DrawSolidCircle(const vec2d&pos,real r,int seg,real ang){DrawCircleEx(pos,r,0,seg,ang);}
  template<typename TYPE>
  void DrawPolyLine2016(const vector<TYPE>&PA,const real&LineSize,const bool&Loop)
  {
    if(PA.empty())return;
    BatchScope Scope(*this);
    {
      int Count=PA.size();
      int c=Loop?Count:Count-1;
      for (int i=0;i<c;i++)
      {
        TYPE const&a=PA[(i+0)%Count];
        TYPE const&b=PA[(i+1)%Count];
        auto ab=vec2d(b-a).SetMag(LineSize*0.5);
        auto n=ab.Ort();auto az=a-ab;auto bz=b+ab;
        int A[4]=
        {
          AddVertex(az+n,color,0.5f,0.5f),
          AddVertex(bz-n,color,0.5f,0.5f),
          AddVertex(az-n,color,0.5f,0.5f),
          AddVertex(bz+n,color,0.5f,0.5f),
        };
        AddTris(A[0],A[1],A[2]);
        AddTris(A[0],A[1],A[3]);
      }
    }
  }
  //template<typename vector_TYPE>
  void DrawPolyLine(const vector_view<vec2d>&PA,const real&LineSize,const bool&Loop)
  {
    if(PA.empty())return;
    BatchScope Scope(*this);
    {
      int Count=PA.size();
      int c=Loop?Count:Count-1;
      for (int i=0;i<c;i++)
      {
        auto const&a=PA[(i+0)%Count];
        auto const&b=PA[(i+1)%Count];
        vec2d n=vec2d(b-a).Ort().SetMag(LineSize*0.5);
        int A[4]=
        {
          AddVertex(a+n,color,0.5f,0.5f),
          AddVertex(b-n,color,0.5f,0.5f),
          AddVertex(a-n,color,0.5f,0.5f),
          AddVertex(b+n,color,0.5f,0.5f),
        };
        AddTris(A[0],A[1],A[2]);
        AddTris(A[0],A[1],A[3]);
      }
    }
  }
  template<typename TYPE>
  void DrawLineList(const vector<TYPE>&PA,const real&LineSize)
  {
    if(PA.empty())return;
    BatchScope Scope(*this);
    {
      int Count=PA.size();
      for (int i=0;i+1<Count;i+=2)
      {
        TYPE const&a=PA[i+0];
        TYPE const&b=PA[i+1];
        TYPE n=vec2d(b-a).Ort().SetMag(LineSize*0.5);
        int A[4]=
        {
          AddVertex(a+n,color,0.5,0.5),
          AddVertex(b-n,color,0.5,0.5),
          AddVertex(a-n,color,0.5,0.5),
          AddVertex(b+n,color,0.5,0.5),
        };
        AddTris(A[0],A[1],A[2]);
        AddTris(A[0],A[1],A[3]);
      }
    }
  }
  template<typename TYPE>
  void DrawMesh(const vector<TYPE>&VA,const vector<int>&IA)
  {
    //QapDebugMsg("[2014.05.22]: way used?");
    if(VA.empty())return;
    BatchScope Scope(*this);
    {
      int base=GetVPos();
      static IntArray VID;
      VID.resize(VA.size());
      for (int i=0;i<VA.size();i++){auto&p=VA[i];VID[i]=AddVertex(p,color,p.x,p.y);}
      for (int i=0;i<IA.size();i+=3)AddTris(VID[IA[i+0]],VID[IA[i+1]],VID[IA[i+2]]);
    }
  }
  void DrawCircleOld(const vec2d&pos,real r,real ang,real ls,int seg)
  {
    static PointArray PA;
    PA.resize(seg);
    for (int i=0;i<seg;i++)
    {
      vec2d v=Vec2dEx((real)i/(real)seg*2.0*Pi,r);
      PA[i]=pos+v;
    };
    DrawPolyLine(PA,ls,true);
  }
  void DrawCircle(const vec2d&pos,real r,real ang,real ls,int seg)
  {
    DrawCircleEx(pos,r-ls*0.5,r+ls*0.5,seg,ang);
  }
  void DrawCircleEx(const vec2d&pos,real r0,real r1,int seg,real ang)
  {
    int n=seg;
    if(n<=0)return;
    BatchScope Scope(*this);
    static vector<int> VID;
    VID.resize(n*2);
    for (int i=0;i<n;i++)
    {
      vec2d v=Vec2dEx(ang+Pi2*(real(i)/real(n)),1);
      VID[0+i]=AddVertex(pos+v*r0,color,0,0);
      VID[n+i]=AddVertex(pos+v*r1,color,0,0);
    }
    for (int i=0;i<n;i++)
    {
      int a=VID[0+(i+0)%n];
      int b=VID[0+(i+1)%n];
      int c=VID[n+(i+0)%n];
      int d=VID[n+(i+1)%n];
      AddTris(a,b,d);
      AddTris(d,c,a);
    }
  }
public:
  #include "t_geom.inl"
};
template<class TYPE>
class QapArray
{
public:
  TMemory Mem;
  QapArray() {}
  ~QapArray()
  {
    Free();
  }
  QapArray(const QapArray<TYPE>&Ref)
  {
    oper_set(Ref);
  }
  void operator=(const QapArray<TYPE>&Ref)
  {
    oper_set(Ref);
  }
  void oper_set(const QapArray<TYPE>&Ref)
  {
    Free();
    if(!Ref)return;
    Init(Ref.Mem.size);
    QapAssert(Ref.Mem.size==this->Mem.size);
    memcpy_s(this->Mem.ptr,this->Mem.size*sizeof(TYPE),Ref.Mem.ptr,Ref.Mem.size*sizeof(TYPE));
  }
  QapArray(QapArray<TYPE>&&Ref)
  {
    oper_set(std::move(Ref));
  }
  void operator=(QapArray<TYPE>&&Ref)
  {
    oper_set(std::move(Ref));
  }
  void oper_set(QapArray<TYPE>&&Ref)
  {
    Free();
    this->Mem=std::move(Ref.Mem);
  }
  operator bool()const
  {
    return Mem.ptr;
  }
  TYPE&operator[](size_t index)
  {
    return (TYPE&)Mem.ptr[index*sizeof(TYPE)];
  }
  const TYPE&operator[](size_t index)const
  {
    return (const TYPE&)Mem.ptr[index*sizeof(TYPE)];
  }
  void Init(int size)
  {
    if(*this)Free();
    QapAssert(size>0);
    QapAssert(size<(1<<26));
    Mem.ptr=(TMemory::ptr_t*)(void*)new TYPE[size];
    Mem.size=size;
  }
  void Free()
  {
    if(!Mem.ptr)return;
    TYPE*p=(TYPE*)(void*)(TMemory::ptr_t*)Mem.ptr;
    delete[] p;
    Mem.ptr=nullptr;
    Mem.size=0;
  }
  TYPE*get()
  {
    return (TYPE*)(void*)(TMemory::ptr_t*)Mem.ptr;
  }
  const TYPE*get()const
  {
    return (TYPE*)(void*)(TMemory::ptr_t*)Mem.ptr;
  }
};
class QapTexMem
{
public:
public:
  typedef QapTexMem SelfClass;
public:
  QapArray<QapColor> Arr;
  int W;
  int H;
public:
  void DoReset()
  {
    {
    }
    detail::FieldTryDoReset(this->Arr);
    (this->W)=(0);
    (this->H)=(0);
  }
public:
public:
  QapTexMem()
  {
    DoReset();
  }
  QapTexMem(const QapTexMem&Ref)
  {
    oper_set(Ref);
  }
  void operator=(const QapTexMem&Ref)
  {
    oper_set(Ref);
  }
  void oper_set(const QapTexMem&Ref)
  {
    this->Arr=Ref.Arr;
    this->W=Ref.W;
    this->H=Ref.H;
  }
  QapTexMem(QapTexMem&&Ref)
  {
    oper_set(std::move(Ref));
  }
  void operator=(QapTexMem&&Ref)
  {
    oper_set(std::move(Ref));
  }
  void oper_set(QapTexMem&&Ref)
  {
    this->Arr=std::move(Ref.Arr);
    this->W=std::move(Ref.W);
    this->H=std::move(Ref.H);
  }
  QapTexMem Clone()
  {
    QapTexMem tmp;
    tmp=*this;
    return std::move(tmp);
  }
  void CopyDataFrom(const QapTexMem&Ref)
  {
    QapTexMem tmp;
    this->Arr=Ref.Arr;
    this->W=Ref.W;
    this->H=Ref.H;
  }
  void MoveDataFrom(QapTexMem&Ref)
  {
    QapTexMem tmp;
    this->Arr=std::move(Ref.Arr);
    this->W=std::move(Ref.W);
    Ref.W=0;
    this->H=std::move(Ref.H);
    Ref.H=0;
  }
  void Init(int W,int H)
  {
    this->W=W;
    this->H=H;
    Arr.Init(W*H);
  }
  void Free()
  {
    Arr.Free();
  }
  QapColor*get()
  {
    return Arr.get();
  }
  const QapColor*get()const
  {
    return Arr.get();
  }
  operator bool()const
  {
    return Arr;
  }
public:
  void InvertY()
  {
    QapColor*pBits=Arr.get();
    for (int i=0;i<H/2;i++)
    {
      int a=W*(i);
      int b=W*(H-i-1);
      for (int j=0;j<W;j++)
      {
        std::swap(pBits[a+j],pBits[b+j]);
      }
    }
  }
  void InvertX()
  {
    QapColor*pBits=Arr.get();
    for (int i=0;i<H;i++)
    {
      QapColor*pLine=&pBits[i*W];
      for (int j=0;j<W/2;j++)
      {
        int a=j;
        int b=W-j-1;
        std::swap(pLine[a],pLine[b]);
      }
    }
  }
public:
  void GenRect(int TexSize)
  {
    Init(TexSize,TexSize);
    auto*pT=get();
    for (int i=0;i<TexSize;i++)
    {
      for (int j=0;j<TexSize;j++)
      {
        *pT=0x80000000;
        pT++;
      }
    }
    RectLine(0,0,TexSize,TexSize,0xffffffff);
  }
  void GenBall(int TexSize)
  {
    Init(TexSize,TexSize);
    auto*pT=get();
    for (int i=0;i<TexSize;i++)
    {
      for (int j=0;j<TexSize;j++)
      {
        float x=i-TexSize/2;
        float y=j-TexSize/2;
        float r=(float)TexSize/2;
        float m=((x*x)+(y*y))/(r*r);
        if(m>1.0f)
        {
          *pT=0x00000000;
          pT++;
          continue;
        }
        m=std::max(0.0f,1.0f-(m*m*m));
        m=std::min(1.0f,m);
        float a=(m<0.5)?m*2:1;
        a=std::min(1.0f,a);
        a=std::max(0.0f,a);
        byte bm=m*255;
        pT->a=byte(a*255);
        pT->r=bm;
        pT->g=bm;
        pT->b=bm;
        pT++;
      }
    }
  }
  void GenQuad(int TexSize)
  {
    Init(TexSize,TexSize);
    auto*pT=get();
    for (int i=0;i<TexSize;i++)
    {
      for (int j=0;j<TexSize;j++)
      {
        float x=i-TexSize/2+0.5f,y=j-TexSize/2+0.5f;
        float r=(float)TexSize/2;
        float m=((x*x*x*x)+(y*y*y*y))/(r*r*r*r);
        if(m>1.0f)
        {
          *pT=0x00000000;
          pT++;
          continue;
        }
        m=std::max(0.0f,1.0f-(m*m*m));
        m=std::min(1.0f,m);
        float a=(m<0.5)?m*2:1;
        a=std::min(1.0f,a);
        a=std::max(0.0f,a);
        byte bm=m*255;
        pT->a=byte(a*255);
        pT->r=bm;
        pT->g=bm;
        pT->b=bm;
        pT++;
      }
    }
  }
  void FillBorder(int x,int y,const QapTexMem&Source,int n=4)
  {
    auto&Target=*this;
    if(!Source)
    {
      QapAssert(false);
      return;
    }
    int sW=Source.W;
    int sH=Source.H;
    int mX=min(W,x+sW);
    int mY=min(H,y+sH);
    auto*pS=Source.get();
    auto*pT=Target.get();
    if(1)
    {
      {
        auto src=pS[(00-0)+(00-0)*sW];
        for (int i=0;i<n;i++)for (int j=0;j<n;j++)if((y+(j+(00-n)))>=0)if((y+(j+(00-n)))<H)if((x+(i+(00-n)))<W)if((x+(i+(00-n)))>=0)pT[(x+(i+(00-n)))+(y+(j+(00-n)))*W]=(src);
      };
      {
        auto src=pS[(sW-1)+(00-0)*sW];
        for (int i=0;i<n;i++)for (int j=0;j<n;j++)if((y+(j+(00-n)))>=0)if((y+(j+(00-n)))<H)if((x+(i+(sW-0)))<W)if((x+(i+(sW-0)))>=0)pT[(x+(i+(sW-0)))+(y+(j+(00-n)))*W]=(src);
      };
      {
        auto src=pS[(sW-1)+(sH-1)*sW];
        for (int i=0;i<n;i++)for (int j=0;j<n;j++)if((y+(j+(sH-0)))>=0)if((y+(j+(sH-0)))<H)if((x+(i+(sW-0)))<W)if((x+(i+(sW-0)))>=0)pT[(x+(i+(sW-0)))+(y+(j+(sH-0)))*W]=(src);
      };
      {
        auto src=pS[(00-0)+(sH-1)*sW];
        for (int i=0;i<n;i++)for (int j=0;j<n;j++)if((y+(j+(sH-0)))>=0)if((y+(j+(sH-0)))<H)if((x+(i+(00-n)))<W)if((x+(i+(00-n)))>=0)pT[(x+(i+(00-n)))+(y+(j+(sH-0)))*W]=(src);
      };
    }
    if(1)
    {
      for (int k=1;k<=n;k++)
      {
        {
          int j=00+y;
          if(InDip(0,j-k,H-1))for (int i=max(0,x);i<mX;i++)
            {
              pT[i+(j-k)*W]=pS[(i-x)+(j-y)*sW];
            }
        }
        {
          int j=mY-1;
          if(InDip(0,j+k,H-1))for (int i=max(0,x);i<mX;i++)
            {
              pT[i+(j+k)*W]=pS[(i-x)+(j-y)*sW];
            }
        }
        {
          int i=00+x;
          if(InDip(0,i-k,W-1))for (int j=max(0,y);j<mY;j++)
            {
              pT[(i-k)+j*W]=pS[(i-x)+(j-y)*sW];
            }
        }
        {
          int i=mX-1;
          if(InDip(0,i+k,W-1))for (int j=max(0,y);j<mY;j++)
            {
              pT[(i+k)+j*W]=pS[(i-x)+(j-y)*sW];
            }
        }
      }
    }
    return;
  }
  void FillMem(int x,int y,const QapTexMem&Source)
  {
    auto&Target=*this;
    if(!Source)
    {
      QapAssert(false);
      return;
    }
    int sW=Source.W;
    int sH=Source.H;
    int mX=min(W,x+sW);
    int mY=min(H,y+sH);
    auto*pS=Source.get();
    auto*pT=Target.get();
    for (int j=max(0,y);j<mY;j++)
    {
      for (int i=max(0,x);i<mX;i++)
      {
        pT[i+j*W]=pS[(i-x)+(j-y)*sW];
      }
    }
    return;
  }
  void DrawInHalfScale(const QapTexMem&Source,int count)
  {
    if(count<0)return;
    if(!count)
    {
      CopyDataFrom(Source);
      return;
    }
    QapTexMem Src=Source;
    for (int i=0;i<count;i++)
    {
      QapTexMem Target;
      int sW=Src.W;
      int sH=Src.H;
      QapAssert(!(sW%2));
      QapAssert(!(sH%2));
      Target.Init(Src.W/2,Src.H/2);
      auto tW=Target.W;
      auto tH=Target.H;
      auto*pS=Src.get();
      auto*pT=Target.get();
      for (int y=0;y<tH;y++)for (int x=0;x<tW;x++)
        {
          auto&out=pT[x+y*tW];
          auto a=pS[(x*2+(0))+(y*2+(0))*sW];
          auto b=pS[(x*2+(1))+(y*2+(0))*sW];
          auto c=pS[(x*2+(0))+(y*2+(1))*sW];
          auto d=pS[(x*2+(1))+(y*2+(1))*sW];
          auto e=QapColor::HalfMix(a,b);
          auto f=QapColor::HalfMix(c,d);
          out=QapColor::HalfMix(e,f);
        }
      Src.MoveDataFrom(Target);
    }
    MoveDataFrom(Src);
  }
  void DrawMemWithBlend(int x,int y,const QapTexMem&Source,const QapColor&color)
  {
    auto&Target=*this;
    if(!Source)
    {
      QapAssert(false);
      return;
    }
    int sW=Source.W;
    int sH=Source.H;
    int mX=min(W,x+sW);
    int mY=min(H,y+sH);
    auto*pS=Source.get();
    auto*pT=Target.get();
    for (int j=max(0,y);j<mY;j++)
    {
      for (int i=max(0,x);i<mX;i++)
      {
        auto inp=pS[(i-x)+(j-y)*sW]*color;
        auto&out=pT[i+j*W];
        auto a=inp.a;
        auto b=255-a;
        out.a=inp.a;
        out.r=(out.r*b+inp.r*a)/255;
        out.g=(out.g*b+inp.g*a)/255;
        out.b=(out.b*b+inp.b*a)/255;
      }
    }
    return;
  }
  void Clear(const QapColor&color=0xFF000000)
  {
    auto*pBits=get();
    for (int i=0;i<W*H;i++)pBits[i]=color;
  }
  void FillLine(const int Y,const QapColor&Color)
  {
    auto*pBits=get();
    for (int i=0;i<W;i++)
    {
      QapColor&pix=pBits[Y*W+i];
      pix=Color;
    }
  }
  void FillColumn(const int X,const QapColor&Color)
  {
    auto*pBits=get();
    for (int i=0;i<H;i++)
    {
      QapColor&pix=pBits[i*W+X];
      pix=Color;
    }
  }
  void RectLine(int X,int Y,int W,int H,const QapColor&Color)
  {
    FillLineEx(Y+0-0,X+0,X+W,Color);
    FillLineEx(Y+H-1,X+0,X+W,Color);
    FillColomnEx(X+0-0,Y+0,Y+H,Color);
    FillColomnEx(X+W-1,Y+0,Y+H,Color);
  }
  void FillLineEx(const int Y,const int X0,const int X1,const QapColor&Color)
  {
    auto*pBits=get();
    int x0=Clamp<int>(X0,0,W);
    int x1=Clamp<int>(X1,0,W);
    if(x0>x1)std::swap(x0,x1);
    for (int i=x0;i<x1;i++)
    {
      QapColor&pix=pBits[Y*W+i];
      pix=Color;
    }
  }
  void FillColomnEx(const int X,const int Y0,const int Y1,const QapColor&Color)
  {
    auto*pBits=get();
    int y0=Clamp<int>(Y0,0,H);
    int y1=Clamp<int>(Y1,0,H);
    if(y0>y1)std::swap(y0,y1);
    for (int i=y0;i<y1;i++)
    {
      QapColor&pix=pBits[i*W+X];
      pix=Color;
    }
  }
public:
  void CopyAlpha(QapTexMem&Alpha)
  {
    auto*pBits=get();
    auto*pAlphaBits=get();
    if(Alpha.W!=W||Alpha.H!=H)return;
    for (int i=0;i<W*H;i++)pBits[i].a=pAlphaBits[i].a;
  }
  void CalcAlpha()
  {
    auto*pBits=get();
    for (int i=0;i<W*H;i++)pBits[i].a=pBits[i].GetLuminance();
  }
  void CalcAlpha(const QapColor&Color,int threshold=0)
  {
    auto*pBits=get();
    for (int i=0;i<W*H;i++)pBits[i].a=pBits[i]==Color?0:255;
    const QapColor&c=Color;
    int t=threshold;
    int t3=t*3;
    for (int i=0;i<W*H;i++)
    {
      QapColor&p=pBits[i];
      int fr=abs(int(p.r)-int(c.r));
      int fg=abs(int(p.g)-int(c.g));
      int fb=abs(int(p.b)-int(c.b));
      int sum=fr+fg+fb;
      p.a=sum>t3?255:t3?255*sum/t3:0;
    }
  }
  void FillChannel(const QapColor&Source,DWORD BitMask)
  {
    auto*pBits=get();
    for (int i=0;i<W*H;i++)
    {
      QapColor&C=pBits[i];
      C=(~BitMask&C)|(BitMask&Source);
    };
  }
  void BlurTexture(int PassCount)
  {
    auto*pBits=get();
    struct QapARGB
    {
      BYTE B,G,R,A;
    };
    static QapARGB VoidMem[2048*2048*4];
    memcpy_s(VoidMem,sizeof(VoidMem),pBits,W*H*sizeof(QapARGB));
    static int BBM[9]=
    {
      1,2,1,
      2,4,2,
      1,2,1
    };
    static int MartixSum;
    {
      static bool _STATIC_SYS_FLAG=true;
      if(_STATIC_SYS_FLAG)
      {
        for (int i=0;i<9;i++)MartixSum+=BBM[i];
        _STATIC_SYS_FLAG=false;
      }
      ;
    };
    int PosRange[9]=
    {
      -W-1,-W,-W+1,
      -1,0,+1,
      +W-1,+W,+W+1
    };
    for (int PassId=0;PassId<PassCount;PassId++)
    {
      QapARGB *PC=0;
      QapARGB *VM=0;
      for (int j=1;j<H-1;j++)
        for (int i=1;i<W-1;i++)
        {
          PC=(QapARGB*)pBits+j*W+i;
          VM=(QapARGB*)VoidMem+j*W+i;
          float AF[4]={0,0,0,0};
          for (int t=0;t<9;t++)
          {
            QapARGB T=*(VM+PosRange[t]);
            AF[0]+=T.R*BBM[t];
            AF[1]+=T.G*BBM[t];
            AF[2]+=T.B*BBM[t];
            AF[3]+=T.A*BBM[t];
          };
          for (int i=0;i<4;i++)AF[i]/=MartixSum*255.0;
          QapColor PCC=D3DCOLOR_COLORVALUE(AF[0],AF[1],AF[2],AF[3]);
          *PC=*((QapARGB*)&PCC);
        }
      memcpy_s(VoidMem,sizeof(VoidMem),pBits,W*H*sizeof(QapARGB));
    }
    return;
  }
};
class QapTexFile
{
public:
public:
  typedef QapTexFile SelfClass;
public:
  string FN;
  QapTexMem Mem;
public:
  void DoReset()
  {
    {
    }
    detail::FieldTryDoReset(this->FN);
    detail::FieldTryDoReset(this->Mem);
  }
public:
  QapTexFile(const QapTexFile&)
  {
    QapDebugMsg("QapTexFile"" is noncopyable");
    DoReset();
  };
  QapTexFile()
  {
    DoReset();
  };
public:
  QapTexFile(QapTexFile&&_Right)
  {
    operator=(std::move(_Right));
  }
  void operator=(QapTexFile&&_Right)
  {
    if(&_Right==this)return;
    {
    }
    this->FN=std::move(_Right.FN);
    this->Mem=std::move(_Right.Mem);
  }
public:
public:
  void Load()
  {
  }
  void Save()
  {
  }
};
class QapTex
{
public:
  class TDynamicResource:public QapD3D9Resource
  {
  public:
    typedef QapTex OwnerClass;
    void operator=(OwnerClass*pOwner)
    {
      this->pOwner=pOwner;
      this->flag=false;
      this->pDev=nullptr;
    }
  public:
    TDynamicResource():pOwner(nullptr),flag(false),pDev(nullptr)
    {
      if(pDev)pDev->Resources.Reg(this);
    };
    ~TDynamicResource()
    {
      if(pDev)pDev->Resources.UnReg(this);
    };
  public:
    QapD3DDev9*pDev;
    void Mount(OwnerClass*pOwner,QapD3DDev9*pDev)
    {
      QapAssert(pOwner);
      QapAssert(pDev);
      this->pOwner=pOwner;
      this->pDev=pDev;
      if(pDev)pDev->Resources.Reg(this);
    }
  public:
    OwnerClass*pOwner;
    bool flag;
    void OnLost()
    {
      if(pOwner)
      {
        flag=bool(pOwner->Tex);
        pOwner->Free();
      }
      else
      {
        QapDebugMsg("fail");
        flag=false;
      }
    }
    void OnReset()
    {
      if(pOwner&&flag)pOwner->ReInit();
    }
  };
public:
public:
  typedef QapTex SelfClass;
public:
  int W;
  int H;
  int Levels;
  TDynamicResource DynRes;
  IDirect3DTexture9* Tex;
  bool after_reinit;
public:
  void HardReset(){
    this->~QapTex();
    new(this)QapTex();
  }
  void DoReset()
  {
    {
    }
    (this->W)=(0);
    (this->H)=(0);
    (this->DynRes)=(this);
    (this->Tex)=(nullptr);
    //added:
    (this->after_reinit=false);
  }
public:
  QapTex(const QapTex&)
  {
    QapDebugMsg("QapTex"" is noncopyable");
    DoReset();
  };
  QapTex()
  {
    DoReset();
  };
public:
public:
  ~QapTex()
  {
    Free();
  }
public:
  bool OnReinit(){auto f=after_reinit;after_reinit=false;return f;}
  void ReInit(){after_reinit=true;Init(W,H,Levels);}
  void MountDev(QapD3DDev9&Dev)
  {
    this->DynRes.Mount(this,&Dev);
  }
  void Init(int W,int H,int Levels)
  {
    Free();
    auto*Sys=DynRes.pDev;
    Sys->pDev->CreateTexture(W,H,Levels,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&Tex,NULL);
    this->W=W;this->H=H;this->Levels=Levels;
  }
  void Free()
  {
    if(!Tex)return;
    Tex->Release();
    Tex=nullptr;
  };
  operator bool()const
  {
    return Tex;
  }
  void Bind(int lvl=0)
  {
    auto*Sys=DynRes.pDev;
    if(!Sys)return;
    Sys->pDev->SetTexture(lvl,Tex);
  };
public:
  void operator=(const QapTex&)
  {
    QapDebugMsg("fail");
  }
};
#pragma once
class GlobalEnv
{
public:
  HINSTANCE hInstance;
  HINSTANCE hPrevInstance;
  LPSTR lpCmdLine;
  int nCmdShow;
public:
  GlobalEnv(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
  ):
      hInstance(hInstance),
      hPrevInstance(hPrevInstance),
      lpCmdLine(lpCmdLine),
      nCmdShow(nCmdShow)
  {
    GlobalEnv::Get(this);
  }
public:
  static const GlobalEnv&Get(GlobalEnv*ptr=nullptr)
  {
    static GlobalEnv*pGE=nullptr;
    if(!pGE)std::swap(pGE,ptr);
    QapAssert(pGE&&!ptr);
    return *pGE;
  }
private:
  GlobalEnv(const GlobalEnv&);
  void operator=(const GlobalEnv&);
};
static bool IsKeyDown(int vKey)
{
  int i=GetAsyncKeyState(vKey);
  return i<0;
}
class TQuad
{
public:
public:
  typedef TQuad SelfClass;
public:
  int x;
  int y;
  int w;
  int h;
public:
  void DoReset()
  {
    {
    }
    (this->x)=(320);
    (this->y)=(240);
    (this->w)=(320);
    (this->h)=(240);
  }
public:
  TQuad(const TQuad&)
  {
    QapDebugMsg("TQuad"" is noncopyable");
    DoReset();
  };
  TQuad()
  {
    DoReset();
  };
public:
  TQuad(TQuad&&_Right)
  {
    operator=(std::move(_Right));
  }
  void operator=(TQuad&&_Right)
  {
    if(&_Right==this)return;
    {
    }
    this->x=std::move(_Right.x);
    this->y=std::move(_Right.y);
    this->w=std::move(_Right.w);
    this->h=std::move(_Right.h);
  }
public:
public:
  void Set(int x,int y,int w,int h)
  {
    this->x=x;
    this->y=y;
    this->w=w;
    this->h=h;
  }
  vec2i&GetPos()
  {
    return *(vec2i*)(void*)(&this->x);
  }
  vec2i&GetSize()
  {
    return *(vec2i*)(void*)(&this->w);
  }
  void SetAs(const vec2i&pos,const vec2i&size)
  {
    GetPos()=pos;
    GetSize()=size;
  }
  RECT GetWinRect()
  {
    RECT tmp={x,y,x+w,y+h};
    return tmp;
  }
  void SetWinRect(const RECT&rect)
  {
    TQuad&r=*(TQuad*)(void*)(&rect);
    Set(r.x,r.y,r.w-r.x,r.h-r.y);
  }
  void SetSize(const SIZE&size)
  {
    w=size.cx;
    h=size.cy;
  }
  static TQuad getFullScreen()
  {
    TQuad tmp;
    auto mode=GetScreenMode();
    tmp.Set(0,0,mode.W,mode.H);
    return std::move(tmp);
  }
  void setAtCenter(const TQuad&quad)
  {
    QapAssert(quad.w>=w);
    QapAssert(quad.h>=h);
    x=(quad.w-w)/2;
    y=(quad.h-h)/2;
  }
  void subpos(const TQuad&q)
  {
    x-=q.x;
    y-=q.y;
  }
  void setAtCenterScreen()
  {
    setAtCenter(getFullScreen());
  }
};
class TD3DGameBox
{
public:
  class TForm
  {
  public:
    class WndClassPair
    {
    public:
      WNDPROC Proc;
      HWND hWnd;
      int ID;
      bool Used;
    public:
      WndClassPair():Proc(nullptr),hWnd(nullptr),ID(0),Used(false) {}
      WndClassPair(WNDPROC Proc,HWND hWnd,int ID,bool Used):Proc(Proc),hWnd(hWnd),ID(ID),Used(Used) {}
    public:
      void Init(TForm*Owner)
      {
        WndProcHeap::Get().Load(*this);
        QapAssert(!Used);
        QapAssert(!hWnd);
        Used=true;
        Proc((HWND)Owner,WMU_INIT,0,0);
      }
      void Free(TForm*Owner)
      {
        QapAssert(!hWnd);
        QapAssert(Used);
        Used=false;
        Proc((HWND)Owner,WMU_FREE,0,0);
        WndProcHeap::Get().Save(*this);
      }
    };
    class WndProcHeap
    {
    public:
      vector<WndClassPair> Arr;
      void Load(WndClassPair&WinPair)
      {
        QapAssert(!Arr.empty());
        WndClassPair&WCP=Arr.back();
        WinPair=std::move(WCP);
        Arr.pop_back();
      }
      void Save(WndClassPair&WinPair)
      {
        Arr.push_back(std::move(WinPair));
      }
      static WndProcHeap&Get()
      {
        static WndProcHeap Heap;
        static bool flag=false;
        if(!flag)
        {
          vector<WndClassPair>&Arr=Heap.Arr;
          {
            WndClassPair tmp(WndProc<0>,nullptr,0,false);
            Arr.push_back(tmp);
          };
          {
            WndClassPair tmp(WndProc<1>,nullptr,1,false);
            Arr.push_back(tmp);
          };
          flag=true;
        }
        return Heap;
      }
    };
  public:
  public:
    typedef TD3DGameBox OwnerClass;
  public:
    typedef TForm SelfClass;
  public:
    TD3DGameBox* Owner;
    ATOM ClassAtom;
    string ClassName;
    WndClassPair WinPair;
  public:
    void DoReset()
    {
      {
      }
      (this->Owner)=(nullptr);
      (this->ClassAtom)=(0);
      (this->ClassName)=("");
      (this->WinPair)=(WndClassPair());
    }
  public:
    TForm(const TForm&)
    {
      QapDebugMsg("TForm"" is noncopyable");
      DoReset();
    };
    TForm()
    {
      DoReset();
    };
  public:
  public:
    ~TForm()
    {
      if(WinPair.hWnd)Free();
    }
    void operator=(TD3DGameBox*Owner)
    {
      this->Owner=Owner;
    }
  public:
    void Init()
    {
      WinPair.hWnd=nullptr;
      auto tmp=CreateWindowA(
            (LPCSTR)ClassAtom,
            "Unnamed",
            WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
            0, 0, 320, 240, NULL, NULL, GlobalEnv::Get().hInstance, NULL
          );
      WinPair.hWnd=tmp;
      QapAssert(tmp);
    }
    void Free()
    {
      QapAssert(DestroyWindow(WinPair.hWnd));
      WinPair.hWnd=nullptr;
    }
  public:
    void Reg()
    {
      WinPair.Init(this);
      ClassName="TD3DGameBox_"+IToS(WinPair.ID);
      WNDCLASSEXA wcx;
      ZeroMemory(&wcx,sizeof(wcx));
      wcx.cbSize        = sizeof(wcx);
      wcx.hInstance     = GlobalEnv::Get().hInstance;
      wcx.hIcon         = LoadIcon(0,IDI_ASTERISK);
      wcx.hIconSm       = wcx.hIcon;
      wcx.hCursor       = LoadCursor(0,IDC_ARROW);
      wcx.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
      wcx.style         = 0;
      wcx.lpfnWndProc   = WinPair.Proc;
      wcx.lpszClassName = ClassName.c_str();
      ClassAtom=RegisterClassExA(&wcx);
    }
    bool UnReg()
    {
      WinPair.Free(this);
      bool flag=UnregisterClassA((LPCSTR)ClassAtom,GlobalEnv::Get().hInstance);
      ClassAtom=0;
      ClassName.clear();
      return flag;
    }
  public:
    static const UINT WMU_INIT=WM_USER+1234;
    static const UINT WMU_FREE=WM_USER+4321;
  public:
    template<int Index>
    static LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
    {
      if(msg==WM_INPUTLANGCHANGEREQUEST)
      {
        int gg=1;
        return DefWindowProcA(hWnd,msg,wParam,lParam);
      }
      if(msg==WM_INPUTLANGCHANGE)
      {
        int gg=1;
        return DefWindowProcA(hWnd,msg,wParam,lParam);
      }
      if(msg==WM_WINDOWPOSCHANGING)
      {
        static bool flag=true;
        if(flag)return 0;
      }
      static TForm*pForm=nullptr;
      if(!pForm)
      {
        QapAssert(WMU_INIT==msg);
        pForm=(TForm*)hWnd;
        return 0;
      }
      if(WMU_FREE==msg)
      {
        QapAssert(pForm);
        QapAssert(pForm->Owner);
        QapAssert(hWnd==((HWND)pForm));
        pForm=nullptr;
        return 0;
      }
      QapAssert(pForm);
      QapAssert(pForm->Owner);
#ifdef _WIN64
  #define GWL_USERDATA GWLP_USERDATA
#endif
      long UD=GetWindowLongA(hWnd,GWL_USERDATA);
      if(!UD)
      {
        SetWindowLong(hWnd,GWL_USERDATA,(long)pForm);
      }
      else
      {
        long pF=(long)pForm;
        QapAssert(pF==UD);
      }
      if(msg==WM_WINDOWPOSCHANGING)
      {
        QapAssert("WTF?");
        return 0;
      }
      auto result=pForm->Owner->WndProc(hWnd,msg,wParam,lParam);
      return result;
    }
  };
public:
public:
  typedef TD3DGameBox SelfClass;
public:
  string Caption;
  bool NeedClose;
  bool Runned;
  bool Visible;
  bool FullScreen;
  TQuad Quad;
  TForm Form;
  QapKeyboard Keyboard;
  int zDelta;
  vec2i mpos;
public:
  void DoReset()
  {
    {
    }
    (this->Caption)=("TD3DGameBox");
    (this->NeedClose)=(false);
    (this->Runned)=(false);
    (this->Visible)=(true);
    (this->FullScreen)=(true);
    detail::FieldTryDoReset(this->Quad);
    (this->Form)=(this);
    detail::FieldTryDoReset(this->Keyboard);
    (this->zDelta)=(0);
    detail::FieldTryDoReset(this->mpos);
  }
public:
  TD3DGameBox(const TD3DGameBox&)
  {
    QapDebugMsg("TD3DGameBox"" is noncopyable");
    DoReset();
  };
  TD3DGameBox()
  {
    DoReset();
  };
public:
  TD3DGameBox(TD3DGameBox&&_Right)
  {
    operator=(std::move(_Right));
  }
  void operator=(TD3DGameBox&&_Right)
  {
    if(&_Right==this)return;
    {
    }
    this->Caption=std::move(_Right.Caption);
    this->NeedClose=std::move(_Right.NeedClose);
    this->Runned=std::move(_Right.Runned);
    this->Visible=std::move(_Right.Visible);
    this->FullScreen=std::move(_Right.FullScreen);
    this->Quad=std::move(_Right.Quad);
    this->Form=std::move(_Right.Form);
    this->Keyboard=std::move(_Right.Keyboard);
    this->zDelta=std::move(_Right.zDelta);
    this->mpos=std::move(_Right.mpos);
  }
public:
public:
  bool IsWindow()
  {
    return Form.WinPair.hWnd;
  }
  void Init()
  {
    if(IsWindow())return;
    Form.Reg();
    Form.Init();
    UpdateWnd();
    Runned=true;
  }
  void UpdateWnd()
  {
    HWND&hWnd=Form.WinPair.hWnd;
    SetWindowTextA(hWnd,Caption.c_str());
  }
  void WindowMode()
  {
    HWND&hWnd=Form.WinPair.hWnd;
    TScreenMode SM=GetScreenMode();
    DWORD Style=true?WS_OVERLAPPED:WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN;
    SetWindowLong(hWnd,GWL_STYLE,Style);
    RECT Rect=Quad.GetWinRect();
    AdjustWindowRect(&Rect,Style,false);
    SetWindowPos(hWnd,0,Quad.x,Quad.y,Quad.w,Quad.h,SWP_FRAMECHANGED|SWP_NOOWNERZORDER);
    ShowWindow(hWnd,SW_MAXIMIZE);
    ShowWindow(hWnd,Visible?SW_SHOW:SW_HIDE);
  }
  ~TD3DGameBox()
  {
    Free();
  }
  void Free()
  {
    if(!IsWindow())return;
    Runned=false;
    Form.Free();
    Form.UnReg();
    NeedClose=false;
  }
  void Update()
  {
    HWND&hWnd=Form.WinPair.hWnd;
    zDelta=0;
    Keyboard.Sync();
    if(GetForegroundWindow()==hWnd)if(GetActiveWindow()==hWnd)mpos=unsafe_GetMousePos();
    for(;;)
    {
      MSG msg;
      if(!PeekMessageA(&msg,NULL,0,0,PM_REMOVE))break;
      TranslateMessage(&msg);
      {
        DispatchMessageA(&msg);
      }
    }
  }
  void Close()
  {
    NeedClose=true;
  }
  void ScanWinPlacement()
  {
    WINDOWPLACEMENT PL;
    GetWindowPlacement(Form.WinPair.hWnd,&PL);
    Quad.SetWinRect(PL.rcNormalPosition);
    QapAssert(Quad.x>-1000);
  }
  static bool Equal(RECT&a,RECT&b)
  {
    if(a.left!=b.left)return false;
    if(a.top!=b.top)return false;
    if(a.right!=b.right)return false;
    if(a.bottom!=b.bottom)return false;
    return true;
  }
  static void KeyboardUpdate(QapKeyboard&Keyboard,const HWND hWnd,const UINT msg,const WPARAM wParam,const LPARAM lParam)
  {
    bool any_key_up=(msg==WM_KEYUP)||(msg==WM_SYSKEYUP);
    bool any_key_down=(msg==WM_KEYDOWN)||(msg==WM_SYSKEYDOWN);
    if(msg==WM_ACTIVATE){
      Keyboard=std::move(QapKeyboard());
    }
    if(any_key_up||any_key_down)
    {
      bool value=any_key_down;
      auto&KB=Keyboard;
      auto&Down=Keyboard.Down;
      auto KeyUpdate=[&KB,&Down](int key,const bool value)->void
      {
        if(Down[key]==value)return;
        if(value!=IsKeyDown(key))return;
        KB.KeyUpdate(key,value);
      };
      struct TSysKey
      {
        int Key,LKey,RKey;
      } key_array[]=
      {
        {VK_SHIFT,VK_LSHIFT,VK_RSHIFT},
        {VK_CONTROL,VK_LCONTROL,VK_RCONTROL},
        {VK_MENU,VK_LMENU,VK_RMENU}
      };
      for (int i=0;i<lenof(key_array);i++)
      {
        auto&ex=key_array[i];
        if(wParam==ex.Key)
        {
          KeyUpdate(ex.LKey,value);
          KeyUpdate(ex.RKey,value);
        }
      }
    }
    if((msg==WM_KEYUP)&&(wParam==VK_SNAPSHOT))
    {
      if(wParam==VK_SNAPSHOT)
      {
        Keyboard.KeyUpdate(VK_SNAPSHOT,true);
      };
      if(wParam==VK_SNAPSHOT)
      {
        Keyboard.KeyUpdate(VK_SNAPSHOT,false);
      };
    }
    if(any_key_up)
    {
      Keyboard.KeyUpdate(wParam,false);
    }
    if(any_key_down)
    {
      Keyboard.KeyUpdate(wParam,true);
    }
    if(msg==WM_LBUTTONUP)
    {
      Keyboard.KeyUpdate(mbLeft,false);
    }
    if(msg==WM_RBUTTONUP)
    {
      Keyboard.KeyUpdate(mbRight,false);
    }
    if(msg==WM_MBUTTONUP)
    {
      Keyboard.KeyUpdate(mbMiddle,false);
    }
    if(msg==WM_LBUTTONDOWN)
    {
      Keyboard.KeyUpdate(mbLeft,true);
    }
    if(msg==WM_RBUTTONDOWN)
    {
      Keyboard.KeyUpdate(mbRight,true);
    }
    if(msg==WM_MBUTTONDOWN)
    {
      Keyboard.KeyUpdate(mbMiddle,true);
    }
    if((msg==WM_CHAR)||(msg==WM_SYSCHAR))
    {
      Keyboard.CharUpdate(wParam);
    }
  }
  LRESULT WndProc(const HWND hWnd,const UINT msg,const WPARAM wParam,const LPARAM lParam)
  {
    KeyboardUpdate(Keyboard,hWnd,msg,wParam,lParam);
    if(msg==WM_MOUSEWHEEL)
    {
      zDelta=GET_WHEEL_DELTA_WPARAM(wParam);
    };
    if(msg==WM_CHAR)
    {
      auto kbl=GetKeyboardLayout(0);
      auto ENGLISH = 0x409;
      auto RUSSIAN = 0x419;
      char c=wParam;
      int gg=1;
    }
    if(msg==WM_CLOSE)
    {
      Close();
      if(Runned)return 0;
    }
    if(msg==WM_SHOWWINDOW)
    {
      Visible=wParam;
    }
    if(msg==WM_ERASEBKGND)
    {
      DoPaint();
    }
    return DefWindowProcA(hWnd,msg,wParam,lParam);
  }
  void DoPaint()
  {
    HWND hWnd=Form.WinPair.hWnd;
    HDC DC=GetDC(hWnd);
    RECT Rect;
    GetClientRect(Form.WinPair.hWnd,&Rect);
    FillRect(DC,&Rect,(HBRUSH)(COLOR_BTNFACE));
    ReleaseDC(hWnd,DC);
  }
  vec2i GetClientSize()
  {
    RECT Rect;
    GetClientRect(Form.WinPair.hWnd,&Rect);
    return vec2i(Rect.right-Rect.left,Rect.bottom-Rect.top);
  }
  vec2i GetMousePos()
  {
    return mpos;
  }
  vec2i unsafe_GetMousePos()
  {
    POINT P;
    GetCursorPos(&P);
    auto hWnd=Form.WinPair.hWnd;
    ScreenToClient(hWnd,&P);
    vec2i p(P.x,-P.y);
    vec2i z=GetClientSize();
    return p-vec2i(z.x/2,-z.y/2);
  }
};
//---

struct QapFontInfo{
  typedef vec2i vec2i_256[256];
  vec2i_256 Coords;
  vec2i&operator[](int index){return Coords[index];}
  const vec2i&operator[](int index)const{return Coords[index];}
};

struct QapFont{
  QapTexMem Mem;
  QapFontInfo Info;
};

struct QapBitmapInfo{
  BITMAPINFO BI;
  BITMAPINFOHEADER&BH;
  QapBitmapInfo(int W,int H):BH(BI.bmiHeader){
    ZeroMemory(&BI,sizeof(BI));
    BH.biSize=sizeof(BI.bmiHeader);
    BH.biWidth=W;BH.biHeight=H;
    BH.biPlanes=1;BH.biBitCount=32;
    BH.biSizeImage=W*H*4;
  }
};

static void CreateFontMem(HWND hWnd,QapTexMem&Out,QapFontInfo&Info,const string&Name,int Size,bool Bold,int TexSize,bool fixed_pitch)
{
  Out.Free();
  Out.Init(TexSize,TexSize);
  QapColor*pix=Out.get();
  {
    HDC DC=GetDC(hWnd);
    int W=Bold?FW_BOLD:FW_NORMAL;
    int H=-MulDiv(Size,GetDeviceCaps(DC,LOGPIXELSY),72);
    {
      HFONT FNT=CreateFontA(H,0,0,0,W,0,0,0,RUSSIAN_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,NONANTIALIASED_QUALITY,fixed_pitch?FIXED_PITCH:DEFAULT_PITCH,Name.c_str());
      {
        HDC MDC=CreateCompatibleDC(DC);
        {
          HBITMAP BMP=CreateCompatibleBitmap(DC,TexSize,TexSize);
          SelectObject(MDC,BMP);
          RECT Rect; SetRect(&Rect,0,0,TexSize,TexSize);
          FillRect(MDC,&Rect,(HBRUSH)GetStockObject(BLACK_BRUSH));
          SelectObject(MDC,FNT);
          SetBkMode(MDC,TRANSPARENT);
          SetTextColor(MDC,0xFFFFFF);
          for(int i=0;i<256;i++)TextOutA(MDC,i%16*(TexSize/16),i/16*(TexSize/16),(LPCSTR)&i,1);
          {QapBitmapInfo QBI(TexSize,TexSize);GetDIBits(MDC,BMP,0,TexSize,pix,&QBI.BI,DIB_RGB_COLORS);}
          for(int i=0;i<TexSize*TexSize;i++){pix[i].a=pix[i].r;pix[i].r=255;pix[i].g=255;pix[i].b=255;}
          for(int i=0;i<256;i++){SIZE cs;GetTextExtentPoint32A(MDC,(LPCSTR)&i,1,&cs);Info.Coords[i]=vec2i(cs.cx,cs.cy);}
          DeleteObject(BMP);
        }
        DeleteDC(MDC);
      }
      DeleteObject(FNT); 
    }
    ReleaseDC(hWnd,DC);
  }
  Out.InvertY();
}
class TD3DGameBoxBuilder;
class TD3DGameBoxBuilder
{
public:
public:
  typedef TD3DGameBoxBuilder SelfClass;
public:
  TD3DGameBox win;
  QapD3D9 D9;
  QapD3DDev9 D9Dev;
  QapDev qDev;
  int SleepMs;
public:
  void DoReset()
  {
    {
    }
    detail::FieldTryDoReset(this->win);
    detail::FieldTryDoReset(this->D9);
    detail::FieldTryDoReset(this->D9Dev);
    detail::FieldTryDoReset(this->qDev);
    (this->SleepMs)=(16);
  }
public:
  TD3DGameBoxBuilder(const TD3DGameBoxBuilder&)
  {
    QapDebugMsg("TD3DGameBoxBuilder"" is noncopyable");
    DoReset();
  };
  TD3DGameBoxBuilder()
  {
    DoReset();
  };
public:
  TD3DGameBoxBuilder(TD3DGameBoxBuilder&&_Right)
  {
    operator=(std::move(_Right));
  }
  void operator=(TD3DGameBoxBuilder&&_Right)
  {
    if(&_Right==this)return;
    {
    }
    this->win=std::move(_Right.win);
    this->D9=std::move(_Right.D9);
    this->D9Dev=std::move(_Right.D9Dev);
    this->qDev=std::move(_Right.qDev);
    this->SleepMs=std::move(_Right.SleepMs);
  }
public:
public:
  void DoNice()
  {
    auto&builder=*this;
    builder.init("2014.06.24");
    builder.win.Init();
    builder.init_d3d();
    builder.win.WindowMode();
    UpdateWindow(builder.win.Form.WinPair.hWnd);
    builder.loop();
  }
public:
  void SceneUpdate()
  {
    SceneUpdateEx();
    SceneRenderEx();
  }
public:
  void SceneUpdateEx()
  {
    SceneDoMove();
  }
  unsigned int main_clear_color=0xffc8c8c8;
  void SceneRenderEx()
  {
    auto&Dev=D9Dev;
    vec2i ClientSize=win.GetClientSize();
    if(!Dev.BeginScene())return;
    {
      D3DVIEWPORT9 ViewPort={0,0,(DWORD)ClientSize.x,(DWORD)ClientSize.y,-1.f,+1.f};
      Dev.pDev->SetViewport(&ViewPort);
      Dev.Set2D(vec2i(0,0),1.0,0,&ClientSize);
      Dev.Clear2D(main_clear_color);
      qDev.NextFrame();
      SceneDoDraw();
    }
    if(!Dev.EndScene())return;
    RECT Rect={0,0,ClientSize.x,ClientSize.y};
    Dev.Present(&Rect,&Rect);
  }
public:
  virtual void DoMove() {};
  virtual void DoDraw() {};
public:
  void SceneDoMove()
  {
    auto&Dev=D9Dev;
    if(!qDev.DynRes.pDev)
    {
      qDev.MountDev(Dev);
      qDev.Init(1024*1024,1024*1024*3);
    }
    DoMove();
  }
  void SceneDoDraw()
  {
    DoDraw();
  }
public:
  void init_d3d()
  {
    HWND hwnd=win.Form.WinPair.hWnd;
    D9.Init();
    D9Dev.PresParams.SetToDef(hwnd,true);
    D9Dev.Init(hwnd,D9);
  }
  void init(const string&caption)
  {
    win.Caption=caption;
    win.Init();
    win.Visible=false;
    win.WindowMode();
    init_d3d();
    win.Visible=true;
    win.WindowMode();
    UpdateWindow(win.Form.WinPair.hWnd);
  }
  void loop()
  {
    for (int i=0;win.Runned;i++)
    {
      win.Update();
      SceneUpdate();
      if(SleepMs>=0)Sleep(SleepMs);
      if(win.NeedClose)win.Runned=false;
    }
  }
public:
  typedef QapDev::t_geom t_geom;
public:
  struct t_simbol{
    struct t_quad{
      vec2i p;
      vec2i s;
      void set(int x,int y,int w,int h){p.x=x;p.y=y;s.x=w;s.y=h;}
    };
    vector<t_quad> arr;
  };
  struct qap_text{
    struct t_simbol{
      struct t_quad{
        vec2i p;
        vec2i s;
        void set(int x,int y,int w,int h){p.x=x;p.y=y;s.x=w;s.y=h;}
      };
      t_geom geom;
      real ps;
      vector<t_quad> arr;
      void make_geom(real ps)
      {
        this->ps=ps;
        for(int j=0;j<arr.size();j++)
        {
          auto&ex=arr[j];
          auto p=vec2d(ex.p.x,-ex.p.y)*ps+(vec2d(ex.s)-vec2d(1,1))*ps*0.5;
          auto sx=ps*ex.s.x;
          auto sy=ps*ex.s.y;
          geom.add(QapDev::GenGeomQuad(p,sx,sy));
        }
      }
    };
    struct t_mem{
      QapTexMem out;
      QapFontInfo info;
      vector<t_simbol> arr;
      vector<unsigned char> x_chars;
      //int def_text_size;
      real ms;
      t_mem():ms(0){}
      void prepare(HWND hwnd){if(arr.empty())init(hwnd);}
      void init(HWND hwnd)
      {
        QapAssert(arr.empty());
        if(!out.W)CreateFontMem(hwnd,out,info,"system",8,false,512,true);
        arr.resize(256);
        for(int i=0;i<256;i++)
        {
          unsigned char c=i;
          auto&cinfo=arr[c];
          cinfo.ps=-1;
          auto size=info.Coords[c];
          auto p=vec2i(c%16,c/16);
          int n=out.W/16;
          auto s=out.Arr.get();
          auto buffx=cinfo.arr;
          auto buffy=cinfo.arr;
          //if(use_x)
          {
            for(int y=0;y<n;y++)for(int x=0;x<size.x;x++)
            {
              auto addr=p.x*n+x+(p.y*n+y)*out.W;
              auto&ex=s[addr];
              if(!ex.a)continue;
              int sx=1;
              for(int cx=x+1;cx<size.x;cx++){
                if(!s[addr-x+cx].a)break;
                sx++;
              }
              qap_add_back(buffx).set(x,y,sx,1);
              if(sx>1)x+=sx-1;
            }
          }
          //if(!use_x)
          {
            for(int x=0;x<size.x;x++)for(int y=0;y<n;y++)
            {
              auto addr=p.x*n+x+(p.y*n+y)*out.W;
              auto&ex=s[addr];
              if(!ex.a)continue;
              int sy=1;
              for(int cy=y+1;cy<n;cy++){
                auto naddr=p.x*n+x+(p.y*n+cy)*out.W;
                if(!s[naddr].a)break;
                sy++;
              }
              qap_add_back(buffy).set(x,y-1,1,-sy);
              if(sy>1){
                y+=sy-1;
              }
            }
          }
          bool best_x=buffx.size()<buffy.size();
          if(best_x){
            qap_add_back(x_chars)=c;
          }
          cinfo.arr=best_x?buffx:buffy;
        }
      }
    };
    static t_mem&get_mem(HWND hwnd){static t_mem mem;mem.prepare(hwnd);return mem;}
    static vec2i get_size(QapDev&qDev,const string&text,int cell_size=32)
    {
      auto&mem=get_mem(qDev.DynRes.pDev->PresParams.pp.hDeviceWindow);
      QapFontInfo&info=mem.info;
      vector<t_simbol>&arr=mem.arr;
      auto ps=cell_size*0.125*0.5;
      auto offset=vec2i_zero;
      for(int i=0;i<text.size();i++)
      {
        unsigned char c=text[i];
        auto size=info.Coords[c];
        offset.x+=size.x*ps;
      }
      return vec2i(offset.x,info.Coords[' '].y);
    }
    static void draw(QapDev&qDev,int px,int py,const string&text,int cell_size=32){draw(qDev,vec2i(px,py),text,cell_size);}
    static void draw(QapDev&qDev,const vec2d&pos,const string&text,int cell_size=32){draw(qDev,vec2i(pos.x,pos.y),text,cell_size);}
    static void draw(QapDev&qDev,const vec2i&pos,const string&text,int cell_size=32)
    {
      auto&mem=get_mem(qDev.DynRes.pDev->PresParams.pp.hDeviceWindow);
      QapFontInfo&info=mem.info;
      vector<t_simbol>&arr=mem.arr;
      auto ps=cell_size*0.125*0.5;
      auto offset=pos;
      for(int i=0;i<text.size();i++)
      {
        unsigned char c=text[i];
        auto size=info.Coords[c];
        auto&ex=arr[c];
        if(ex.ps<0)ex.make_geom(ps);
        if(ex.ps==ps)
        {
          qDev.DrawGeomWithOffset(ex.geom,offset);
        }
        if(ex.ps!=ps)
        {
          auto&quads=ex.arr;
          for(int j=0;j<quads.size();j++)
          {
            auto&ex=quads[j];
            auto p=vec2d(ex.p.x,-ex.p.y)*ps+(vec2d(ex.s)-vec2d(1,1))*ps*0.5;
            auto sx=ps*ex.s.x;
            auto sy=ps*ex.s.y;
            qDev.DrawQuad(offset+p,sx,sy);
          }
        }
        offset.x+=size.x*ps;
      }
    }
  };
public:
  vec2d get_dir_from_keyboard_wasd_and_arrows()
  {
    auto&kb=win.Keyboard;
    vec2d dp=vec2d_zero;
    auto dir_x=vec2d(1,0);
    auto dir_y=vec2d(0,1);
    #define F(dir,key_a,key_b)if(kb.Down[key_a]||kb.Down[key_b]){dp+=dir;}
    F(-dir_x,VK_LEFT,'A');
    F(+dir_x,VK_RIGHT,'D');
    F(+dir_y,VK_UP,'W');
    F(-dir_y,VK_DOWN,'S');
    #undef F
    return dp;
  }
};
template<class TYPE>
static vector<int> qap_find_val(const vector<TYPE>&arr,const TYPE&val){
  vector<int> out;
  for(int i=0;i<arr.size();i++){if(val==arr[i])out.push_back(i);}
  return out;
}
static bool qap_add_unique_str(vector<string>&arr,const string&value){
  auto a=qap_find_str(arr,value);
  QapAssert(a.size()<=1);
  if(!a.empty())return false;
  arr.push_back(value);
  return true;
}
template<class TYPE>static void qap_drop_by_id(vector<TYPE>&arr,int id){
  auto*ptr=qap_check_id(arr,id)?&arr[id]:nullptr;QapCleanIf(arr,[ptr](TYPE&ex){return &ex==ptr;});
}
template<class VECTOR_TYPE>
void qap_clean_if_deaded(VECTOR_TYPE&arr)
{
  size_t last=0;auto arr_size=arr.size();
  for(size_t i=0;i<arr_size;i++)
  {
    auto&ex=arr[i];
    if(ex.deaded)continue;
    if(last!=i)
    {
      auto&ax=arr[last];
      ax=std::move(ex);
    }
    last++;
  }
  if(last==arr.size())return;
  arr.resize(last);
}
template<class TYPE>static bool qap_check_id(const vector<TYPE>&arr,int id){return id>=0&&id<arr.size();}
template<class TYPE>static bool qap_check_id(const vector<TYPE>&arr,float id){QapNoWay();return false;}
template<class TYPE>static bool qap_check_id(const vector<TYPE>&arr,real id){QapNoWay();return false;}
template<class TYPE>static bool qap_check_id(const vector<TYPE>&arr,long long id){return id<arr.size();}
template<class TYPE>static bool qap_check_id(const vector<TYPE>&arr,unsigned long long id){return id<arr.size();}
template<class TYPE>static bool qap_check_id(const vector<TYPE>&arr,unsigned id){return id<arr.size();}
//-------------------------------------------//
//-------------------------------------------//
#undef SAY
//-------------------------------------------//
/*static string conv_tm_to_str(const std::tm&t)
{
  auto*ptm=&t;
  char buff[128];
  sprintf(&buff[0],"%04u.%02u.%02u %02u:%02u:%02u\0",
    1900+ptm->tm_year,
    ptm->tm_mon+1,
    ptm->tm_mday,
    ptm->tm_hour,ptm->tm_min,ptm->tm_sec
  );
  return buff;
}
static string cur_date_str()
{
  time_t rawtime;
  time(&rawtime);
  auto tm=*gmtime(&rawtime);
  return conv_tm_to_str(tm);
}
std::tm conv_systime_to_tm(const SYSTEMTIME&st)
{
	std::tm tm;
	tm.tm_sec=st.wSecond;
	tm.tm_min=st.wMinute;
	tm.tm_hour=st.wHour;
	tm.tm_mday=st.wDay;
	tm.tm_mon=st.wMonth-1;
	tm.tm_year=st.wYear-1900;
	tm.tm_isdst=-1;
	return tm;
}
SYSTEMTIME conv_filetime_to_systime(const FILETIME&ft)
{
  SYSTEMTIME st;
  QapAssert(FileTimeToSystemTime(&ft,&st));
  return st;
}
string conv_filetime_to_str(const FILETIME&ft)
{
  return conv_tm_to_str(conv_systime_to_tm(conv_filetime_to_systime(ft)));
}
string get_file_last_write_time(const string&fn)
{
  auto hFile=CreateFile(fn.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  if(hFile==INVALID_HANDLE_VALUE){QapNoWay();return "XXXX.XX.XX XX:XX;XX";}
  FILETIME CreationTime;
  FILETIME LastAccessTime;
  FILETIME LastWriteTime;
  bool ok=GetFileTime(hFile,&CreationTime,&LastAccessTime,&LastWriteTime);
  if(!ok){QapNoWay();return "XXXX.XX.XX XX:XX;XX";}
  CloseHandle(hFile);
  return conv_filetime_to_str(LastWriteTime);
}*/
void DrawLine(QapDev&qDev,const vec2d&a,const vec2d&b,real line_size)
{
  qDev.DrawQuad((b+a)*0.5,(b-a).Mag(),line_size,(b-a).GetAng());
}
int sqr(int x){return x*x;}
double sqr(double x){return x*x;}
class TQapGame:public TD3DGameBoxBuilder{public:QapKeyboard&kb;vec2i&mpos;TQapGame():kb(win.Keyboard),mpos(win.mpos){DoReset();}};

struct t_offcentric_scope{
  QapDev&qDev;
  const vec2d&unit_pos;
  const vec2d&unit_dir;
  const real scale;
  t_offcentric_scope(QapDev&qDev,const vec2d&unit_pos,const vec2d&unit_dir,real scale,bool unit_offcentric):
    qDev(qDev),unit_pos(unit_pos),unit_dir(unit_dir),scale(scale)
  {
    qDev.xf=make_xf(qDev.viewport,unit_pos,unit_dir,scale,unit_offcentric);
    QapAssert(!qDev.use_xf);
    qDev.use_xf=true;
  }
  static real get_koef(){return 0.25;}
  static transform2f make_xf(const t_quad&viewport,const vec2d&unit_pos,const vec2d&unit_dir,real scale,bool unit_offcentric)
  {
    static const real offcentric_koef=get_koef();
    vec2d offcentric=vec2d(0,viewport.size.y*offcentric_koef);
    auto base_offset=unit_pos;//+qDev.viewport.pos;
    transform2f xf;
    xf.r.set(unit_dir.GetAng());
    xf.r.mul(scale);
    xf.p.set_zero();
    xf.p=(xf*-base_offset)+vec2f(-(unit_offcentric?offcentric:vec2d_zero));
    return xf;
  }
  static vec2d screen_to_world(const t_quad&viewport,const vec2d&s2wpos,const vec2d&cam_pos,const vec2d&cam_dir,real scale,bool offcentric)
  {
    auto off_offset=offcentric?vec2d(0,viewport.size.y*t_offcentric_scope::get_koef()):vec2d(0,0);
    return cam_pos+(s2wpos+off_offset).Rot(cam_dir)*(1.0/scale);
  };
 ~t_offcentric_scope()
  {
    qDev.use_xf=false;
    qDev.xf.set_ident();
  }
};

template<class MONSTER,class UNIT>
static const UNIT*get_near_unit(const MONSTER&a,const vector<UNIT>&arr)
{
  auto p=a.pos;int id=-1;real d=-1;
  for(int i=0;i<arr.size();i++){
    auto&ex=arr[i];
    if(ex.deaded)continue;
    auto m=(ex.pos-p).SqrMag();
    if(id>=0&&m>=d)continue;
    d=m;id=i;
  }
  return id<0?nullptr:&arr[id];
}
inline real SToF(const string&S){real r;sscanf_s(S.c_str(),"%lf",&r);return r;};

template<class TYPE>void reverse(vector<TYPE>&loop){auto c=loop.size();for(int i=0;i<c/2;i++)std::swap(loop[c-1-i],loop[i]);}

static bool rewind(vector<vec2d>&arr,int point_id){
  if(!qap_check_id(arr,point_id))return false;
  vector<vec2d> out;for(int i=0;i<arr.size();i++)qap_add_back(out)=arr[(i+point_id)%arr.size()];arr=out;
  return true;
}

template<class TYPE>
void operator+=(vector<TYPE>&arr,vector<TYPE>&&ref)
{
  //arr.reserve(arr.size()+ref.size());
  for(int i=0;i<ref.size();i++)
  {
    arr.push_back(std::move(ref[i]));
  }
  ref.clear();
}
template<class TYPE>void qap_sort(vector<TYPE>&arr){std::sort(arr.begin(),arr.end());}
template<class TYPE>void qap_subarr(vector<TYPE>&arr,int offset){
  if(offset<0)offset+=arr.size();
  if(!offset||!qap_check_id(arr,offset))return;
  int p=0;
  for(int i=offset;i<arr.size();i++)arr[p++]=std::move(arr[i]);
  arr.resize(arr.size()-offset);
}

template<size_t BLOCK_LENGTH=1024*1024>
class FsIO:public QapIO{
public:
  fstream f;
  string block;
  size_t bpos;
  size_t pos;
  size_t n;
  static size_t FileLength(iostream&f)
  {
    using namespace std;
    f.seekg(0,ios::end);
    auto L=f.tellg();
    f.seekg(0,ios::beg);
    return L;
  };
  FsIO(const string&FN){
    using namespace std;
    f.open(FN.c_str(),ios::in|ios::binary);
    if(!f){n=0;pos=0;bpos=0;return;}
    n=FileLength(f);
    pos=0;bpos=0;
    read_block();
  }
  void read_block(){
    auto v=std::min(BLOCK_LENGTH,n-bpos);
    block.resize(v);
    f.read(&block[0],block.size());
    bpos+=block.size();
  }
public:
  virtual void SavePOD(void*p,int count){QapNoWay();}
  virtual void LoadPOD(void*ptr,int c)
  {
    auto*p=(char*)ptr;auto count=c;
    QapAssert(count>=1);
    QapAssert(pos+count<=n);
    for(;;)
    {
      if(pos+count<=bpos){
        auto addr=pos-(bpos-block.size());
        QapAssert(addr>=0&&addr<block.size());
        memcpy(p,&block[addr],count);
        pos+=count;
        return;
      }else{
        auto addr=pos-(bpos-block.size());
        auto bef_n=bpos-pos;
        QapAssert(bef_n<=block.size()-addr);
        if(block.size()!=addr&&bef_n!=0)
        {
          QapAssert(addr>=0&&addr<block.size());
          memcpy(p,&block[addr],bef_n);
        }
        read_block();
        p+=bef_n;
        count-=bef_n;
        pos+=bef_n;
      }
    }
    int gg=1;
  }
  virtual bool TryLoad(int count){return pos+count<=n;}
  virtual void Crash(){QapNoWay();}
  virtual bool IsCrashed(){return n<=0;}
  virtual bool IsSizeIO(){return false;}
  virtual int GetSize(){return n;}
  virtual void WriteTo(QapIO&ref){QapNoWay();}
};

class FsWrite:public QapIO{
public:
  fstream f;
  int pos;
  int n;
  FsWrite(const string&FN){
    using namespace std;
    f.open(FN.c_str(),ios::out|ios::binary);
    if(!f)QapNoWay();
    pos=0;//bpos=0;
  }
public:
  virtual void SavePOD(void*p,int count){
    f.write((char*)p,count);
    pos+=count;
  }
  virtual void LoadPOD(void*ptr,int c){QapNoWay();}
  virtual bool TryLoad(int count){QapNoWay();return false;}
  virtual void Crash(){QapNoWay();}
  virtual bool IsCrashed(){return false;}
  virtual bool IsSizeIO(){return false;}
  virtual int GetSize(){QapNoWay();return n;}
  virtual void WriteTo(QapIO&ref){QapNoWay();}
};

static string get_first_part(const string&query,char needle){auto p=query.find(needle);if(p==std::string::npos)return query;return query.substr(0,p);}
static string get_second_part(const string&query,char needle){auto p=query.find(needle);if(p==std::string::npos)return "";return query.substr(p+1);}
static string get_first_part(const string&query,const string&needle){auto p=query.find(needle);if(p==std::string::npos)return query;return query.substr(0,p);}
static string get_second_part(const string&query,const string&needle){auto p=query.find(needle);if(p==std::string::npos)return "";return query.substr(p+needle.size());}

template<class TYPE,class FUNC>
int qap_minval_id_for_vec(vector<TYPE>&arr,FUNC func){
  if(arr.empty())return -1;                         
  int id=0;auto val=func(arr[0],0); 
  for(int i=1;i<arr.size();i++){                    
    auto&ex=arr[i];                                 
    auto tmp=func(ex,i);                  
    if(tmp>=val)continue;               
    val=tmp;id=i;                 
  }                                                 
  return id;
}
#define QAP_MINVAL_ID_OF_VEC(arr,code)qap_minval_id_for_vec(arr,[&](decltype(arr[0])&ex,int i){return code;});

vec2d get_max_quad_vertex(const vec2d&quad_vertex,const vec2d&quad_ox){
  auto qv=quad_vertex;auto out=qv;
  vec2d::comax(out,+qv.UnRot(quad_ox));
  vec2d::comax(out,-qv.UnRot(quad_ox));
  qv.x*=-1;
  vec2d::comax(out,+qv.UnRot(quad_ox));
  vec2d::comax(out,-qv.UnRot(quad_ox));
  return out;
}

template<typename TYPE,size_t COUNT>inline size_t lenof(std::array<TYPE,COUNT>(&)){return COUNT;}

template<class TYPE>
static bool qap_add_unique_value(vector<TYPE>&arr,const TYPE&value){
  auto a=qap_find_val(arr,value);
  QapAssert(a.size()<=1);
  if(!a.empty())return false;
  arr.push_back(value);
  return true;
}
//-------------------------------------------//
//   {<|          21.10.2016           |>}   //
//-------------------------------------------//