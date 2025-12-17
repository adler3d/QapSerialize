#define DEF_PRO_FULL
#include "StdAfx.h"
#include "QapLiteCutted.h"
#include "TQapGameV2.inl"
double Dist2Line(const vec2d&point,const vec2d&a,const vec2d&b){
  auto p=(point-a).Rot(b-a);
  if(p.x<0||p.x>(b-a).Mag())return 1e9;
  return fabs(p.y);
}

bool QapPublicUberFullLoaderBinLastHope(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
bool QapPublicUberFullSaverProto(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
bool QapPublicUberFullSaverBin(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);

class t_endpoint{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_endpoint)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(vec2d,pos,DEF,$,$)\
ADDEND()
//=====+>>>>>t_endpoint
#include "QapGenStruct.inl"
//<<<<<+=====t_endpoint
public:
};
class t_material{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_material)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(QapColor,color,DEF,$,$)\
ADDEND()
//=====+>>>>>t_material
#include "QapGenStruct.inl"
//<<<<<+=====t_material
public:
};
class t_line{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_line)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(t_endpoint,a,DEF,$,$)\
ADDVAR(t_endpoint,b,DEF,$,$)\
ADDVAR(bool,enabled,SET,false,$)\
ADDVAR(double,weight,SET,1.0,$)\
ADDVAR(string,label,DEF,$,$)\
ADDVAR(TWeakPtr<t_material>,mat,DEF,$,$)\
ADDEND()
//=====+>>>>>t_line
#include "QapGenStruct.inl"
//<<<<<+=====t_line
public:
};

class t_world{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_world)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(int,tick,SET,0,$)\
ADDVAR(vector<t_line>,larr,DEF,$,$)\
ADDVAR(vector<t_material>,marr,DEF,$,$)\
ADDEND()
//=====+>>>>>t_world
#include "QapGenStruct.inl"
//<<<<<+=====t_world
public:
};

class TGame:public TQapGameV2{
public:
  IEnvRTTI*pEnv=nullptr;
  string fn="save20251217.qap";
  t_world w;
  t_line nl;bool nle=false;
public:
  void DoInit(){

  }
  void DoMigrate(){
    if(!QapPublicUberFullLoaderBinLastHope(*pEnv,QapRawUberObject(w),fn)){
      DoInit();
      int gg=1;
    }
  }
  void DoUpdate(){
    if(kb.OnDown(mbLeft)){
      nl.a.pos=mpos;
    }
    if(kb.Down[mbLeft]){
      nl.b.pos=mpos;
    }
    if(kb.OnUp(mbLeft)){
      qap_add_back(w.larr)=std::move((nl));
    }
    if(kb.OnDown(VK_ADD)){
      for(auto&ex:w.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        ex.weight=Clamp(ex.weight+0.1,0.0,1.0);
      }
    }
    if(kb.OnDown(VK_SUBTRACT)){
      for(auto&ex:w.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        ex.weight=Clamp(ex.weight-0.1,0.0,1.0);
      }
    }
    if(kb.News){
      for(auto&ex:w.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        string nc=" ";nc[0]=kb.LastChar;
        ex.label=nc;
      }
    }
  }
  void DoMove()override{
    if(kb.OnDown(VK_ESCAPE)){
      QapPublicUberFullSaverProto(*pEnv,QapRawUberObject(w),fn+".proto");
      QapPublicUberFullSaverBin(*pEnv,QapRawUberObject(w),fn);
      TerminateProcess(GetCurrentProcess(),0);
    }
    if(w.tick==0){
      DoMigrate();
    }
    DoUpdate();
    w.tick++;
  }
  typedef QapColor uint32;
  void Draw(const t_line&ref){
    qDev.color=0xff000000;
    qDev.DrawCircleEx(ref.a.pos,0,8,24,0);
    qDev.DrawCircleEx(ref.b.pos,0,8,24,0);
    QapColor mat=ref.mat?ref.mat->color:0xffffffff;
    qDev.color=QapColor::Mix(0xff000000,mat,ref.weight);
    qDev.DrawLine(ref.a.pos,ref.b.pos,2);
    auto p=0.5*(ref.a.pos+ref.b.pos);
    qDev.color=0xff000000;
    qap_text::draw(qDev,p,ref.label);
  }
  template<class TYPE>
  void Draw(const vector<TYPE>&arr){for(auto&ex:arr)Draw(ex);}
  void DoDraw()override{
    QapDev::BatchScope Scope(qDev);
    qDev.color=0xff101010;
    Draw(w.larr);
    if(kb.Down[mbLeft])Draw(nl);
  }
};
void main_2025_12_16(IEnvRTTI&Env){
  TGame builder;
  builder.SleepMs=0;
  builder.pEnv=&Env;
  builder.DoNice();
}

void win_main_init(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
  static GlobalEnv gEnv(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}
namespace
{
  struct ThisCompileUnit:public ICompileUnit
  {
    const char*get_filename()const override{return __FILE__;}
    void RegAll(IEnvRTTI&Env)override
    {
      #define F(CLASS){Sys$$<CLASS>::GetRTTI(Env);};;
      #define LIST(F)\
        F(TType)
      //===+>>>LIST
      LIST(F);
      //<<<+===LIST
      #undef LIST
      #undef F
    }
    void Run(IEnvRTTI&Env)override
    {
      main_2025_12_16(Env);
      int gg=1;
    }
  };
  ThisCompileUnit ThisCompileUnitInstance;
}