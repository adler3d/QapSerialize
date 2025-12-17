#define DEF_PRO_FULL
#define ThisCompileUnit ThisCompileUnit$QapPublicUberXImpl
#define ThisCompileUnitInstance ThisCompileUnitInstance$QapPublicUberXImpl
#include "QapPublicUberXImpl.cpp"
#undef ThisCompileUnitInstance
#undef ThisCompileUnit

template<class TYPE>int qap_includes(const vector<TYPE>&arr,const TYPE&value){for(int i=0;i<arr.size();i++){if(arr[i]==value)return true;}return false;}
bool TDynStructFactory_set_not_sys_field_from_string(TType*p,const string&source,void*pValue)
{
  QapDebugMsg("no impl, but who care?");
  return true;
}

#include <iostream>
//#include "last_hope_loader.cpp"
bool QapPublicUberFullLoaderBinLastHope(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename);
class t_node{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_node)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(string,name,DEF,$,$)\
ADDVAR(vector<t_node>,arr,DEF,$,$)\
ADDVAR(vector<TWeakPtr<t_node>>,links,DEF,$,$)\
ADDEND()
//=====+>>>>>t_some_class
#include "QapGenStruct.inl"
//<<<<<+=====t_some_class
public:
};
class t_some_class{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_some_class)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(t_node,n,DEF,$,$)\
ADDVAR(int,b,SET,10,$)\
ADDVAR(double,c,SET,20,$)\
ADDVAR(string,more,SET,"2025.12.13 18:07",$)\
ADDVAR(double,z,SET,30,$)\
ADDEND()
//=====+>>>>>t_some_class
#include "QapGenStruct.inl"
//<<<<<+=====t_some_class
public:
};

void main_2025_1(IEnvRTTI&Env){
  t_some_class var;
  if(!QapPublicUberFullLoaderBinLastHope(Env,QapRawUberObject(var),"out1.qap")){
    var.n.links.push_back(&var.n);
    var.n.arr.push_back(std::move(t_node()));
    var.n.arr.push_back(std::move(t_node()));
    var.n.arr.push_back(std::move(t_node()));
    var.n.links.push_back(&var.n.arr[2]);
    var.n.links.push_back(&var.n.arr[1]);
    var.n.links.push_back(&var.n.arr[0]);
    var.n.arr[1].links.push_back(&var.n.arr[2]);
  }
  TQapFileStream fsproto("out1.qap.proto",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceProto(fsproto));
  TQapFileStream fs("out1.qap",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceBin(fs));
  int gg=1;
}

class i_node{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(i_node)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDEND()
//=====+>>>>>i_node
#include "QapGenStruct.inl"
//<<<<<+=====i_node
public:
  virtual void check(){QapNoWay();}
};

class t_node2:public i_node{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_node2)PARENT(i_node)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(string,name,DEF,$,$)\
ADDVAR(vector<TAutoPtr<i_node>>,arr,DEF,$,$)\
ADDVAR(vector<TWeakPtr<i_node>>,links,DEF,$,$)\
ADDEND()
//=====+>>>>>t_some_class
#include "QapGenStruct.inl"
//<<<<<+=====t_some_class
public:
};
class t_leaf:public i_node{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_leaf)PARENT(i_node)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(string,name,DEF,$,$)\
ADDVAR(string,payload,DEF,$,$)\
ADDVAR(TFieldPtr,fieldptr,DEF,$,$)\
ADDEND()
//=====+>>>>>t_leaf
#include "QapGenStruct.inl"
//<<<<<+=====t_leaf
public:
  void check()override{
    QapAssert(name=="leaf");
    QapAssert(payload=="foo");
  }
};
class t_some_class2{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_some_class2)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(t_node2,n,DEF,$,$)\
ADDVAR(double,z,SET,30,$)\
ADDEND()
//=====+>>>>>t_some_class2
#include "QapGenStruct.inl"
//<<<<<+=====t_some_class2
public:
};

