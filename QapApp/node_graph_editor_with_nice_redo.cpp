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

class t_port{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_port)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(vec2d,offset,DEF,$,$)\
ADDVAR(double,r,SET,8,$)\
ADDVAR(string,type,DEF,$,$)\
ADDEND()
//=====+>>>>>t_port
#include "QapGenStruct.inl"
//<<<<<+=====t_port
public:
};
class t_node{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_node)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(bool,enabled,SET,true,$)\
ADDVAR(vec2d,pos,DEF,$,$)\
ADDVAR(double,r,SET,32,$)\
ADDVAR(vector<t_port>,ports,DEF,$,$)\
ADDEND()
//=====+>>>>>t_node
#include "QapGenStruct.inl"
//<<<<<+=====t_node
public:
};
class t_endpoint{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_endpoint)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TWeakPtr<t_node>,n,DEF,$,$)\
ADDVAR(int,port_id,SET,0,$)\
ADDEND()
//=====+>>>>>t_endpoint
#include "QapGenStruct.inl"
//<<<<<+=====t_endpoint
public:
};
class t_link{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_link)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(bool,enabled,SET,true,$)\
ADDVAR(t_endpoint,a,DEF,$,$)\
ADDVAR(t_endpoint,b,DEF,$,$)\
ADDVAR(double,line_size,SET,4,$)\
ADDVAR(vector<vec2d>,line,DEF,$,$)\
ADDEND()
//=====+>>>>>t_link
#include "QapGenStruct.inl"
//<<<<<+=====t_link
public:
};
class t_layer{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_layer)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(string,name,DEF,$,$)\
ADDVAR(vector<t_node>,nodes,DEF,$,$)\
ADDVAR(vector<t_link>,links,DEF,$,$)\
ADDEND()
//=====+>>>>>t_layer
#include "QapGenStruct.inl"
//<<<<<+=====t_layer
public:
};
class i_cmd{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(i_cmd)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDEND()
//=====+>>>>>i_cmd
#include "QapGenStruct.inl"
//<<<<<+=====i_cmd
public:
};
class t_cmd_add_node:public i_cmd{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_cmd_add_node)PARENT(i_cmd)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(vec2d,pos,DEF,$,$)\
ADDEND()
//=====+>>>>>t_cmd_add_node
#include "QapGenStruct.inl"
//<<<<<+=====t_cmd_add_node
public:
};
class t_cmd_add_link:public i_cmd{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_cmd_add_link)PARENT(i_cmd)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(t_link,link,DEF,$,$)\
ADDEND()
//=====+>>>>>t_cmd_add_link
#include "QapGenStruct.inl"
//<<<<<+=====t_cmd_add_link
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
ADDVAR(vector<TAutoPtr<i_cmd>>,cmd,DEF,$,$)\
ADDVAR(TWeakPtr<t_node>,sn,DEF,$,$)\
ADDEND()
//=====+>>>>>t_world
#include "QapGenStruct.inl"
//<<<<<+=====t_world
public:
};

