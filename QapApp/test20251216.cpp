#define DEF_PRO_FULL
#include "StdAfx.h"
#include "QapLiteCutted.h"
#include "TQapGameV2.inl"

namespace NS_V0{
  class t_subscriber;
  class t_node{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_node)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(vec2d,pos,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_node
  #include "QapGenStruct.inl"
  //<<<<<+=====t_node
  public:
  };
  class t_publisher{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_publisher)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
  ADDVAR(vector<TWeakPtr<t_subscriber>>,arr,DEF,$,$)\
  ADDVAR(vec2d,pos,DEF,$,$)\
  ADDVAR(vector<t_node>,narr,DEF,$,$)\
  ADDVAR(string,name,DEF,$,$)\
  ADDVAR(string,payload,DEF,$,$)\
  ADDVAR(string,script,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_publisher
  #include "QapGenStruct.inl"
  //<<<<<+=====t_publisher
  public:
  };
  class t_subscriber{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_subscriber)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
  ADDVAR(string,name,DEF,$,$)\
  ADDVAR(vec2d,pos,DEF,$,$)\
  ADDVAR(TWeakPtr<t_publisher>,publisher,DEF,$,$)\
  ADDVAR(vector<string>,content,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_subscriber
  #include "QapGenStruct.inl"
  //<<<<<+=====t_subscriber
  public:
  };
  class t_world{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_world)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(vector<t_publisher>,parr,DEF,$,$)\
  ADDVAR(vector<t_subscriber>,sarr,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_world
  #include "QapGenStruct.inl"
  //<<<<<+=====t_world
  public:
  };
  inline t_publisher&InitPublisher(t_world&world,const string&name,const string&script)
  {
    auto&p=qap_add_back(world.parr);
    p.name=name;
    p.pos=vec2d(rand()*512.0/RAND_MAX,rand()*512.0/RAND_MAX);
    p.script=script;
    return p;
  }
  inline t_subscriber&InitSubscriber(t_world&world,const string&name,t_publisher&pub,const vector<string>&content)
  {
    auto&s=qap_add_back(world.sarr);
    s.name=name;
    s.pos=vec2d(rand()*512.0/RAND_MAX,rand()*512.0/RAND_MAX);
    s.publisher=&pub;
    s.content=content;
    pub.arr.push_back(&s);
    return s;
  }
  inline void InitTestWorld(t_world&world)
  {
    world.parr.reserve(1024);
    auto&p1=InitPublisher(world,"news","print('news')");
    auto&p2=InitPublisher(world,"alerts","print('alert')");
    InitSubscriber(world,"S1",p1,{"a","b"});
    InitSubscriber(world,"S2",p1,{"c"});
    InitSubscriber(world,"S3",p2,{"x"});
  }
  void update(NS_V0::t_world&w)
  {
    for(auto&p:w.parr)
    {
      vec2d delta(
        (rand()-RAND_MAX/2)*10.0/RAND_MAX,
        (rand()-RAND_MAX/2)*10.0/RAND_MAX
      );
      p.pos+=delta;
      NS_V0::t_node n;
      n.pos=p.pos;
      p.narr.push_back(std::move(n));
    }
  }
};
namespace NS_V1{
  class t_subscriber;
  class t_node{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_node)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(vec2d,pos,DEF,$,$)\
  ADDVAR(TWeakPtr<t_subscriber>,sub,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_node
  #include "QapGenStruct.inl"
  //<<<<<+=====t_node
  public:
  };
  class t_publisher{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_publisher)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
  ADDVAR(vector<TWeakPtr<t_subscriber>>,arr,DEF,$,$)\
  ADDVAR(vec2d,pos,DEF,$,$)\
  ADDVAR(vector<t_node>,narr,DEF,$,$)\
  ADDVAR(string,name,DEF,$,$)\
  ADDVAR(string,payload,DEF,$,$)\
  ADDVAR(string,script,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_publisher
  #include "QapGenStruct.inl"
  //<<<<<+=====t_publisher
  public:
  };
  class t_subscriber{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_subscriber)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
  ADDVAR(string,name,DEF,$,$)\
  ADDVAR(vec2d,pos,DEF,$,$)\
  ADDVAR(TWeakPtr<t_publisher>,publisher,DEF,$,$)\
  ADDVAR(vector<string>,content,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_subscriber
  #include "QapGenStruct.inl"
  //<<<<<+=====t_subscriber
  public:
  };
  class t_world{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_world)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(vector<t_publisher>,parr,DEF,$,$)\
  ADDVAR(vector<t_subscriber>,sarr,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_world
  #include "QapGenStruct.inl"
  //<<<<<+=====t_world
  public:
  };
  void update(t_world&w)
  {
    for(auto&p:w.parr)
    {
      vec2d delta(
        (rand()-RAND_MAX/2)*100.0/RAND_MAX,
        (rand()-RAND_MAX/2)*100.0/RAND_MAX
      );
      p.pos+=delta;
      NS_V1::t_node n;
      n.pos=p.pos;
      int sc=int(p.arr.size());
      n.sub=p.arr[rand()%sc];
      p.narr.push_back(std::move(n));
    }
  }
};