namespace t_before{
  class t_old_stuff{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_old_stuff)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(string,name,DEF,$,$)\
  ADDVAR(vector<t_old_stuff>,arr,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_old_stuff
  #include "QapGenStruct.inl"
  //<<<<<+=====t_old_stuff
  };
  class t_node2:public i_node{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_node2)PARENT(i_node)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(string,name,DEF,$,$)\
  ADDVAR(vector<TAutoPtr<i_node>>,arr,DEF,$,$)\
  ADDVAR(vector<TWeakPtr<i_node>>,links,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_node2
  #include "QapGenStruct.inl"
  //<<<<<+=====t_node2
  public:
    t_node2*init(const string&Name){name=Name;return this;}
  };
  class t_leaf:public i_node{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_leaf)PARENT(i_node)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(t_old_stuff,old_stuff,DEF,$,$)\
  ADDVAR(string,payload,DEF,$,$)\
  ADDVAR(string,payload2,DEF,$,$)\
  ADDVAR(TFieldPtr,fieldptr,DEF,$,$)\
  ADDVAR(string,payload3,DEF,$,$)\
  ADDVAR(string,name,DEF,$,$)\
  ADDEND()
  //=====+>>>>>t_leaf
  #include "QapGenStruct.inl"
  //<<<<<+=====t_leaf
  public:
    t_leaf*init(IEnvRTTI&Env){
      payload="foo";
      payload2="bar";
      payload3="baz";
      name="leaf";
      qap_add_back(qap_add_back(old_stuff.arr).arr).name="inner";
      qap_add_back(old_stuff.arr).name="object";
      old_stuff.name="stuff";
      return this;
    }
  };
  class t_some_class2{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_some_class2)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(double,z,SET,360,$)\
  ADDVAR(t_node2,n,DEF,$,$)\
  ADDVAR(double,x,SET,30,$)\
  ADDEND()
  //=====+>>>>>t_some_class2
  #include "QapGenStruct.inl"
  //<<<<<+=====t_some_class2
  public:
  };
  void init(IEnvRTTI&Env,t_some_class2&var){
    var.n.links.push_back(&var.n);
    i_node*ptr=nullptr;
    var.n.arr.push_back({});auto&N1=*var.n.arr[0].build<t_node2>(Env);N1.name="N1";
    var.n.arr.push_back({});var.n.arr[1].build<t_leaf>(Env)->init(Env)->fieldptr.ConnectToField(Env,N1,N1.links);
    var.n.arr.push_back({});var.n.arr[2].build<t_node2>(Env)->name="N2";
    var.n.links.push_back(var.n.arr[2].get());
    var.n.links.push_back(var.n.arr[1].get());
    var.n.links.push_back(var.n.arr[0].get());
    N1.links.push_back(var.n.arr[2].get());
  }
};

