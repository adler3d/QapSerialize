#define DEF_PRO_SORT_BY_FIELD(sort_by_field,TYPE,FIELD)\
  struct t_help_struct_for_sort_vec_of_##TYPE##_by_##FIELD{\
    static int __cdecl cmp_func(const void*a,const void*b){return cmp(*(TYPE*)a,*(TYPE*)b);}\
    static int cmp(const TYPE&a,const TYPE&b){return a.FIELD-b.FIELD;}\
  };\
  void sort_by_field(vector<TYPE>&arr){\
    if(arr.empty())return;\
    std::qsort(&arr[0],arr.size(),sizeof(t_id_with_dist),t_help_struct_for_sort_vec_of_##TYPE##_by_##FIELD::cmp_func);\
  }
#define PRO_FUNCGEN_GETP_BY_FIELD_V2(rettype,getp,arr,field_type,field,FAST)\
  rettype*getp(field_type value)\
  {\
    rettype*p=nullptr;\
    for(int i=0;i<arr.size();i++){\
      auto&ex=arr[i];\
      if(ex.field!=value)continue;\
      if(FAST)return &ex;QapAssert(!p);\
      p=&ex;\
    }\
    return p;\
  }\
  const rettype*getp(field_type value)const\
  {\
    const rettype*p=nullptr;\
    for(int i=0;i<arr.size();i++){\
      auto&ex=arr[i];\
      if(ex.field!=value)continue;\
      if(FAST)return &ex;QapAssert(!p);\
      p=&ex;\
    }\
    return p;\
  }

#define PRO_FUNCGEN_ADD_UNIQUE_OBJ_BY_FIELD(rettype,adduni,arr,field_type,field)\
  rettype*adduni(field_type value)\
  {\
    rettype*p=nullptr;\
    for(int i=0;i<arr.size();i++){\
      auto&ex=arr[i];\
      if(ex.field!=value)continue;\
      QapAssert(!p);\
      p=&ex;\
    }\
    if(!p)p=&qap_add_back(arr);\
    return p;\
  }

class TQapGameV2:public TQapGame{
public:
#define DEF_PRO_VARIABLE(ADD)\
ADD(bool,drawpass,false)\
ADD(bool,movepass,false)\
ADD(bool,srvpass,false)\
ADD(bool,cntpass,false)\
ADD(t_quad&,viewport,qDev.viewport)\
ADD(vec2d&,consize,viewport.size)\
//===
#include "defprovar.inl"
//===
public:
  TQapGameV2():viewport(qDev.viewport),consize(viewport.size){DoReset();}
public:
  virtual void DoPreDraw(){}
  virtual void DoPreMove(){}
  void DoDraw(){qDev.auto_clamp=true;viewport.pos=vec2d(0,0);consize=win.GetClientSize();drawpass=true;DoPreDraw();soUpdate();drawpass=false;}
  void DoMove(){qDev.auto_clamp=true;viewport.pos=vec2d(0,0);consize=win.GetClientSize();movepass=true;DoPreMove();soUpdate();movepass=false;}
  virtual void soUpdate(){}
  void DrawDownedKeys()
  {
    vector<QapColor> c;
    vector<string> arr;
    auto SysDo=[&](const char*key,const QapColor&color)->bool{qap_add_back(arr)=key;qap_add_back(c)=color;qDev.SetColor(color);return true;};
    #define F(COLOR,STRKEY,KEY)if(kb.Down[KEY])if(SysDo(#STRKEY,COLOR))
    F(0x30FF0000,mbLeft,mbLeft){qDev.DrawCircle(mpos,16,0,4,64);}
    F(0x300000FF,mbRight,mbRight){qDev.DrawCircle(mpos,14,0,4,64);}
    F(0x30008000,CONTROL,VK_CONTROL){qDev.DrawCircle(mpos,12,0,4,64);}
    F(0x30707000,ALT,VK_MENU){qDev.DrawCircle(mpos,18,0,4,64);}
    F(0x30007000,SHIFT,VK_SHIFT){qDev.DrawCircle(mpos,16,0,6,3);}
    F(0x40FF0000,DEL,VK_DELETE){DrawCross(mpos,32,4,0);}
    #define Z(NUM)F(0x30000000,F##NUM,VK_F##NUM);
    Z(1)Z(2)Z(3)Z(4)Z(5)Z(6)Z(7)Z(8)Z(9)Z(10)Z(11)Z(12)
    #undef Z
    #define Z(KEY)F(0x30000000,KEY,#KEY[0]);
    Z(A)Z(B)Z(C)Z(D)Z(E)Z(F)Z(G)Z(H)Z(I)Z(J)Z(K)Z(L)Z(M)Z(N)Z(O)Z(P)Z(Q)Z(R)Z(S)Z(T)Z(U)Z(V)Z(W)Z(X)Z(Y)Z(Z)
    Z(1)Z(2)Z(3)Z(4)Z(5)Z(6)Z(7)Z(8)Z(9)Z(0)
    #undef Z
    #define Z(KEY)F(0x30000000,NUM_##KEY,VK_NUMPAD##KEY);
    Z(1)Z(2)Z(3)Z(4)Z(5)Z(6)Z(7)Z(8)Z(9)Z(0)
    #undef Z
    F(0x30000000,DIV,VK_DIVIDE);
    F(0x30000000,MUL,VK_MULTIPLY);
    F(0x30000000,ADD,VK_ADD);
    F(0x30000000,SUB,VK_SUBTRACT);
    F(0x30000000,BACK,VK_BACK);
    F(0x30000000,SPACE,VK_SPACE);
    F(0x30000000,ENTER,VK_RETURN);
    F(0x30000000,LEFT,VK_LEFT);
    F(0x30000000,RIGHT,VK_RIGHT);
    F(0x30000000,UP,VK_UP);
    F(0x30000000,DOWN,VK_DOWN);
    #undef F
    vec2d p=mpos+vec2d(16,-16);auto dy=vec2d(0,-16);
    for(int i=0;i<arr.size();i++){qDev.SetColor(c[i]);qap_text::draw(qDev,p,arr[i],16);p+=dy;}
  }
public:
  void DrawCross(const vec2d&point,real w,real h,real ang){qDev.DrawQuad(mpos,w,h,+PiD4);qDev.DrawQuad(mpos,w,h,-PiD4);}
  static void DrawQuadWithBorder(QapDev&qDev,t_quad q)
  {
    qDev.DrawQuad(q.pos,q.size);
    DrawQuadBorder(qDev,q);
  }
  static void DrawQuadBorder(QapDev&qDev,t_quad q){qDev.color=QapColor::HalfMix(0xff000000,qDev.color);qDev.DrawRectAsQuad(q.pos,q.size,2);}
  void DrawQuadWithBorder(t_quad q){DrawQuadWithBorder(qDev,q);}
  void DrawQuadBorder(t_quad q){DrawQuadBorder(qDev,q);}
  static void DrawQuad(QapDev&qDev,t_quad q){qDev.DrawQuad(q.pos,q.size);}
  static void qap_text_draw_at_x_center(QapDev&qDev,const vec2d&pos,const string&msg,int textsize)
  {
    t_quad q;
    q.size=qap_text::get_size(qDev,msg,textsize);
    q.pos=pos;
    qap_text::draw(qDev,q.get_left_top(),msg,textsize);
  }
public:
  static real rndreal(){return real(rand())/RAND_MAX;}
  static real rndreal(real from,real to){return from+rndreal()*(to-from);}
  static vec2d rndvec2d(){return vec2d(rndreal(),rndreal());}
  static vec2d rndvec2d(real from,real to){return vec2d(rndreal(from,to),rndreal(from,to));}
};