bool QapPublicUberFullLoaderBinLastHope(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
bool QapPublicUberFullSaverProto(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
bool QapPublicUberFullSaverBin(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);

class TGame:public TQapGameV2{
public:
  int tick=0;
  bool migrated=false;
  NS_V0::t_world w0;
  NS_V1::t_world w1;
public:
  void DoMigrate(){
    {
      TStdAllocator MA;
      TEnvRTTI Env2;
      Env2.Arr.reserve(1024);
      Env2.Alloc=&MA;
      Env2.OwnerEnv=&Env2;
      //QapPublicUberFullSaverProto(Env2,QapRawUberObject(w0),"out20251216bef.qap.proto");
      QapPublicUberFullSaverBin(Env2,QapRawUberObject(w0),"out20251216bef.qap");
    }
    {
      TStdAllocator MA;
      TEnvRTTI Env3;
      Env3.Arr.reserve(1024);
      Env3.Alloc=&MA;
      Env3.OwnerEnv=&Env3;
      if(!QapPublicUberFullLoaderBinLastHope(Env3,QapRawUberObject(w1),"out20251216bef.qap")){
        int gg=1;
      }
      //QapPublicUberFullSaverProto(Env3,QapRawUberObject(w1),"out20251216aft.qap.proto");
      //QapPublicUberFullSaverBin(Env,QapRawUberObject(var),"out20251216aft.qap");
    }
  }
  void DoMove()override{
    if(kb.OnDown(VK_ESCAPE)){
      TerminateProcess(GetCurrentProcess(),0);
    }
    if(tick==0){
      NS_V0::InitTestWorld(w0);
    }
    if(!migrated){
      NS_V0::update(w0);
      if(kb.OnDown(VK_SPACE)){
        DoMigrate();
        NS_V1::update(w1);
        migrated=true;
      }
    }else{
      if(tick%100==99){
        NS_V1::update(w1);
      }
    }
    tick++;
  }
  typedef QapColor uint32;
  inline void DrawPublisher(QapDev&qDev,const vec2d&p,uint32 color){
    qDev.color=color;
    qDev.DrawCircleEx(p,0,8,24,0);
  }
  inline void DrawSubscriber(QapDev&qDev,const vec2d&p,uint32 color){
    qDev.color=color;
    qDev.DrawCircleEx(p,0,4,16,0);
  }
  inline void DrawLink(QapDev&qDev,const vec2d&a,const vec2d&b,uint32 color){
    qDev.color=color;
    qDev.DrawLine(a,b,2);
  }
  template<class TNode>
  inline void DrawTrail(QapDev&qDev,const vector<TNode>&arr,uint32 color){
    if(arr.size()<2)return;
    qDev.color=color;
    for(int i=1;i<int(arr.size());i++){
      qDev.DrawLine(arr[i-1].pos,arr[i].pos,1);
    }
  }
  void DrawWorldV0(){
    for(auto&p:w0.parr){
      DrawTrail(qDev,p.narr,0xff808080);
      DrawPublisher(qDev,p.pos,0xff00aa00);
      for(auto&s:p.arr){
        if(!s)continue;
        DrawLink(qDev,p.pos,s->pos,0xff404040);
      }
    }
    for(auto&s:w0.sarr){
      DrawSubscriber(qDev,s.pos,0xff0000ff);
    }
  }
  void DrawWorldV1(){
    for(auto&p:w1.parr){
      DrawTrail(qDev,p.narr,0xff808080);
      DrawPublisher(qDev,p.pos,0xff00ff00);
      for(auto&n:p.narr){
        if(!n.sub)continue;
        DrawLink(qDev,n.pos,n.sub->pos,0xffaa0000);
      }
    }
    for(auto&s:w1.sarr){
      DrawSubscriber(qDev,s.pos,0xff0000ff);
    }
  }
  void DoDraw()override{
    QapDev::BatchScope Scope(qDev);
    qDev.color=0xff101010;
    //qDev.DrawCircleEx(vec2d(256,256),0,512,32,0);
    if(!migrated){
      DrawWorldV0();
    }else{
      DrawWorldV1();
    }
  }
  void DoDrawTestGAPI(){
    QapDev::BatchScope Scope(qDev);
    qDev.color=0xff000000;
    qDev.DrawCircleEx(vec2d(0,0),0,512,32,0);
    qDev.color=0xffff0000;
    qDev.DrawLine(vec2d(0,0),vec2d(0,512),4);
  }
};
void main_2025_12_16(IEnvRTTI&Env){
  TGame builder;builder.SleepMs=0;
  builder.DoNice();
  
  /*{
    TStdAllocator MA;
    TEnvRTTI Env2;
    Env2.Arr.reserve(1024);
    Env2.Alloc=&MA;
    Env2.OwnerEnv=&Env2;

    auto var=NS_V0::InitTestWorld();
    QapPublicUberFullSaverProto(Env2,QapRawUberObject(var),"out20251216bef.qap.proto");
    QapPublicUberFullSaverBin(Env2,QapRawUberObject(var),"out20251216bef.qap");
  }
  t_world var;
  if(!QapPublicUberFullLoaderBinLastHope(Env,QapRawUberObject(var),"out20251216bef.qap")){
    int gg=1;
  }
  TestMigratedWorld_V2(var);
  QapPublicUberFullSaverBin(Env,QapRawUberObject(var),"out20251216aft.qap.proto");
  QapPublicUberFullSaverBin(Env,QapRawUberObject(var),"out20251216aft.qap");
  int gg=1;*/
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