void main_2025(IEnvRTTI&Env){
  {
    TStdAllocator MA;
    TEnvRTTI Env2;
    Env2.Arr.reserve(1024);
    Env2.Alloc=&MA;
    Env2.OwnerEnv=&Env2;

    t_before::t_some_class2 var;
    t_before::init(Env2,var);
    TQapFileStream fsproto("out_before.qap.proto",false);
    QapUberFullSaver(Env2,QapRawUberObject(var),UberSaveDeviceProto(fsproto));
    TQapFileStream fs("out.qap",false);
    QapUberFullSaver(Env2,QapRawUberObject(var),UberSaveDeviceBin(fs));
  }
  Sys$$<t_node2>::GetRTTI(Env);Sys$$<t_leaf>::GetRTTI(Env);
  t_some_class2 var;
  if(!QapPublicUberFullLoaderBinLastHope(Env,QapRawUberObject(var),"out.qap")){
    int gg=1;
  }
  var.n.arr[1]->check();
  QapAssert(var.z==360);
  TQapFileStream fsproto("out.qap.proto",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceProto(fsproto));
  TQapFileStream fs("out.qap",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceBin(fs));
  int gg=1;
}
namespace t_channel_test{
class t_channel{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_channel)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(string,tag,DEF,$,$)\
ADDEND()
//=====+>>>>>t_channel
#include "QapGenStruct.inl"
//<<<<<+=====t_channel
public:
};
class t_subscriber;
class t_publisher{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_publisher)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(vector<TWeakPtr<t_subscriber>>,arr,DEF,$,$)\
ADDVAR(string,name,DEF,$,$)\
ADDVAR(string,payload,DEF,$,$)\
ADDVAR(TWeakPtr<t_channel>,channel,DEF,$,$)\
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
ADDVAR(TWeakPtr<t_publisher>,publisher,DEF,$,$)\
ADDVAR(vector<string>,content,DEF,$,$)\
ADDVAR(int,priority,SET,0,$)\
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
namespace t_before{
  class t_subscriber;
  class t_publisher{
  public:
  #define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_publisher)
  #define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
  ADDBEG()\
  ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
  ADDVAR(vector<TWeakPtr<t_subscriber>>,arr,DEF,$,$)\
  ADDVAR(string,name,DEF,$,$)\
  ADDVAR(string,content,DEF,$,$)\
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
  inline t_publisher&InitPublisher_V1(t_world&world,const string&name,const string&content,const string&script)
  {
    auto&p=qap_add_back(world.parr);
    p.name=name;
    p.content=content;
    p.script=script;
    return p;
  }
  inline t_subscriber&InitSubscriber_V1(t_world&world,t_publisher&pub,const vector<string>&content)
  {
    auto&s=qap_add_back(world.sarr);
    s.publisher=&pub;
    s.content=content;
    pub.arr.push_back(&s);
    return s;
  }
  inline t_world InitTestWorld_V1()
  {
    t_world world;
    world.parr.reserve(1024);
    auto&p1=InitPublisher_V1(world,"news","hello","print('news')");
    auto&p2=InitPublisher_V1(world,"alerts","system","print('alert')");
    InitSubscriber_V1(world,p1,{"a","b"});
    InitSubscriber_V1(world,p1,{"c"});
    InitSubscriber_V1(world,p2,{"x"});
    return world;
  }
};

inline void TestMigratedWorld_V2(t_world&world){
  QapAssert(world.parr.size()==2);
  QapAssert(world.sarr.size()==3);
  int news_cnt=0;
  int alert_cnt=0;
  for(auto&p:world.parr){
    QapAssert(!p.payload.size());
    QapAssert(p.script.size());
    if(p.name=="news"){
      QapAssert(p.script=="print('news')");
      news_cnt++;
    }
    if(p.name=="alerts"){
      QapAssert(p.script=="print('alert')");
      alert_cnt++;
    }
    for(auto&s:p.arr){
      QapAssert(s);
      QapAssert(s->publisher.get()==&p);
    }
  }
  QapAssert(news_cnt==1);
  QapAssert(alert_cnt==1);
  for(auto&s:world.sarr){
    QapAssert(s.publisher);
    QapAssert(s.priority==0);
  }
}
void main_2025_3(IEnvRTTI&Env){
  {
    TStdAllocator MA;
    TEnvRTTI Env2;
    Env2.Arr.reserve(1024);
    Env2.Alloc=&MA;
    Env2.OwnerEnv=&Env2;

    t_before::t_world var=t_before::InitTestWorld_V1();
    TQapFileStream fsproto("out3bef.qap.proto",false);
    QapUberFullSaver(Env2,QapRawUberObject(var),UberSaveDeviceProto(fsproto));
    TQapFileStream fs("out3bef.qap",false);
    QapUberFullSaver(Env2,QapRawUberObject(var),UberSaveDeviceBin(fs));
  }
  t_world var;
  if(!QapPublicUberFullLoaderBinLastHope(Env,QapRawUberObject(var),"out3bef.qap")){
    int gg=1;
  }
  TestMigratedWorld_V2(var);
  TQapFileStream fsproto("out3aft.qap.proto",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceProto(fsproto));
  TQapFileStream fs("out3aft.qap",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceBin(fs));
  int gg=1;
}
}
void main_2025_2(IEnvRTTI&Env){
  t_some_class2 var;
  Sys$$<t_node2>::GetRTTI(Env);Sys$$<t_leaf>::GetRTTI(Env);
  if(!QapPublicUberFullLoaderBinLastHope(Env,QapRawUberObject(var),"out2.qap")){
    var.n.links.push_back(&var.n);
    var.n.arr.push_back({});auto&N1=*var.n.arr[0].build<t_node2>(Env);N1.name="N1";
    var.n.arr.push_back({});var.n.arr[1].build<t_leaf>(Env)->fieldptr.ConnectToField(Env,N1,N1.links);
    var.n.arr.push_back({});var.n.arr[2].build<t_node2>(Env)->name="N2";
    var.n.links.push_back(var.n.arr[2].get());
    var.n.links.push_back(var.n.arr[1].get());
    var.n.links.push_back(var.n.arr[0].get());
    N1.links.push_back(var.n.arr[2].get());
  }
  TQapFileStream fsproto("out2.qap.proto",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceProto(fsproto));
  TQapFileStream fs("out2.qap",false);
  QapUberFullSaver(Env,QapRawUberObject(var),UberSaveDeviceBin(fs));
  int gg=1;
}

int main()
{
  return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}

#include "main2.inl"
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
  void win_main_init(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);
  win_main_init(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
  auto&comp_unit_man=TCompileUnitMan::get_man();
  //string unit_name=comp_unit_man.arr[0]->get_filename();
  //static GlobalEnv gEnv(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
  TStdAllocator MA;
  {
    TEnvRTTI Env;
    Env.Arr.reserve(1024);
    Env.Alloc=&MA;
    Env.OwnerEnv=&Env;
    TCompileUnitMan::reg_and_run_all(Env);
    main_2025(Env);
    main_2025_1(Env);
    main_2025_2(Env);
    t_channel_test::main_2025_3(Env);
    if(0)Env.OwnerEnv=nullptr;
  }
  return 0;
}


//-----
#undef DEF_PRO_FULL