class TGame:public TQapGameV2{
public:
  IEnvRTTI*pEnv=nullptr;
  string fn="save_node_editor.qap";
  t_world w;
  t_link nl;
  double mat_size=32;
  struct t_sel_endpoint{bool a=false;int id=-1;};
  t_sel_endpoint sep;
  struct t_node_with_port{
    TWeakPtr<t_node> n;
    int p=-1;
  };
  t_node_with_port npa;
  t_node_with_port npb;
  vector<vec2d> points;
public:
  void DoInit(){

  }
  void DoMigrate(){
    if(!QapPublicUberFullLoaderBinLastHope(*pEnv,QapRawUberObject(w),fn)){
      DoInit();
      int gg=1;
    }
  }
  void DoFixLinkPorts(t_link&ref){
    if(!ref.a.n||!ref.b.n)return;
    struct t_best{bool ok=false;int a;int b;int d;void aib(const t_best&c){if(!ok||c.d<d)*this=c;}};
    t_best best;
    int aid=0;
    for(auto&a:ref.a.n->ports){
      int bid=0;
      for(auto&b:ref.b.n->ports){
        auto pa=ref.a.n->pos+a.offset;
        auto pb=ref.b.n->pos+b.offset;
        t_best cur={true,aid,bid,pa.sqr_dist_to(pb)};
        best.aib(cur);
        bid++;
      }
      aid++;
    }
    if(!best.ok)return;
    ref.a.port_id=best.a;
    ref.b.port_id=best.b;
  }
  void DoUpdate(){
    if(kb.OnDown(VK_F1)){
      for(auto&ex:w.cur.nodes){
        auto r=rand()*Pi2/RAND_MAX;
        ex.ports.clear();
        auto n=rand()%4+3;
        for(int i=0;i<n;i++)qap_add_back(ex.ports).offset=Vec2dEx(r+Pi2*1.0*i/n,ex.r);
      }
      for(auto&ex:w.cur.links){
        DoFixLinkPorts(ex);
      }
    }
    if(kb.OnDown(VK_F10)){
      for(auto&ex:w.cur.nodes){
        ex.ports.clear();
      }
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
    /*if(kb.OnDown(mbLeft)){
      nl.a.n=FindNodeAt(mpos);
    }
    if(kb.Down[mbLeft]){
      nl.b.n=FindNodeAt(mpos);
    }
    if(kb.OnUp(mbLeft)){
      qap_add_back(w.cur.links)=std::move(nl);
    }*/
    if(kb.OnDown(mbLeft)){
      npa=std::move(FindNP(mpos));
      points.clear();
    }
    if(kb.Down[mbLeft]){
      npb=std::move(FindNP(mpos));
      points.push_back(mpos);
    }
    if(kb.OnUp(mbLeft)){
      auto&b=qap_add_back(w.cur.links);
      b.a.n=npa.n;
      b.a.port_id=npa.p;
      b.b.n=npb.n;
      b.b.port_id=npb.p;
      b.line=std::move(points);
    }
    if(kb.OnDown(mbRight)){
      qap_add_back(w.cur.nodes).pos=mpos;
    }
    if(kb.OnDown(mbMiddle)){
      w.sn=FindNodeAt(mpos);
    }
    if(kb.Down(mbMiddle)){
      if(w.sn)w.sn->pos=mpos;
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
  vec2d get_offset(const t_endpoint&e){
    if(!e.n)return vec2d(0,0);
    return qap_check_id(e.n->ports,e.port_id)?e.n->ports[e.port_id].offset:vec2d(0,0);
  }
  void Draw(const t_link&ref){
    if(!ref.enabled)return;
    qDev.color=0xff000000;
    if(ref.a.n){
      auto&n=*ref.a.n.get();
      auto p=n.pos+get_offset(ref.a);
      qDev.DrawCircleEx(p,0,8,24,0);
    }
    qDev.color=0xff000000;
    if(ref.b.n){
      auto&n=*ref.b.n.get();
      auto p=n.pos+get_offset(ref.b);
      qDev.DrawCircleEx(p,0,8,24,0);
    }
    if(ref.a.n)if(ref.b.n){
      auto a=ref.a.n->pos+get_offset(ref.a);
      auto b=ref.b.n->pos+get_offset(ref.b);
      qDev.color=0xff000000;
      if(ref.line.size()){
        qDev.DrawLine(a,ref.line.front(),ref.line_size);
        qDev.DrawLine(b,ref.line.back(),ref.line_size);
        qDev.DrawPolyLine(ref.line,ref.line_size,false);
      }else{
        qDev.DrawLine(a,b,ref.line_size);
      }
    }
  }
  void Draw(const t_node&ref){
    if(!ref.enabled)return;
    qDev.color=0xff000000;
    qDev.DrawCircleEx(ref.pos,ref.r-4,ref.r,24,0);
    for(auto&ex:ref.ports){
      qDev.color=0xff000000;
      qDev.DrawCircleEx(ref.pos+ex.offset,0,ex.r,24,0);
    }
  }
  void Draw(const t_layer&ref){
    Draw(ref.nodes);
    Draw(ref.links);
  }
  template<class TYPE>
  void Draw(const vector<TYPE>&arr){for(auto&ex:arr)Draw(ex);}
  void DoDraw()override{
    QapDev::BatchScope Scope(qDev);
    qDev.color=0xff101010;
    Draw(w.cur);
    if(kb.Down[mbLeft])Draw(nl);
    string s="layer_id = "+IToS(w.layer_id);
    auto p=vec2d(-512,+512);
    qDev.color=0xff000000;
    qap_text::draw(qDev,p+vec2d(+1,-1),s);
    qDev.color=0xffffffff;
    qap_text::draw(qDev,p,s);
  }
  t_node_with_port FindNP(const vec2d&p){
    t_node_with_port np;
    for(auto&ex:w.cur.nodes){
      int i=0;
      for(auto&port:ex.ports){
        if((ex.pos+port.offset).dist_to_point_less_that_r(p,port.r))return {&ex,i};
        i++;
      }
    }
    return np;
  }
  t_node*FindNodeAt(const vec2d&p){
    for(auto&ex:w.cur.nodes)
      if((ex.pos-p).SqrMag()<mat_size*mat_size*0.25)return &ex;
    return nullptr;
  }/*
  t_sel_endpoint FindSEP(const vec2d&p){
    int i=0;
    for(auto&ex:w.cur.links){
      if(ex.enabled){
        if(ex.a.pos.dist_to_point_less_that_r(p,max(4.0,ex.a.r)))return {true,i};
        if(ex.b.pos.dist_to_point_less_that_r(p,max(4.0,ex.b.r)))return {false,i};
      }
      i++;
    }
    return {};
  }*/
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