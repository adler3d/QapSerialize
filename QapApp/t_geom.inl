struct t_geom{
  vector<vec2d> VA;
  vector<int> IA;
  void AddTris(int a,int b,int c)
  {
    qap_add_back(IA)=a;
    qap_add_back(IA)=b;
    qap_add_back(IA)=c;
  }
  void AddVertex(real x,real y){VA.push_back(vec2d(x,y));}
  void AddVertex(const vec2d&pos){VA.push_back(pos);}
  int AddVertexAndRetVID(const vec2d&pos){VA.push_back(pos);return VA.size()-1;}
  void add(const t_geom&geom)
  {
    QapAssert(0==geom.IA.size()%3);
    auto vpos=VA.size();
    for(int i=0;i<geom.VA.size();i++)VA.push_back(geom.VA[i]);
    for(int i=0;i<geom.IA.size();i++)IA.push_back(vpos+geom.IA[i]);
  }
  void add_with_offset(const t_geom&geom,const vec2d&pos)
  {
    auto vpos=VA.size();
    for(int i=0;i<geom.VA.size();i++)
    {
      auto&ex=geom.VA[i];
      AddVertex(pos+ex);
    }
    QapAssert(0==geom.IA.size()%3);
    auto&IA=geom.IA;
    for(int i=0;i<IA.size();i+=3)
    {
      auto&a=IA[i+0];
      auto&b=IA[i+1];
      auto&c=IA[i+2];
      AddTris(vpos+a,vpos+b,vpos+c);
    }
  }
};
static t_geom GenGeomQuad(const vec2d&pos,float w,float h){return GenGeomQuad(pos.x,pos.y,w,h);}
static t_geom GenGeomQuad(float x,float y,float w,float h)
{
  t_geom geom;
  #define F(X,Y,U,V){geom.AddVertex(x+(X)*w,y+(Y)*h);}
  vec2d O(x,y);
  F(-0.5f,-0.5f,0.0f,1.0f);
  F(+0.5f,-0.5f,1.0f,1.0f);
  F(+0.5f,+0.5f,1.0f,0.0f);
  F(-0.5f,+0.5f,0.0f,0.0f); 
  geom.AddTris(1,0,3);
  geom.AddTris(3,2,1);
  #undef F
  return geom;
}
static t_geom GenGeomCircleSolid(real r,int seg,real ang)
{
  t_geom geom;
  int n=seg;
  if(n<=0)return t_geom();
  for(int i=0;i<n;i++)
  {
    vec2d v=Vec2dEx(ang+Pi2*(real(i)/real(n)),1);
    qap_add_back(geom.VA)=v*r;
  }
  for(int i=2;i<n;i++)
  {
    geom.AddTris(0,i-1,i);
  }
  return geom;
}
static t_geom GenGeomCircleEx(real r0,real r1,int seg,real ang)
{
  t_geom geom;
  int n=seg;
  if(n<=0)return t_geom();
  geom.VA.resize(n*2);
  for(int i=0;i<n;i++)
  {
    vec2d v=Vec2dEx(ang+Pi2*(real(i)/real(n)),1);
    geom.VA[i+0]=v*r0;
    geom.VA[i+n]=v*r1;
  }
  for(int i=0;i<n;i++)
  {
    int a=0+(i+0)%n;
    int b=0+(i+1)%n;
    int c=n+(i+0)%n;
    int d=n+(i+1)%n;
    geom.AddTris(a,b,d);
    geom.AddTris(d,c,a);
  }
  return geom;
}
template<typename TYPE>
static t_geom GenGeomLineList(const vector<TYPE>&PA,const real&LineSize)
{
  t_geom geom;
  if(PA.empty())return geom;
  int Count=PA.size();
  for (int i=0;i+1<Count;i+=2)
  {
    auto&a=PA[i+0];
    auto&b=PA[i+1];
    TYPE n=vec2d(b-a).Ort().SetMag(LineSize*0.5);
    int A=geom.AddVertexAndRetVID(a+n);
    int B=geom.AddVertexAndRetVID(b-n);
    int C=geom.AddVertexAndRetVID(a-n);
    int D=geom.AddVertexAndRetVID(b+n);
    geom.AddTris(A,B,C);
    geom.AddTris(A,B,D);
  }
  return geom;
}
void DrawGeomWithOffset(t_geom&geom,const vec2d&pos)
{
  auto&qDev=*this;
  QapDev::BatchScope Scope(qDev);
  auto vpos=qDev.VPos;
  for(int i=0;i<geom.VA.size();i++)
  {
    auto&ex=geom.VA[i];
    //auto&v=qDev.AddVertexRaw();
    //v.get_pos()=pos+ex;
    //v.color=qDev.color;
    qDev.AddVertex(pos+ex,qDev.color,0,0);
  }
  QapAssert(0==geom.IA.size()%3);
  auto&IA=geom.IA;
  for(int i=0;i<IA.size();i+=3)
  {
    auto&a=IA[i+0];
    auto&b=IA[i+1];
    auto&c=IA[i+2];
    qDev.AddTris(vpos+a,vpos+b,vpos+c);
  }
}