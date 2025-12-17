#define DEF_PRO_FULL
#include "StdAfx.h"
#include "QapLiteCutted.h"
#include "TQapGameV2.inl"
double Dist2Line(const vec2d&point,const vec2d&a,const vec2d&b){
  auto p=(point-a).Rot(b-a);
  if(p.x<0||p.x>(b-a).Mag())return 1e9;
  return fabs(p.y);
}

bool QapPublicUberFullLoaderBinLastHopeMem(IEnvRTTI&Env,IQapRawObject&&Object,const string&Inp);
bool QapPublicUberFullLoaderBinLastHope(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
bool QapPublicUberFullSaverProto(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
bool QapPublicUberFullSaverBin(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
bool QapPublicUberFullCloneBinMem(IEnvRTTI&Env,IQapRawObject&&Dest,IQapRawObject&&Source);

class t_material{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_material)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(QapColor,color,DEF,$,$)\
ADDVAR(vec2d,pos,DEF,$,$)\
ADDEND()
//=====+>>>>>t_material
#include "QapGenStruct.inl"
//<<<<<+=====t_material
public:
};
class t_endpoint{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_endpoint)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(vec2d,pos,DEF,$,$)\
ADDVAR(double,r,SET,8,$)\
ADDVAR(TWeakPtr<t_material>,mat,DEF,$,$)\
ADDEND()
//=====+>>>>>t_endpoint
#include "QapGenStruct.inl"
//<<<<<+=====t_endpoint
public:
};
class t_line{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_line)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(t_endpoint,a,DEF,$,$)\
ADDVAR(t_endpoint,b,DEF,$,$)\
ADDVAR(bool,enabled,SET,true,$)\
ADDVAR(double,line_size,SET,4.0,$)\
ADDVAR(double,weight,SET,1.0,$)\
ADDVAR(string,label,DEF,$,$)\
ADDVAR(TWeakPtr<t_material>,mat,DEF,$,$)\
ADDEND()
//=====+>>>>>t_line
#include "QapGenStruct.inl"
//<<<<<+=====t_line
public:
};
class t_layer{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_layer)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(string,name,DEF,$,$)\
ADDVAR(vector<t_line>,larr,DEF,$,$)\
ADDVAR(vector<t_material>,marr,DEF,$,$)\
ADDEND()
//=====+>>>>>t_layer
#include "QapGenStruct.inl"
//<<<<<+=====t_layer
public:
};
class t_world{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_world)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(int,tick,SET,0,$)\
ADDVAR(vector<t_layer>,layers,DEF,$,$)\
ADDVAR(int,layer_id,SET,-1,$)\
ADDVAR(t_layer,cur,DEF,$,$)\
ADDVAR(bool,show_mat,SET,false,$)\
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
  t_line nl;
  double mat_size=32;public:
  TWeakPtr<t_material> drag_mat;
  TWeakPtr<t_line> hover_line;
  struct t_sel_endpoint{bool a=false;int id=-1;};
  t_sel_endpoint sep;
