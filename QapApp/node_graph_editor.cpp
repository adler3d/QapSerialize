#define DEF_PRO_FULL
#include "StdAfx.h"
#include "QapLiteCutted.h"
#include "TQapGameV2.inl"
#include "curve_resampler.hpp"
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
ADDVAR(double,line_pulse_koef,SET,1,$)\
ADDEND()
//=====+>>>>>t_link
#include "QapGenStruct.inl"
//<<<<<+=====t_link
public:
};
class t_text{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_text)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(bool,enabled,SET,true,$)\
ADDVAR(string,text,DEF,$,$)\
ADDVAR(vec2d,pos,DEF,$,$)\
ADDVAR(vec2d,dir,DEF,$,$)\
ADDVAR(double,font_size,SET,24,$)\
ADDEND()
//=====+>>>>>t_text
#include "QapGenStruct.inl"
//<<<<<+=====t_text
public:
};
class t_fish{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_fish)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(bool,enabled,SET,true,$)\
ADDVAR(vector<vec2d>,line,DEF,$,$)\
ADDVAR(double,line_size,SET,4,$)\
ADDEND()
//=====+>>>>>t_fish
#include "QapGenStruct.inl"
//<<<<<+=====t_fish
public:
};
class t_layer_with_offset;
class t_layer{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_layer)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(string,name,DEF,$,$)\
ADDVAR(vector<t_node>,nodes,DEF,$,$)\
ADDVAR(vector<t_link>,links,DEF,$,$)\
ADDVAR(vector<t_text>,texts,DEF,$,$)\
ADDVAR(vector<t_fish>,fishs,DEF,$,$)\
ADDVAR(vector<t_layer_with_offset>,layers,DEF,$,$)\
ADDEND()
//=====+>>>>>t_layer
#include "QapGenStruct.inl"
//<<<<<+=====t_layer
public:
};
class t_layer_with_offset{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(t_layer_with_offset)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(TSelfPtr<SelfClass>,Self,DEF,$,$)\
ADDVAR(t_layer,L,DEF,$,$)\
ADDVAR(vec2d,offset,DEF,$,$)\
ADDVAR(double,scale,SET,1.0,$)\
ADDEND()
//=====+>>>>>t_layer_with_offset
#include "QapGenStruct.inl"
//<<<<<+=====t_layer_with_offset
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
ADDVAR(TWeakPtr<t_text>,st,DEF,$,$)\
ADDVAR(TWeakPtr<t_layer_with_offset>,sl,DEF,$,$)\
ADDVAR(bool,hq_links,SET,false,$)\
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
  double new_node_r=32;
public:
  void DoInit(){

  }
  void DoMigrate(){
    //t_world whabr;
    //QapPublicUberFullLoaderBinLastHope(*pEnv,QapRawUberObject(whabr),"save_node_editor.qap");
    if(1||!QapPublicUberFullLoaderBin(*pEnv,QapRawUberObject(w),fn)){
      if(!QapPublicUberFullLoaderBinLastHope(*pEnv,QapRawUberObject(w),fn)){
        DoInit();
        int gg=1;
      }
    }
    //w.cur=std::move(whabr.cur);
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
  void DoOptLine(vector<vec2d>&line){
    auto&arr=line;auto n=arr.size();
    vector<vec2d> out;out.resize(n);
    for(int i=0;i<n;i++){
      auto&a=i-1<0?arr[0+0]:arr[i-1];
      auto&b=arr[i];
      auto&c=i+2>n?arr[n-1]:arr[i+1];
      out[i]=(a+b+c)*(1.0/3.0);
    }
    arr=out;
  }
  void DoOptCurve(t_link&ref){
    ref.line=CurveResampler::resampleWithFixedSpacing(ref.line,3);
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
  void DoRndPulseKoef(vector<t_link>&arr){
    for(auto&ex:arr){
      ex.line_pulse_koef=real(rand())*0.75/RAND_MAX+0.25;
    }
  }
  void DoRndPulseKoef(t_layer_with_offset&L){
    DoRndPulseKoef(L.L.links);
  }
  void DoRndPulseKoef(){
    for(auto&ex:w.cur.layers)DoRndPulseKoef(ex);
    DoRndPulseKoef(w.cur.links);
  }
  void DoUpdate(){
    if(kb.OnDown(VK_F1)){
      for(auto&ex:w.cur.nodes){
        if(ex.ports.size())continue;
        auto r=rand()*Pi2/RAND_MAX;
        ex.ports.clear();
        auto n=rand()%4+3;
        //auto n=4;
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
      for(auto&ex:w.cur.fishs){
        DoOptLine(ex.line);
      }
    }
    if(kb.Down[VK_CONTROL]&&kb.OnDown('Q')){
      for(auto&ex:w.cur.links){
        DoOptCurve(ex);
      }
      for(auto&L:w.cur.layers)for(auto&ex:L.L.links){
        DoOptCurve(ex);
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
      DoRndPulseKoef();
    }
    if(kb.OnDown('~')){
      w.cur.links.clear();
    }
    if(kb.OnDown('~')){
      w.cur.fishs.clear();
    }
    if(kb.OnDown('`')){
      for(auto&ex:w.cur.nodes){
        if(!ex.provider)ex.color=0xff000000;
      }
    }
    if(kb.OnDown('`')){
      w.hq_links=false;
      w.hq_links=true;
      for(auto&ex:w.cur.nodes){
        ex.name.clear();
      }
    }
    if(kb.OnDown('`'))if(qap_check_id(w.layers,w.layer_id)){
      auto&b=qap_add_back(w.cur.layers);
      b.scale=0.5;
      auto&layer=w.layers[w.layer_id];
      auto&tmp=b.L;
      tmp={};
      QapPublicUberFullCloneBinMem(*pEnv,QapRawUberObject(tmp),QapRawUberObject(layer));
    }
    if(kb.OnDown('`')){
      w.cur.texts.clear();
      auto arr=split("тлен ассимиляция потомство иерарх изобретатели сражения"," ");
      for(int i=0;i<3;i++)for(auto&ex:arr){
        auto&b=qap_add_back(w.cur.texts);
        b.dir=Vec2dEx(1.75*PiD4*(rand()*2.0/RAND_MAX-1.0),1.0);b.pos=vec2d(rand()%768-768*0.5,rand()%768-768*0.5);b.text=ex;
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
    if(kb.OnDown(VK_DELETE)){
      auto*n=FindNodeAt(mpos);
      if(n)QapCleanIf(w.cur.nodes,[n](t_node&ex){return &ex==n;});
      auto*l=FindLinkAt(mpos);
      if(l)QapCleanIf(w.cur.links,[l](t_link&ex){return &ex==l;});
    }
    if(kb.OnDown(mbLeft)){
      npa=std::move(FindNP(mpos));
      points.clear();
    }
    if(kb.Down[mbLeft]){
      npb=std::move(FindNP(mpos));
      if(points.empty()||points.back()!=mpos)points.push_back(mpos);
    }
    if(kb.OnUp(mbLeft)){
      if(npa.n&&npb.n){
        auto&b=qap_add_back(w.cur.links);
        b.a.n=npa.n;
        b.a.port_id=npa.p;
        b.b.n=npb.n;
        b.b.port_id=npb.p;
        b.line=std::move(points);
      }else if(points.size()){
        auto&b=qap_add_back(w.cur.fishs);
        b.line=std::move(points);
      }
      points.clear();
    }
    if(kb.OnDown(mbRight)){
      auto&b=qap_add_back(w.cur.nodes);b.pos=mpos;b.r=new_node_r;
    }
    if(kb.OnDown(mbMiddle)){
      w.sn=FindNodeAt(mpos);
    }
    if(kb.Down(mbMiddle)){
      if(w.sn)w.sn->pos=mpos;
    }
    if(kb.OnDown(VK_LCONTROL)){
      //w.cur.texts.resize(1);
      w.st=FindTextAt(mpos);
      w.sl=FindLayerAt(mpos);
    }
    if(kb.Down(VK_LCONTROL)){
      if(w.st)w.st->pos=-(t_offcentric_scope::screen_to_world(qDev.viewport,mpos,w.st->pos,w.st->dir,1.0,false)-w.st->pos);
      if(w.sl)w.sl->offset=mpos;
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
          if(w.hq_links){
            DrawPolyLineDense(a,b,ref.line,ref.a.n->color,ref.b.n->color,ref.line_size,3,1,ref.line_pulse_koef);
          }else{
            DrawPolyLine(a,b,ref.line,ref.a.n->color,ref.b.n->color,ref.line_size);
          }
        }
      }else{
        qDev.DrawLine(a,b,ref.line_size);
      }
    }
  }
  void Draw(const t_fish&ref){
    if(!ref.enabled)return;
    if(ref.line.empty())return;
    auto c=0xffffffff;
    auto a=ref.line.front();
    auto b=ref.line.back();
    if(w.hq_links){
      DrawPolyLineDense(a,b,ref.line,c,c,ref.line_size,8,8,1);
    }else{
      DrawPolyLine(a,b,ref.line,c,c,ref.line_size,8);
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
  void Draw(const t_text&ref){
    if(!ref.enabled)return;
    auto p=ref.pos+vec2d(-12+5,+12+3);
    t_offcentric_scope scope(qDev,ref.pos,ref.dir/*.UnRot(Vec2dEx(PiD2,1.0))*/,1.0,false);
    //qDev.color=0xff000000;
    //qap_text::draw(qDev,vec2d(+1,-1).UnRot(ref.dir),ref.text,ref.font_size);
    qDev.color=0xff000000;
    //qDev.DrawCircleEx(vec2d(0,0),0,8,24,0);
    qap_text::draw(qDev,vec2d(0,0),ref.text,ref.font_size);
  }
  void Draw(const t_layer_with_offset&ref){
    t_offcentric_scope scope(qDev,-ref.offset*(1.0/ref.scale),vec2d(1,0),ref.scale,false);
    Draw(ref.L);
    qDev.color=0xff000000;
    qDev.DrawCircleEx(vec2d(0,0),0,8,24,0);
  }
  void Draw(const t_layer&ref){
    Draw(ref.layers);
    Draw(ref.nodes);
    Draw(ref.links);
    Draw(ref.fishs);
    Draw(ref.texts);
  }
  void DrawPolyLine(const vec2d&a,const vec2d&b,const vector<vec2d>&arr,QapColor bef,QapColor aft,double line_size,double pn=4){
    auto n=arr.size();
    for(int i=0;i<=n;i++){
      auto&pa=i-1<0?a:arr[i-1];
      auto&pb=i+1>n?b:arr[i+0];
      auto ls=line_size+Lerp(0.0,line_size,sin(real(i)/n*Pi*pn));
      qDev.color=0xff000000;
      qDev.DrawLine(pa,pb,ls+4);
      qDev.color=QapColor::Mix(bef,aft,i*1.0/n);
      qDev.DrawLine(pa,pb,ls);
    }
  }
  void DrawPolyLineDense(
    const vec2d& a,
    const vec2d& b,
    const vector<vec2d>& arr,
    QapColor bef,
    QapColor aft,
    double line_size,
    int k = 4, // сколько сегментов между каждой парой соседей
    double pn=8,
    double pulse_koef=1.0
  ) {
    return CurveResampler::DrawPolyLineDenseOptimized(qDev,a,b,arr,bef,aft,line_size,k,pn,pulse_koef);
    if (k <= 0) k = 1;

    // Соберём полный путь: [a] + arr + [b]
    vector<vec2d> full;
    full.reserve(arr.size() + 2);
    full.push_back(a);
    full.insert(full.end(), arr.begin(), arr.end());
    full.push_back(b);
    double len=0;
    for(int i=0;i+1<full.size();i++){
      auto&pa=full[i+0];
      auto&pb=full[i+1];
      len+=pa.dist_to(pb);
    }
    pn=len*4*pulse_koef/24;
    int N = (int)full.size() - 1;        // количество отрезков в full
    int total_segments = N * k;         // общее число сегментов после разбиения

    for (int seg = 0; seg < total_segments; seg++) {
      // Какому исходному отрезку full[i] -> full[i+1] принадлежит этот сегмент?
      int i = seg / k;
      int sub = seg % k;

      double t = sub * (1.0 / k); // 0.0 ... 0.999

      vec2d pa = Lerp(full[i], full[i + 1], t);
      vec2d pb = Lerp(full[i], full[i + 1], (sub + 1) * (1.0 / k));

      // Общий прогресс по всей линии (0..1)
      double global_t = seg * (1.0 / total_segments);
      double pulse = sin(global_t * Pi * pn); // pn полных волны по всей длине
      //double c = cos(global_t * Pi * 4);
      double ls = line_size + Lerp(0.0, line_size, 1.0 * (1.0 + pulse)); // от line_size до 2*line_size

      // Чёрная обводка
      qDev.color = 0xff000000;
      qDev.DrawLine(pa, pb, ls + 4);
      
      // Цвет с градиентом
      qDev.color = QapColor::Mix(bef, aft, global_t);
      qDev.DrawLine(pa, pb, ls);

      //qDev.color = 0xff000000;
      //qDev.DrawLine(pa, pb, std::min((1.0-pulse),0.0));
    }
  }
  template<class TYPE>
  void Draw(const vector<TYPE>&arr){for(auto&ex:arr)Draw(ex);}
  void DoDraw()override{
    QapDev::BatchScope Scope(qDev);
    qDev.color=0xff101010;
    QapClock clock;
    Draw(w.cur);
    auto ms=clock.MS();
    qDev.color=0xff101010;
    qDev.DrawPolyLine(points,t_link().line_size,false);
    if(kb.Down[mbLeft])Draw(nl);
    string s="layer_id = "+IToS(w.layer_id)+" tick = "+IToS(w.tick)+" ms="+FToS(ms);
    auto p=vec2d(-qDev.viewport.size.x*0.5+24,+552);
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
  bool has_cd(const t_link&ref,const vec2d&p){
    for(auto&ex:ref.line){
      if(p.dist_to_point_less_that_r(ex,mat_size))return true;
    }
    return ref.a.n&&ref.b.n&&Dist2Line(p,ref.a.n->pos,ref.b.n->pos)<mat_size;
  }
  t_link*FindLinkAt(const vec2d&p){
    for(auto&ex:w.cur.links)
      if(has_cd(ex,p))return &ex;
    return nullptr;
  }
  t_node*FindNodeAt(const vec2d&p){
    for(auto&ex:w.cur.nodes)
      if(ex.pos.dist_to_point_less_that_r(p,ex.r))return &ex;
    return nullptr;
  }
  t_text*FindTextAt(const vec2d&p){
    for(auto&ex:w.cur.texts){
      auto wp=t_offcentric_scope::screen_to_world(qDev.viewport,p,ex.pos,ex.dir,1.0,false);
      if(wp.SqrMag()<mat_size*mat_size*0.25)return &ex;
    }
    return nullptr;
  }
  t_layer_with_offset*FindLayerAt(const vec2d&p){
    for(auto&ex:w.cur.layers){
      //auto wp=t_offcentric_scope::screen_to_world(qDev.viewport,p,ex.pos,ex.dir,1.0,false);
      if(ex.offset.dist_to_point_less_that_r(p,mat_size))return &ex;
    }
    return nullptr;
  }
};
void main_2025_12_17(IEnvRTTI&Env){
  TGame builder;
  builder.SleepMs=0;
  builder.pEnv=&Env;
  //builder.main_clear_color=0xff010101;
  builder.viewport.size.x=GetScreenMode().W;
  builder.viewport.size.y=GetScreenMode().H;
  builder.qDev.viewport=builder.viewport;
  builder.DoNice(true);
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