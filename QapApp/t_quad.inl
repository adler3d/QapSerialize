struct t_quad{
  vec2d pos;
  vec2d size;
  t_quad&set(const vec2d&p,const vec2d&s){pos=p;size=s;return *this;}
  vec2d get_vertex_by_dir(const vec2d&dir)const{return pos+vec2d::sign(dir).Mul(size)*0.5;}
  vec2d get_left_top()const{return pos+vec2d(-1,+1).Mul(size)*0.5;}
  void set_pos_by_vertex_and_dir(const vec2d&vertex,const vec2d&dir)
  {
    auto d=vec2d::sign(dir);
    //QapAssert(d.x&&d.y);
    pos=vertex+d.Mul(size)*0.5;
  }
  t_quad add_size(real ds)const{t_quad q=*this;q.size+=vec2d(ds,ds);return q;}
  t_quad add_size(vec2d size)const{t_quad q=*this;q.size+=size;return q;}
  t_quad add_pos(const vec2d&pos)const{t_quad q=*this;q.pos+=pos;return q;}
  vec2d clamp(const vec2d&point)const
  {
    return clamp(point-pos,size)+pos;
  }
  static vec2d clamp(const vec2d&p,const vec2d&size)
  {
    return vec2d(Clamp<real>(p.x,-size.x*0.5,+size.x*0.5),Clamp<real>(p.y,-size.y*0.5,+size.y*0.5));
  }
};

static bool point_in_quad(t_quad q, vec2d p)
{
  auto s=q.size*0.5;
  auto o=p-q.pos;
  return abs(s.x)>=abs(o.x)&&abs(s.y)>=abs(o.y);
}

static vec2d join_quads_by_y(const vector<t_quad>&arr)
{
  real total_y=0; real max_x=0;
  for(int i=0;i<arr.size();i++){auto&s=arr[i].size;total_y+=s.y;max_x=std::max<real>(max_x,s.x);}
  return vec2d(max_x,total_y);
}
static vec2d join_quads_by_x(const vector<t_quad>&arr)
{
  real total_x=0; real max_y=0;
  for(int i=0;i<arr.size();i++){auto&s=arr[i].size;total_x+=s.x;max_y=std::max<real>(max_y,s.y);}
  return vec2d(total_x,max_y);
}
static vec2d join_quads_by_dir(const vector<t_quad>&arr,char dir='x')
{
  if(dir=='x')return join_quads_by_x(arr);
  if(dir=='y')return join_quads_by_y(arr);
  QapNoWay();
  return vec2d_zero;
}
static vec2d join_quads_by_y_ex(const vector<t_quad>&arr,real space){return join_quads_by_y(arr)+vec2d(0,space*std::max<int>(0,int(arr.size())-1));}
static vec2d join_quads_by_x_ex(const vector<t_quad>&arr,real space){return join_quads_by_x(arr)+vec2d(space*std::max<int>(0,int(arr.size())-1),0);}

static vec2d join_quads_by_dir_ex(const vector<t_quad>&arr,char dir='x',real space=0)
{
  if(dir=='x')return join_quads_by_x_ex(arr,space);
  if(dir=='y')return join_quads_by_y_ex(arr,space);
  QapNoWay();
  return vec2d_zero;
}
static void layout_by_dir(vector<t_quad>&arr,char dir='x',real space=0)
{
  QapAssert(dir=='x'||dir=='y');
  t_quad q;
  q.size=join_quads_by_dir_ex(arr,dir,space);
  vec2d vdir=dir=='x'?vec2i(+1,0):vec2d(0,-1);
  auto p=q.get_vertex_by_dir(-vdir);
  for(int i=0;i<arr.size();i++)
  {
    auto&ex=arr[i];
    ex.set_pos_by_vertex_and_dir(p,vdir);
    auto dp=dir=='x'?vec2d(space+ex.size.x,0):vec2d(0,-space-ex.size.y);
    p+=dp;
  }
}
static vector<t_quad> split_quad_by_dir(t_quad q,char dir='x',int n=2)
{
  vector<t_quad> arr;
  vec2d dv=vec2d(dir=='x'?1:0,dir=='y'?1:0);
  vec2d dn=vec2d(dv.y,dv.x);
  auto d=dv*(1.0/n)+dn;
  auto size=q.size.Mul(d);
  for(int i=0;i<n;i++)qap_add_back(arr).size=size;
  layout_by_dir(arr,dir,0.0);
  return arr;
}