public:
  void UpdateMatPos(){
    int i=0;
    for(auto&ex:w.cur.marr){
      ex.pos=vec2d((i-int(w.cur.marr.size())/2)*mat_size,-220);
      i++;
    }
  }
  void DoInit(){
    qap_add_back(w.cur.marr).color=0xffff0000;
    qap_add_back(w.cur.marr).color=0xff00ff00;
    qap_add_back(w.cur.marr).color=0xff0000ff;
    UpdateMatPos();
  }
  void DoMigrate(){
    if(!QapPublicUberFullLoaderBinLastHope(*pEnv,QapRawUberObject(w),fn)){
      DoInit();
      int gg=1;
    }
  }
  void DoUpdate(){
    if(kb.OnDown(VK_F1)){
      qap_add_back(w.cur.marr).color=0xffffff00;
      qap_add_back(w.cur.marr).color=0xffff8000;
      qap_add_back(w.cur.marr).color=0xff0080ff;
      UpdateMatPos();
    }
    if(kb.OnDown(VK_F2)){
      auto&layer=qap_add_back(w.layers);
      layer=std::move(w.cur);
    }
    if(kb.OnDown(VK_F5)&&qap_check_id(w.layers,w.layer_id)){
      auto&layer=w.layers[w.layer_id];
      auto&tmp=w.cur;
      tmp={};
      QapPublicUberFullCloneBinMem(*pEnv,QapRawUberObject(tmp),QapRawUberObject(layer));
    }
    if(kb.OnDown(VK_F4)&&qap_check_id(w.layers,w.layer_id)){
      auto&layer=w.layers[w.layer_id];
      auto&tmp=w.cur;
      layer={};
      QapPublicUberFullCloneBinMem(*pEnv,QapRawUberObject(layer),QapRawUberObject(tmp));
    }
    if(kb.OnDown(VK_UP))w.layer_id++;
    if(kb.OnDown(VK_DOWN))w.layer_id--;
    if(kb.OnDown(mbLeft)){
      nl.a.pos=mpos;
    }
    if(kb.Down[mbLeft]){
      nl.b.pos=mpos;
    }
    if(kb.OnUp(mbLeft)){
      qap_add_back(w.cur.larr)=std::move((nl));
    }
    if(kb.OnDown(VK_ADD)){
      for(auto&ex:w.cur.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        ex.weight=Clamp(ex.weight+0.1,0.0,1.0);
      }
    }
    if(kb.OnDown(VK_SUBTRACT)){
      for(auto&ex:w.cur.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        ex.weight=Clamp(ex.weight-0.1,0.0,1.0);
      }
    }
    if(kb.OnDown(VK_NEXT)){
      for(auto&ex:w.cur.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        ex.line_size=Clamp(ex.line_size+100,1.0,16.0);
      }
    }
    if(kb.OnDown(VK_PRIOR)){
      for(auto&ex:w.cur.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        ex.line_size=Clamp(ex.line_size-0.1,1.0,16.0);
      }
    }
    if(kb.News){
      for(auto&ex:w.cur.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4){
        string nc=" ";nc[0]=kb.LastChar;
        ex.label=nc;
      }
    }
    if(kb.OnDown(VK_DELETE)){
      for(auto&ex:w.cur.larr)if(Dist2Line(mpos,ex.a.pos,ex.b.pos)<4)ex.enabled=false;
    }
    if(kb.OnDown(VK_RCONTROL)){
      if(auto*m=FindMaterialAt(mpos)){
        drag_mat=m;
      }
      sep=FindSEP(mpos);
    }
    if(kb.Down[VK_RCONTROL]&&drag_mat){
      sep=FindSEP(mpos);
    }
    if(kb.OnUp(VK_RCONTROL)&&drag_mat&&qap_check_id(w.cur.larr,sep.id)){
      auto&l=w.cur.larr[sep.id];
      auto&ep=sep.a?l.a:l.b;
      ep.mat=drag_mat.get();
    }
    if(kb.OnDown(mbRight)){
      if(auto*m=FindMaterialAt(mpos)){
        drag_mat=m;
      }
      sep=FindSEP(mpos);
    }
    if(kb.OnDown(mbMiddle)){
      sep=FindSEP(mpos);
    }
    if(kb.Down[mbMiddle]&&qap_check_id(w.cur.larr,sep.id)){
      auto&l=w.cur.larr[sep.id];
      auto&ep=sep.a?l.a:l.b;
      ep.r=(ep.pos-mpos).Mag();
    }
    if(kb.Down[mbRight]&&!drag_mat&&qap_check_id(w.cur.larr,sep.id)){
      auto&l=w.cur.larr[sep.id];
      auto&ep=sep.a?l.a:l.b;
      ep.pos=mpos;
    }
    if(kb.Down[mbRight]&&drag_mat){
      hover_line=FindLineAt(mpos);
    }
    if(kb.OnUp(mbRight)){
      if(drag_mat&&hover_line){
        hover_line->mat=drag_mat.get();
      }
      drag_mat=nullptr;
      hover_line=nullptr;
    }
    w.show_mat=false;
    if(kb.Down(VK_MENU)){
      w.show_mat=true;
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
    if(!ref.enabled)return;
    qDev.color=ref.a.mat?ref.a.mat->color:0xff000000;
    qDev.DrawCircleEx(ref.a.pos,0,ref.a.r,24,0);
    qDev.color=ref.b.mat?ref.b.mat->color:0xff000000;
    qDev.DrawCircleEx(ref.b.pos,0,ref.b.r,24,0);
    QapColor mat=ref.mat?ref.mat->color:0xffffffff;
    qDev.color=QapColor::Mix(0xff000000,mat,ref.weight);
    qDev.DrawLine(ref.a.pos,ref.b.pos,ref.line_size);
    auto p=LineCenter(ref);
    if(ref.mat&&w.show_mat){
      qDev.color=ref.mat->color;
      qDev.DrawLine(ref.mat->pos,p,4);
    }
  }
  void DrawText(const t_line&ref){
    if(!ref.enabled)return;
    auto p=LineCenter(ref);
    qDev.color=0xff000000;
    qap_text::draw(qDev,p+vec2d(+1,-1),ref.label);
    qDev.color=0xffffffff;
    qap_text::draw(qDev,p,ref.label);
  }
  void Draw(const t_material&ref){
    qDev.color=ref.color;
    qDev.DrawQuad(ref.pos,vec2d(1,1)*mat_size);
  }
  void Draw(const t_layer&ref){
    Draw(ref.larr);
    Draw(ref.marr);
    for(auto&ex:ref.larr)DrawText(ex);
  }
  template<class TYPE>
  void Draw(const vector<TYPE>&arr){for(auto&ex:arr)Draw(ex);}
  void DoDraw()override{
    QapDev::BatchScope Scope(qDev);
    qDev.color=0xff101010;
    Draw(w.cur);
    if(kb.Down[mbLeft])Draw(nl);
    if(drag_mat){
      vec2d a=drag_mat->pos;
      vec2d b=mpos;
      if(hover_line)b=LineCenter(*hover_line.get());
      qDev.color=0x80ffffff;
      qDev.DrawLine(a,b,4);
    }
    string s="layer_id = "+IToS(w.layer_id);
    auto p=vec2d(-512,+512);
    qDev.color=0xff000000;
    qap_text::draw(qDev,p+vec2d(+1,-1),s);
    qDev.color=0xffffffff;
    qap_text::draw(qDev,p,s);
  }
  t_material*FindMaterialAt(const vec2d&p){
    for(auto&ex:w.cur.marr)
      if((ex.pos-p).SqrMag()<mat_size*mat_size*0.25)return &ex;
    return nullptr;
  }
  t_line*FindLineAt(const vec2d&p){
    for(auto&ex:w.cur.larr)
      if(ex.enabled&&Dist2Line(p,ex.a.pos,ex.b.pos)<4)return &ex;
    return nullptr;
  }
  t_sel_endpoint FindSEP(const vec2d&p){
    int i=0;
    for(auto&ex:w.cur.larr){
      if(ex.enabled){
        if(ex.a.pos.dist_to_point_less_that_r(p,max(4.0,ex.a.r)))return {true,i};
        if(ex.b.pos.dist_to_point_less_that_r(p,max(4.0,ex.b.r)))return {false,i};
      }
      i++;
    }
    return {};
  }
  vec2d LineCenter(const t_line&ex){
    return 0.5*(ex.a.pos+ex.b.pos);
  }
};
void main_2025_12_17(IEnvRTTI&Env){
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
      main_2025_12_17(Env);
      int gg=1;
    }
  };
  ThisCompileUnit ThisCompileUnitInstance;
}