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
ADDVAR(QapColor,color,SET,0xff000000,$)\
ADDVAR(bool,provider,SET,false,$)\
ADDVAR(vector<t_port>,ports,DEF,$,$)\
ADDVAR(string,name,DEF,$,$)\
ADDEND()
//=====+>>>>>t_node
#include "QapGenStruct.inl"
//<<<<<+=====t_node
public:
  t_node&set(bool prov,vec2d p){pos=p;provider=prov;return *this;}
};
class t_endpoint{
public:
#define DEF_PRO_AUTO_COPY
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
#define DEF_PRO_AUTO_COPY
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
ADDVAR(vector<TAutoPtr<i_cmd>>,cmds,DEF,$,$)\
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
    if(ref.line.size())return;
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
  void DoOptLink(t_link&ref){
    if(!ref.a.n||!ref.b.n)return;
    auto pa=ref.a.n->pos+get_offset(ref.a);
    auto pb=ref.b.n->pos+get_offset(ref.b);
    vector<vec2d> out;out.reserve(ref.line.size()/2);
    auto&arr=ref.line;
    for(int i=0;i+1<arr.size();i+=2){
      auto&a=arr[i];
      auto&b=arr[i+1];
      qap_add_back(out)=0.5*(a+b);
    }
    arr=std::move(out);
  }
  void DoOptLink3(t_link&ref){
    if(!ref.a.n||!ref.b.n)return;
    auto pa=ref.a.n->pos+get_offset(ref.a);
    auto pb=ref.b.n->pos+get_offset(ref.b);
    auto&arr=ref.line;auto n=arr.size();
    for(int i=0;i<n;i++){
      auto&a=i-1<0?pa:arr[i-1];
      auto&b=arr[i];
      auto&c=i+2>n?pb:arr[i+1];
      b=(a+b+c)*(1.0/3.0);
    }
  }
  struct t_color_accum{
    vec4f col=vec4f(0,0,0,0);
    int n=0;
    void add(QapColor c){
      col+=vec4f(c.b,c.g,c.r,c.a);
      n++;
    }
    QapColor get()const{
      auto c=col;
      c=(n?1.0/(n*255):0)*c;
      c.a=255.0;
      return c.GetColor();
    }
  };
  void DoProp(){
    auto&nodes=w.cur.nodes;
    auto&links=w.cur.links;
    const int ITER=800;
    for(int it=0;it<ITER;it++){
      vector<t_color_accum> acc(nodes.size());
      for(auto&L:links){
        if(!L.enabled)continue;
        if(!L.a.n||!L.b.n)continue;
        auto*na=L.a.n.get();
        auto*nb=L.b.n.get();
        int ia=int(na-&nodes[0]);
        int ib=int(nb-&nodes[0]);
        if(na->provider||1){
          acc[ib].add(na->color);
        }
        if(nb->provider||1){
          acc[ia].add(nb->color);
        }
      }
      for(int i=0;i<nodes.size();i++){
        auto&n=nodes[i];
        if(n.provider)continue;
        if(!acc[i].n)continue;
        n.color=acc[i].get();//QapColor::Mix(n.color,acc[i].get(),0.5);
      }
    }
  }
  void DoPropWithBFS(){
    auto&nodes=w.cur.nodes;
    auto&links=w.cur.links;
    vector<t_node*> cur,next;map<t_node*,int> V;
    map<t_node*,t_color_accum> acc;
    for(auto&ex:nodes)if(ex.provider){cur.push_back(&ex);acc[&ex].add(ex.color);}
    for(int iter=1;cur.size();iter++){
      for(auto&f:cur){
        for(auto&ex:links){
          if(!ex.a.n||!ex.b.n)continue;
          if(ex.a.n.get()!=f&&ex.b.n.get()!=f)continue;
          auto*oe=ex.a.n.get()==f?ex.b.n.get():ex.a.n.get();
          if(oe->provider||oe==f)continue;
          acc[oe].add(acc[f].get());
          auto&v=V[oe];if(v)continue;v=iter;
          next.push_back(oe);
        }
      }
      cur=std::move(next);
    }
    for(int i=0;i<nodes.size();i++){
      auto&n=nodes[i];
      if(n.provider)continue;
      if(!acc[&n].n)continue;
      n.color=acc[&n].get();//QapColor::Mix(n.color,acc[i].get(),0.5);
      n.name=IToS(acc[&n].n);
    }
  }
  void DoUpdate(){
    if(kb.OnDown(VK_F1)){
      for(auto&ex:w.cur.nodes){
        if(ex.ports.size())continue;
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
    if(kb.OnDown(VK_F9)){
      for(auto&ex:w.cur.links){
        DoOptLink(ex);
      }
    }
    if(kb.OnDown(VK_F8)){
      for(auto&ex:w.cur.links){
        DoOptLink3(ex);
      }
    }
    if(kb.OnDown(VK_F7)){
      auto d=400/5.0;
      qap_add_back(w.cur.nodes).set(1,vec2d(-300,-2*d)).color=0xffffff00;
      qap_add_back(w.cur.nodes).set(1,vec2d(-300,-1*d)).color=0xffff8800;
      qap_add_back(w.cur.nodes).set(1,vec2d(-300,-0*d)).color=0xff0088ff;
      qap_add_back(w.cur.nodes).set(1,vec2d(-300,+1*d)).color=0xff44ff44;
      qap_add_back(w.cur.nodes).set(1,vec2d(-300,+2*d)).color=0xffaa44aa;
      //qap_add_back(w.cur.nodes).set(1,vec2d(-300,+2*d)).color=0xffff0000;
    }
    if(kb.OnDown(VK_F6)){
      DoPropWithBFS();//DoProp();
    }
    if(kb.OnDown(VK_F2)){
      auto&layer=qap_add_back(w.layers);
      layer=std::move(w.cur);
    }
    if(kb.OnDown(VK_F3)){
      for(auto&ex:w.cur.nodes){
        ex.color=0xff000000;
        ex.provider=false;
        ex.name.clear();
      }
    }
    if(kb.OnDown('~')){
      QapCleanIf(w.cur.links,[](t_link&ex){return !ex.a.n||!ex.b.n;});
      w.layers.clear();
      //for(int i=0;i<w.layers.size();i++)if(i!=28)w.layers[i]={};
    }
    if(kb.OnDown('~')){
      w.cur.links.clear();
    }
    if(kb.OnDown('`')){
      for(auto&ex:w.cur.nodes){
        if(!ex.provider)ex.color=0xff000000;
      }
    }
    if(kb.OnDown('`')){
      for(auto&ex:w.cur.nodes){
        ex.name.clear();
      }
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
      if(points.empty()||points.back()!=mpos)points.push_back(mpos);
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
      if(kb.Down[VK_SHIFT])QapPublicUberFullSaverProto(*pEnv,QapRawUberObject(w),fn+".proto");
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
    /*qDev.color=0xff000000;
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
    }*/
    if(ref.a.n)if(ref.b.n){
      auto a=ref.a.n->pos+get_offset(ref.a);
      auto b=ref.b.n->pos+get_offset(ref.b);
      qDev.color=0xff000000;
      if(ref.line.size()){
        qDev.DrawLine(a,ref.line.front(),ref.line_size);
        qDev.DrawLine(b,ref.line.back(),ref.line_size);
        if(ref.a.n->provider||ref.b.n->provider){
          qDev.DrawPolyLine(ref.line,ref.line_size,false);
        }else{
          DrawPolyLine(ref.line,ref.a.n->color,ref.b.n->color,ref.line_size);
        }
      }else{
        qDev.DrawLine(a,b,ref.line_size);
      }
    }
  }
  void Draw(const t_node&ref){
    if(!ref.enabled)return;
    if(!ref.provider){
      qDev.color=ref.color;
      qDev.DrawCircleEx(ref.pos,0,ref.r,24,0);
    }else{
      qDev.color=ref.color;
      qDev.DrawCircleEx(ref.pos,0,16,24,0);
      qDev.color=0xff000000;
      qDev.DrawCircleEx(ref.pos,16,ref.r,24,0);
    }
    qDev.color=0xff000000;
    qDev.DrawCircleEx(ref.pos,ref.r-4,ref.r,24,0);
    for(auto&ex:ref.ports){
      qDev.color=ref.provider?ref.color:0xff000000;
      qDev.DrawCircleEx(ref.pos+ex.offset,0,ex.r,24,0);
    }
    auto p=ref.pos+vec2d(-12+5,+12+3);
    qDev.color=0xff000000;
    qap_text::draw(qDev,p+vec2d(+1,-1),ref.name);
    qDev.color=0xffffffff;
    qap_text::draw(qDev,p,ref.name);
  }
  void Draw(const t_layer&ref){
    Draw(ref.nodes);
    Draw(ref.links);
  }
  void DrawPolyLine(const vector<vec2d>&arr,QapColor b,QapColor e,double line_size){
    for(int i=0;i+1<arr.size();i++){
      auto&pa=arr[i+0];
      auto&pb=arr[i+1];
      qDev.color=0xff000000;
      qDev.DrawLine(pa,pb,line_size+4);
      qDev.color=QapColor::Mix(b,e,i*1.0/arr.size());
      qDev.DrawLine(pa,pb,line_size);
    }
  }
  template<class TYPE>
  void Draw(const vector<TYPE>&arr){for(auto&ex:arr)Draw(ex);}
  void DoDraw()override{
    QapDev::BatchScope Scope(qDev);
    qDev.color=0xff101010;
    Draw(w.cur);
    qDev.color=0xff101010;
    qDev.DrawPolyLine(points,t_link().line_size,false);
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