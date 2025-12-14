#include "StdAfx.h"
//-------------------------------------------//
#include "MetaTypeGraber.inl"
#include "NewBinarySaver.inl"
#include "TSimpleBinarySaver.inl"
//-------------------------------------------//

class TDynVector{
public:
  vector<char> arr;
public:
  size_t size(TTypeVector*pvec)const
  {
    auto raw_size=arr.size();
    auto elem_size=pvec->ElementType->Info.Size;
    QapAssert(elem_size>0);
    auto size=arr.size()/elem_size;
    return size;
  }
  void resize(TTypeVector*pvec,size_t new_size)
  {
    auto raw_size=arr.size();
    auto*pet=pvec->ElementType.get();
    auto elem_size=pet->Info.Size;
    QapAssert(elem_size>0);
    auto size=arr.size()/elem_size;
    auto raw_new_size=new_size*elem_size;
    if(raw_new_size==raw_size)return;
    auto*pf=pet->Factory.get();
    if(raw_new_size<raw_size)
    {
      for(int i=new_size;i<size;i++)
      {
        TMemory mem;
        mem.ptr=(TMemory::ptr_t*)&arr[i*elem_size];
        mem.size=elem_size;
        pf->UnsafeFree(mem);
      }
      arr.resize(new_size);
      return;
    }
    if(arr.capacity()>raw_new_size)
    {
      arr.resize(raw_new_size);
      for(int i=size;i<new_size;i++)
      {
        TMemory mem;
        mem.ptr=(TMemory::ptr_t*)&arr[i*elem_size];
        mem.size=elem_size;
        pf->UnsafeInit(mem);
      }
      return;
    }
    vector<char> new_arr;
    new_arr.resize(raw_new_size);
    for(int i=0;i<new_size;i++)
    {
      TMemory dest;
      dest.ptr=(TMemory::ptr_t*)&new_arr[i*elem_size];
      dest.size=elem_size;
      pf->UnsafeInit(dest);
      if(i>=size)continue;
      TMemory source;
      source.ptr=(TMemory::ptr_t*)&arr[i*elem_size];
      source.size=elem_size;
      pf->UnsafeMove(dest,source);
    }
    arr=std::move(new_arr);
  }
  char&at(TTypeVector*pvec,size_t index)
  {
    auto raw_size=arr.size();
    auto elem_size=pvec->ElementType->Info.Size;
    QapAssert(elem_size>0);
    QapAssert(index*elem_size<raw_size);
    return arr[index*elem_size];
  }
};

class TDynVectorAPI:public IVectorAPI{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(TDynVectorAPI)PARENT(IVectorAPI)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(THardPtr<TTypeVector>,pType,DEF,$,$)\
ADDEND()
//=====+>>>>>TDynVectorAPI
#include "QapGenStruct.inl"
//<<<<<+=====TDynVectorAPI
public:
  virtual SizeType GetCount(void*p)
  {
    auto&v=*(TDynVector*)p;return v.size(pType.get());
  }
  virtual void SetCount(void*p,SizeType c)
  {
    auto&v=*(TDynVector*)p;v.resize(pType.get(),c);
  }
  virtual void*GetItem(void*p,SizeType i)
  {
    auto&v=*(TDynVector*)p;return &v.at(pType.get(),i);
  }
public:
  TType*GetElementType(IEnvRTTI&Env)
  {
    QapAssert(&Env==pType->Env.get());
    return pType->ElementType.get();
  }
  TType*GetMetaType(IEnvRTTI&Env)
  {
    QapDebugMsg("What does this code mean?");
    return (TType*)Sys$$<SelfClass>::GetRTTI(Env);
  };
public:
  //static SelfClass*Get(){
  //  static SelfClass tmp;
  //  static bool flag=true;
  //  if(flag){
  //    TVectorAPI<void>::GetArr().push_back(&tmp);
  //    flag=false;
  //  }
  //  return &tmp;
  //}
};

class TDynVectorFactory:public IFactory{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(TDynVectorFactory)PARENT(IFactory)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(THardPtr<TTypeVector>,pType,DEF,$,$)\
ADDVAR(TDynVectorAPI,VecAPI,DEF,$,$)\
ADDEND()
//=====+>>>>>TDynVectorFactory
#include "QapGenStruct.inl"
//<<<<<+=====TDynVectorFactory
public:
  typedef TDynVector ptr_t;
public:
  static void Create(ptr_t*ptr){new(ptr) ptr_t;detail::FieldTryDoReset(*ptr);}
  static void Destroy(ptr_t*ptr){ptr->~ptr_t();}
public:
  void Init(IAllocator*pAlloc,TMemory&Mem){
    Mem.size=GetSize();
    pAlloc->Alloc(Mem);
    Create(get(Mem.ptr));
  }
  void Free(IAllocator*pAlloc,TMemory&Mem){
    Destroy(get(Mem.ptr));
    pAlloc->Dealloc(Mem);
    Mem.size=0;
  }
public:
  template<class TYPE>
  int weak_GetSize()
  {
    return sizeof(ptr_t);
  }
  int GetSize()
  {
    return weak_GetSize<void>();
  }
  void UnsafeInit(const TMemory&Mem){
    QapAssert(Mem.ptr);
    QapAssert(Mem.size==GetSize());
    Create(get(Mem.ptr));
  }
  void UnsafeFree(const TMemory&Mem){
    QapAssert(Mem.ptr);
    QapAssert(Mem.size==GetSize());
    Destroy(get(Mem.ptr));
  }
  void UnsafeMove(const TMemory&Dest,const TMemory&Source){
    QapAssert(Dest.ptr);
    QapAssert(Dest.size==GetSize());
    QapAssert(Source.ptr);
    QapAssert(Source.size==GetSize());
    auto*pd=get(Dest.ptr);
    auto*ps=get(Source.ptr);
    *pd=std::move(*ps);
  }
public:
  void unlink(){pType=nullptr;};
public:
  template<class QAP_RTTI>
  TType*weak_GetElementType(QAP_RTTI&Env)
  {
    QapAssert(pType);
    auto*ptype=pType.get();
    if(!ptype)return nullptr;
    QapAssert(&Env==ptype->Env.get());
    return ptype;
  }
  TType*GetElementType(IEnvRTTI&Env){
    return weak_GetElementType(Env);
  }
  TType*GetMetaType(IEnvRTTI&Env){return (TType*)Sys$$<SelfClass>::GetRTTI(Env);};
public:
  static inline ptr_t*get(TMemory::ptr_t*ptr){return (ptr_t*)(void*)ptr;}
};

class TQapBinaryObjectLastHope{
public:
#define DEF_PRO_STRUCT_INFO(NAME,PARENT,OWNER)NAME(TQapBinaryObjectLastHope)
#define DEF_PRO_VARIABLE(ADDBEG,ADDVAR,ADDEND)\
ADDBEG()\
ADDVAR(int,SpaceSize,SET,0,$)\
ADDVAR(vector<EnvDefTypeStr>,MetaSpec,DEF,$,$)\
ADDVAR(FullDeclareEnvRTTI,FullDeclEnv,DEF,$,$)\
ADDVAR(THardPtr<TType>,RootType,DEF,$,$)\
ADDEND()
//=====+>>>>>TQapBinaryObjectLastHope
#include "QapGenStruct.inl"
//<<<<<+=====TQapBinaryObjectLastHope
public:
  typedef vector<EnvDefTypeStr> TMetaSpec;
  struct TRawPtrs
  {
    TRawPtr SpaceSize;
    TRawPtr MetaSpec;
    TRawPtr FullDeclEnv;
    TRawPtr RootType;
    TRawPtr Root;
  public:
    void Init(IEnvRTTI&Env,TQapBinaryObjectLastHope&qbo)
    {
      #define F(FIELD)this->FIELD.set(Env,qbo.FIELD);
      F(SpaceSize);F(MetaSpec);F(FullDeclEnv);F(RootType);
      #undef F
      Root.setZero();
    }
  };
};

struct TConservationMachineLastHope;

struct IUberLoadDeviceLastHope
{
  typedef TConservationMachineLastHope TMachine;
  virtual bool DoLoad(TMachine&machine,TRawPtr&raw_ptr){return false;}
  virtual bool Load(TMachine&machine,TRawPtr&raw_ptr){return false;}
};

struct TConservationMachineLastHope 
{
  typedef TServerPtrSpace TSpace;
  typedef TQapBinaryObjectLastHope::TRawPtrs TRawPtrs;
  TSpace Space;
  TSpaceDependsTools Tools;
  TClientPtrVcbCounter ClientCounter;
  TClientPtrVisitor ClientVisitor;
  DependsSolver DS;
  TQapBinaryObjectLastHope&qbo;
  TRawPtrs ptrs;
  vector<int> last_hope;
  vector<string> last_hope_str;
  vector<EnvType> old_types;
  TConservationMachineLastHope(IEnvRTTI&Env,TQapBinaryObjectLastHope&qbo):
    Tools(Env,Space),ClientCounter(Env,Space),ClientVisitor(ClientCounter),DS(Env,ClientVisitor),qbo(qbo)
  {
    ptrs.Init(Env,qbo);
    Space.AddPointsFromSys();
    Space.AddPointsFromEnv(Env);
    Tools.BuildProxy();
    Tools.PrepareEx();
    Tools.MegaPass();
  }
  ~TConservationMachineLastHope()
  {
    Space.clear();
    for(int i=0;i<old_types.size();i++)
    {
      auto&ex=old_types[i];
      if(auto*ptr=TTypeStruct::UberCast(ex.Type.get()))
      {
        ptr->SubType=nullptr;
        ptr->OwnType=nullptr;
        for(int i=0;i<ptr->Members.size();i++)ptr->Members[i].Type=nullptr;
        ptr->Factory=nullptr;
        ptr->Env=nullptr;
        auto*pf=(TTypeStruct::TDynamicFactory*)ex.Factory.get();
        pf->pType=nullptr;
      }
      if(auto*ptr=TTypeVector::UberCast(ex.Type.get()))
      {
        ptr->ElementType=nullptr;
        ptr->SizeType=nullptr;
        ptr->VecAPI=nullptr;
        ptr->Factory=nullptr;
        ptr->Env=nullptr;
        auto*pf=(TDynVectorFactory*)ex.Factory.get();
        //auto*pf=(TTypeVector::TDynamicFactory*)ex.Factory.get();
        pf->pType=nullptr;
        pf->VecAPI.pType=nullptr;
      }
    }
  }
  static void Check_EachItemsOfTheSpaceIsNoPassed(TServerPtrSpace&Space)
  {
    vector<int> bad_items;
    auto&arr=Space.Arr;
    if(arr.empty())return;
    arr[0].CheckNullptr();
    for(int i=1;i<arr.size();i++)
    {
      auto&item=arr[i];
      if(item.passed)
      {
        string itemname=item.name.empty()?"{id="+IToS(i)+"}":item.name;
        QapDebugMsg("bad item -'"+itemname+"'");
        bad_items.push_back(i);
      }
    }
    if(bad_items.empty())return;
    QapDebugMsg("bad_items.size() == "+IToS(bad_items.size()));
  }
  struct TPointsHolder
  {
    vector<EnvItem> Arr;
    void move(TSpace&From,TSpace&To)
    {
      QapAssert(From.size()==To.size());
      for(int i=0;i<Arr.size();i++)
      {
        auto&ex=Arr[i];
        auto&SaveID=ex.ptr.ptr->SaveID;
        QapAssert(SaveID);
        QapAssert(From.CheckServSaveID(ex.ptr.ptr));
        QapAssert(To.IsAllowed(SaveID));
        auto&from=From[SaveID];
        auto&to=To[SaveID];
        QapAssert(from.id==to.id);
        QapAssert(to.ptr.IsNull()&&to.ptr_lnk.IsNull());
        From.Del(from);
        To.InsertTo(to.id,ex);
      }
      bool debug_flag=false;
      if(debug_flag)Arr.clear();
    }
    void on(TSpace&Space)const
    {
      for(int i=0;i<Arr.size();i++)
      {
        auto&ex=Arr[i];
        QapAssert(ex.ptr.IsValid());
        auto&SaveID=ex.ptr.ptr->SaveID;
        QapAssert(!SaveID);
        QapAssert(Space.IsAllowed(ex.id));
        auto&item=Space[ex.id];
        QapAssert(item.ptr.IsNull()&&item.ptr_lnk.IsNull());
        Space.InsertTo(ex.id,ex);
        QapAssert(SaveID);
      }
    }
    void off_all_objects(TSpace&Space)const
    {
      //set object.SaveID to zero
      for(int i=0;i<Arr.size();i++)
      {
        auto&ex=Arr[i];
        QapAssert(ex.ptr.IsValid());
        auto&SaveID=ex.ptr.ptr->SaveID;
        QapAssert(SaveID);
        QapAssert(Space.CheckServSaveID(ex.ptr.ptr));
        auto&item=Space[SaveID];
        EnvItem temp;
        temp.copy(item);
        QapAssert(item.id==ex.id);
        Space.Del(item);
        QapAssert(!SaveID);
        item.copy(temp);
      }
    }
    void off_all_envitems(TSpace&Space,bool assert_objects_enabled=false)const
    {
      //set envitems.ptr to zero
      for(int i=0;i<Arr.size();i++)
      {
        auto&ex=Arr[i];
        QapAssert(ex.ptr.IsValid());
        auto&SaveID=ex.ptr.ptr->SaveID;
        QapAssert(assert_objects_enabled==bool(SaveID));
        QapAssert(Space.IsAllowed(ex.id));
        auto&item=Space[ex.id];
        QapAssert(item.ptr.IsValid()&&item.ptr_lnk.IsValid());
        item.ptr.set_zero();
        item.ptr_lnk.set_zero();
      }
    }
    void off(TSpace&Space)const
    {
      for(int i=0;i<Arr.size();i++)
      {
        auto&ex=Arr[i];
        QapAssert(ex.ptr.IsValid());
        auto&SaveID=ex.ptr.ptr->SaveID;
        QapAssert(SaveID);
        QapAssert(Space.CheckServSaveID(ex.ptr.ptr));
        auto&item=Space[SaveID];
        QapAssert(item.id==ex.id);
        Space.Del(item);
      }
    }
    void insert_to_end(TSpace&Space)const
    {
      for(int i=0;i<Arr.size();i++)
      {
        auto&ex=Arr[i];
        QapAssert(ex.ptr.IsValid());
        auto&SaveID=ex.ptr.ptr->SaveID;
        QapAssert(!SaveID);
        Space.Insert(ex);
      }
    }
  };
  struct TUserPointsHolder:TPointsHolder
  {
    void take(TSpace&Space)
    {
      auto&arr=Space.Arr;
      if(arr.empty())return;
      arr[0].CheckNullptr();
      for(int i=1;i<arr.size();i++)
      {
        auto&item=arr[i];
        if(item.passed)
        {
          int id=Arr.size();
          this->Arr.push_back(EnvItem());
          auto&back=this->Arr.back();
          back.copy(item);
          QapAssert(back.name.empty());
          back.name="userpoint["+IToS(id)+"]";
        }
      }
    }
  };
  struct TSyncPointsHolder:TPointsHolder
  {
    typedef TSyncPointsHolder SelfClass;
    void sync(TSpace&Space)
    {
      for(int i=0;i<Arr.size();i++)
      {
        auto&ex=Arr[i];
        auto&item=Space[ex.ptr.ptr->SaveID];
        Space.Swap(ex.id,item.id);
      }
    }
    void copy(const TPointsHolder&ref)
    {
      this->Arr.resize(ref.Arr.size());
      for(int i=0;i<ref.Arr.size();i++)
      {
        Arr[i].copy(ref.Arr[i]);
      }
    }
    void copy_envitem_lnk(const TPointsHolder&ref,TSpace&Space)
    {
      this->Arr.resize(ref.Arr.size());
      for(int i=0;i<ref.Arr.size();i++)
      {
        auto&ex=ref.Arr[i];
        auto&ax=ex.get_lnk(Space);
        Arr[i].copy(ax);
      }
    }
    void copy_envitem_lnk_and_keep_id(const TPointsHolder&ref,TSpace&Space)
    {
      this->Arr.resize(ref.Arr.size());
      for(int i=0;i<ref.Arr.size();i++)
      {
        auto&ex=ref.Arr[i];
        auto&ax=ex.get_lnk(Space);
        Arr[i].copy(ax).id=ex.id;
      }
    }
    void update_id(const TSpace&Space)
    {
      auto&arr=Arr;
      for(int i=0;i<arr.size();i++)
      {
        auto&ex=arr[i];
        QapAssert(ex.ptr.IsValid());
        QapAssert(Space.CheckServSaveID(ex.ptr.ptr));
        auto&SaveID=ex.ptr.ptr->SaveID;
        auto&item=Space[SaveID];
        QapAssert(item.oper_eq(ex,true));
        ex.id=SaveID;
      }
    }
    void insert_and_sync(TSpace&Space,TSpace&SpaceForIt)
    {
      auto&UpA=*this;
      UpA.off(Space);
      UpA.insert_to_end(SpaceForIt);
      SelfClass UpB;
      UpB.copy(UpA);
      UpB.update_id(SpaceForIt);
      UpB.off_all_objects(SpaceForIt);
      UpA.on(Space);
      UpB.sync(Space);
    }
  };
  static int get_lnk_id(TSpace&Space,TType*pType)
  {
    auto&item=Space[pType->Self];
    auto*p=(DeclareType*)item.ptr_lnk.get();
    return p->Self.P.SaveID;
  };
  /*
    out.MiniSpace - туда складыватся используемые(по версии Space) метатипы из arr, а потом они(используемые) привзяываются к SpaceForMetaSpec.
    TakeUsedMetatypesAndLinkHimWithSpace == InitMetaSpec
  */
  static void InitMetaSpec(TQapBinaryObjectLastHope::TMetaSpec&out,TSpaceDependsTools::TDeclareTypeArray&arr,TSpace&Space,TSpace&SpaceForMetaSpec)
  {
    auto&SpaceFMS=SpaceForMetaSpec;
    {
      int count=arr.size();
      out.reserve(count);
      SpaceFMS.reserve(1+count+count);
    }
    auto get=[&Space](TType*pType)->int
    {
      return get_lnk_id(Space,pType);
    };
    auto copy_count_from_lnk_for_cur=[&Space,&get](TType*pType)
    {
      auto&cur_item=Space[pType->Self];
      auto&lnk_item=Space[get(pType)];
      QapAssert(!cur_item.count);
      if(cur_item.count==lnk_item.count)return;
      cur_item.count=lnk_item.count;
    };
    auto pass=[&Space,&SpaceFMS,&get,&out](TType*pType)
    {
      auto&sys_item=Space[pType->Self];
      auto&def_item=Space[get(pType)];
      if(!def_item.count)return;
      out.push_back(EnvDefTypeStr());
      auto&back=out.back();
      back.Decl.Name=pType->GetFullName();
      QapAssert(sys_item.name.substr(0,5)=="Sys$$");
      QapAssert(def_item.name.substr(0,5)=="Def$$");
      SpaceFMS.AddExt(EnvAdapter::get(back.Decl.Decl.Self),nullptr,"Def$$<"+back.Decl.Name+">::GetRTTI()").ptr_lnk=def_item.ptr;
      SpaceFMS.AddExt(EnvAdapter::get(back.Impl.Self),nullptr,"Sys$$<"+back.Decl.Name+">::GetRTTI()").ptr_lnk=sys_item.ptr;
    };
    for(int i=0;i<arr.size();i++)
    {
      auto&ex=arr[i];
      copy_count_from_lnk_for_cur(ex.first);
      copy_count_from_lnk_for_cur(ex.second);
    }
    for(int i=0;i<arr.size();i++)
    {
      auto&ex=arr[i];
      pass(ex.first);
    }
    for(int i=0;i<arr.size();i++)
    {
      auto&ex=arr[i];
      pass(ex.second);
    }
  }
  static void SyncTwoSpace_Stage1(TQapBinaryObjectLastHope::TMetaSpec&Spec,TSpace&Space,TSpace&SpaceFMS)
  {
    auto do_swap=[&Space,&SpaceFMS](EnvDefEntryPoint&EP)
    {
      auto&item=SpaceFMS[EP.Self];
      auto&jtem=Space[item.ptr_lnk.ptr->SaveID];
      QapAssert(jtem.name==item.name);
      Space.Swap(jtem.id,item.id);
    };
    auto&arr=Spec;
    for(int i=0;i<arr.size();i++)
    {
      auto&ex=arr[i];
      do_swap(ex.Decl.Decl);
      do_swap(ex.Impl);
    }
  }
public:
  TType*FindTypeByName(const string&fn)
  {
    auto*pmt=Tools.Solver.Env.FindTypeByName_is_ug_but_work(fn);
    if(pmt)return pmt;
    auto&arr=old_types;
    for(int i=0;i<arr.size();i++)
    {
      auto&ex=arr[i];
      QapAssert(ex.Type);
      if(ex.Type->GetFullName()==fn)return ex.Type.get();
    }
    return nullptr;
  }
public:
  struct SpaceScopeBotLastHope
  {    
    typedef TConservationMachineLastHope TMachine;
    TMachine&machine;
    TSpace&Space;
    SpaceScopeBotLastHope(TMachine&machine,TSpace&Space):machine(machine),Space(Space){Init();}
   ~SpaceScopeBotLastHope(){Free();}
    void Init()
    {
      std::swap(machine.Space,Space);
    }
    static void CheckAssert(TMachine&machine){QapAssert(&machine.qbo.SpaceSize==machine.ptrs.SpaceSize.pValue);}
    static void Go(TMachine&machine,IUberLoadDeviceLastHope&dev,TRawPtr&raw_ptr)
    {
      CheckAssert(machine);
      dev.DoLoad(machine,machine.ptrs.SpaceSize);
      machine.Space.resize(machine.qbo.SpaceSize);
      dev.DoLoad(machine,raw_ptr);
    }
    void Free()
    {
      std::swap(machine.Space,Space);
    }
  };
  //------------------------------------------- GP_BOTS ---------------------------------------------
  class GP_BotA_LT;
  class GP_BotB_LT;
  class GP_BotC_LT;
  class GP_BotD_LT;

  typedef TConservationMachineLastHope SelfClass;
  typedef SelfClass::TRawPtr TRawPtr;
  typedef SelfClass::TSyncPointsHolder TSyncPointsHolder;
  typedef SelfClass::TUserPointsHolder TUserPointsHolder;
  typedef SelfClass::TPointsHolder TPointsHolder;
  typedef TServerPtrSpace TSpace;

  class GP_Bot{
  public:
    static void copy_envitem_lnk_to_envitem_id(TSpace&Dest,TSpace&Source,TSpace&LnkSource)
    {
      QapAssert(Dest.size()<=Source.size());
      auto&out_space=Dest;
      out_space.resize(Source.size());
      auto&arr=Source.Arr;
      for(int i=1;i<arr.size();i++)
      {
        auto&ex=arr[i];
        auto&item=ex.get_lnk(LnkSource);
        QapAssert(out_space.IsAllowed(item.id));
        out_space[item.id].copy(item);
      }
    }
  };

  //=====
  //===============
  //=====

  class GP_BotContext_LT{
  public:
    typedef GP_BotContext_LT SelfClass;
    typedef GP_BotA_LT GP_BotA;
    typedef GP_BotB_LT GP_BotB;
    typedef GP_BotC_LT GP_BotC;
    typedef GP_BotD_LT GP_BotD;
  public:
  public:
    TConservationMachineLastHope&machine;
    TRawPtr&raw_ptr;
  public:
    TSpace SpaceForMetaSpec;
    TSpace SpaceForFullDeclEnv;
    TSpace SpaceForRoot;
  public:
    GP_BotContext_LT(TConservationMachineLastHope&machine,TRawPtr&raw_ptr):machine(machine),raw_ptr(raw_ptr){Init();}
    ~GP_BotContext_LT(){Free();}
  public:
    bool result;
    TPointsHolder BuffForRoot;
  public:
    void Init()
    {
      result=true;
    }
  public:
    void Free()
    {
      auto&qbo=machine.qbo;
      SpaceForMetaSpec.hack_clear();
      {
        auto&arr=qbo.FullDeclEnv.ImplPtrs;
        for(int i=0;i<arr.size();i++)
        {
          auto&ex=arr[i];
          auto&decl=SpaceForFullDeclEnv[ex.Decl->Self];
          auto&impl=SpaceForFullDeclEnv[ex.Impl.Self];
          SpaceForFullDeclEnv.Del(decl,true);
          SpaceForFullDeclEnv.Del(impl,true);
        }
      }
      auto&Space=this->machine.Space;
      Space.clear();
      SpaceForFullDeclEnv.Arr.clear();
      BuffForRoot.off_all_envitems(SpaceForRoot);
      SpaceForRoot.clear();
    }
  };

  class GP_BotA_LT{
  public:
    typedef GP_BotA_LT SelfClass;
    typedef GP_BotContext_LT TContext;
  public:
    TContext&Context;
  public:
    GP_BotA_LT(TContext&Context):Context(Context){Init();}
   ~GP_BotA_LT(){Free();}
  public:
   TSpace&get_space(){return Context.SpaceForMetaSpec;}
  public:
    void Init()
    {
      //nothing
    }
    void Free()
    {
      auto&result=this->Context.result;
      auto&Space=this->Context.machine.Space;
      auto&SpaceForMetaSpec=this->Context.SpaceForMetaSpec;
      auto&SpaceForFullDeclEnv=this->Context.SpaceForFullDeclEnv;
      auto&SpaceForRoot=this->Context.SpaceForRoot;
      //LT
      auto&qbo=this->Context.machine.qbo;
      auto&Tools=this->Context.machine.Tools;
      //LT
      result=LinkMetaSpec(qbo.MetaSpec,Tools.DeclareTypeArray,Space,SpaceForMetaSpec);
      if(!result)return;
      SyncTwoSpace_Stage1(qbo.MetaSpec,Space,SpaceForMetaSpec);
    }
    static bool LinkMetaSpec(TQapBinaryObjectLastHope::TMetaSpec&input,TSpaceDependsTools::TDeclareTypeArray&arr,TSpace&Space,TSpace&SpaceForMetaSpec)
    {
      auto pass=[&Space,&SpaceForMetaSpec](TType*pType,const EnvDefTypeStr&def)->bool
      {
        auto&key=def.Decl.Name;
        if(key!=pType->GetFullName())return false;
        auto&impl_BLT=Space[pType->Self];
        auto&decl_BLT=impl_BLT.get_lnk(Space);
        auto&impl_LT=SpaceForMetaSpec[def.Impl.Self];
        auto&decl_LT=SpaceForMetaSpec[def.Decl.Decl.Self];
        impl_LT.ptr_lnk=impl_BLT.ptr;
        decl_LT.ptr_lnk=decl_BLT.ptr;
        impl_LT.name="Sys$$<"+key+">::GetRTTI()";
        decl_LT.name="Def$$<"+key+">::GetRTTI()";
        return true;
      };
      auto link_keyword=[&pass,&arr](const EnvDefTypeStr&def)->bool
      {
        auto&key=def.Decl.Name;
        for(int i=0;i<arr.size();i++)
        {
          auto&ex=arr[i];
          if(pass(ex.first,def))return true;
          if(pass(ex.second,def))return true;
        }
        return false;
      };
      for(int j=0;j<input.size();j++)
      {
        auto&ex=input[j];
        auto&key=ex.Decl.Name;
        if(!link_keyword(ex))
        {
          CppString cpp;
          cpp=key;
          QapDebugMsg("\""+cpp.data+"\" - keyword not found");
          return false;
        }
      }
      return true;
    }
  };

  class GP_BotB_LT:public GP_Bot{
  public:
    typedef GP_BotB_LT SelfClass;
    typedef GP_BotContext_LT TContext;
  public:
    TContext&Context;
  public:
    GP_BotB_LT(TContext&Context):Context(Context){Init();}
   ~GP_BotB_LT(){Free();}
  public:
   TSpace&get_space(){return Context.SpaceForFullDeclEnv;}
  public:
    typedef TClientPtrSolver::TwoMetaType TwoMetaType;
  public:
    void Init()
    {
      auto&result=this->Context.result;
      auto&Space=this->Context.machine.Space;
      auto&SpaceForMetaSpec=this->Context.SpaceForMetaSpec;
      auto&SpaceForFullDeclEnv=this->Context.SpaceForFullDeclEnv;
      auto&SpaceForRoot=this->Context.SpaceForRoot;
      //LT
      copy_envitem_lnk_to_envitem_id(SpaceForFullDeclEnv,SpaceForMetaSpec,Space);
    }
    void Free()
    {
      auto&result=this->Context.result;
      auto&Space=this->Context.machine.Space;
      auto&SpaceForMetaSpec=this->Context.SpaceForMetaSpec;
      auto&SpaceForFullDeclEnv=this->Context.SpaceForFullDeclEnv;
      auto&SpaceForRoot=this->Context.SpaceForRoot;
      //LT
      auto&qbo=this->Context.machine.qbo;
      auto&Tools=this->Context.machine.Tools;
      //LT
      if(true)
      {
        //LT
        TSpace SpaceLinked;
        TPointsHolder buff_FMS;
        //` {
        //`   SpaceMetaSpec->get_lnk(Space),
        //`   SpaceForForFullDeclEnv->get_lnk(Space),
        //`   SpaceForForFullDeclEnv
        //` }
        copy_envitem_lnk_to_envitem_id(SpaceLinked,SpaceForMetaSpec,Space); 
        {for(int i=1;i<SpaceLinked.size();i++)SpaceLinked[i].ptr_lnk=SpaceLinked[i].ptr;}
        {for(int i=1;i<SpaceForMetaSpec.size();i++)buff_FMS.Arr.push_back(std::move(EnvItem().copy(SpaceForMetaSpec[i])));}
        //LT
        TPointsHolder buff_BLT;
        TPointsHolder buff_LT;
        //LT
        result=LinkFullDeclEnv(qbo.FullDeclEnv,Tools,SpaceForFullDeclEnv,Context);
        if(!result)
        {
          buff_FMS.off_all_envitems(SpaceLinked,true);
          SpaceLinked.clear();
          return;
        }
        //LT
        grab_from_implptrs_envitem_lnk_to_buff(qbo.FullDeclEnv.ImplPtrs,buff_BLT,Space,SpaceForFullDeclEnv);
        grab_from_implptrs_envitem_to_buff(qbo.FullDeclEnv.ImplPtrs,buff_LT,Space,SpaceForFullDeclEnv);
        //LT
        buff_BLT.off(Space);
        buff_LT.off(SpaceForFullDeclEnv);
        //LT
        buff_LT.insert_to_end(SpaceLinked);
        buff_BLT.insert_to_end(SpaceLinked);
        //LT
        make_linked(buff_LT,SpaceLinked);
        result=Testing(qbo.FullDeclEnv,TwoMetaType(Tools),SpaceLinked,Context);
        //LT
        check_last_hope();
        //LT
        buff_FMS.off_all_envitems(SpaceLinked,true);
        SpaceLinked.clear();
        //LT
        buff_LT.on(SpaceForFullDeclEnv);
        buff_BLT.on(Space);
        //LT
        if(!result)
        {
          //SpaceForFullDeclEnv.
          return;
        }
        //LT
        TSyncPointsHolder buff_sync;
        buff_sync.copy_envitem_lnk_and_keep_id(buff_LT,Space);
        buff_sync.sync(Space);
        //LT
      }
      //LT
    }
    void check_last_hope()
    {
      //Context.last_hope
    }
    static void make_linked(TPointsHolder&buff,TSpace&Space)
    {
      auto&arr=buff.Arr;
      for(int i=0;i<arr.size();i++)
      {
        auto&item=arr[i];
        auto&jtem=item.get_lnk(Space);
        item.ptr_lnk.set_zero();
        jtem.ptr_lnk.set_zero();
        Space.make_linked(item,jtem);
      }
    }
    typedef FullDeclareEnvRTTI::TImplPtrs TImplPtrs;
    static void grab_from_implptrs_envitem_lnk_to_buff(TImplPtrs&ImplPtrs,TPointsHolder&buff,TSpace&Space,TSpace&SpaceFFDE)
    {
      auto&arr=ImplPtrs;
      buff.Arr.reserve(arr.size()*2);
      for(int i=0;i<arr.size();i++)
      {
        auto&ex=arr[i];
        auto&decl_LT=SpaceFFDE[ex.Decl->Self];
        auto&impl_LT=SpaceFFDE[ex.Impl.Self];
        auto&decl_BLT=decl_LT.get_lnk(Space);
        auto&impl_BLT=impl_LT.get_lnk(Space);
        buff.Arr.push_back(std::move(EnvItem().copy(decl_BLT)));
        buff.Arr.push_back(std::move(EnvItem().copy(impl_BLT)));
      }
    }
    typedef FullDeclareEnvRTTI::TImplPtrs TImplPtrs;
    static void grab_from_implptrs_envitem_to_buff(TImplPtrs&ImplPtrs,TPointsHolder&buff,TSpace&Space,TSpace&SpaceFFDE)
    {
      auto&arr=ImplPtrs;
      buff.Arr.reserve(arr.size()*2);
      for(int i=0;i<arr.size();i++)
      {
        auto&ex=arr[i];
        auto&decl_LT=SpaceFFDE[ex.Decl->Self];
        auto&impl_LT=SpaceFFDE[ex.Impl.Self];
        buff.Arr.push_back(std::move(EnvItem().copy(decl_LT)));
        buff.Arr.push_back(std::move(EnvItem().copy(impl_LT)));
      }
    }
    static bool Testing(FullDeclareEnvRTTI&input,TClientPtrSolver::TwoMetaType&tmt,TSpace&SpaceLinked,TContext&Context)
    {
      DeclareDetail::IsEqualTo<TServerPtrSpace> V(SpaceLinked);
      V.Mute=true;
      auto get_ptr_as_DeclareType=[&tmt](EnvItem&item)->DeclareType*
      {
        bool flag=item.ptr.type->IsBasedOn(tmt.pDeclareType);
        if(flag)return (DeclareType*)item.ptr.get();
        QapDebugMsg(item.name+" - WTF?");
        return nullptr;
      };
      auto&last_hope=Context.machine.last_hope;
      auto&last_hope_str=Context.machine.last_hope_str;
      auto&arr=input.ImplPtrs;
      for(int i=0;i<arr.size();i++)
      {
        auto&ex=arr[i];
        auto*pValue=ex.Decl.get();
        V.Result=false;
        V.pValue=pValue;
        auto&item=SpaceLinked[pValue->Self];
        if(item.ptr_lnk.IsValid())
        {
          auto&jtem=item.get_lnk(SpaceLinked);
          auto*p=get_ptr_as_DeclareType(jtem);
          item.ptr_lnk.set_zero();
          jtem.ptr_lnk.set_zero();
          V.Result=false;
          p->Use(V);
          if(!V.Result)
          {
            auto name=pValue->GetFullName();
            last_hope.push_back(i);
            last_hope_str.push_back(name);
          }
          //if(V.Result)
          {
            bool f0=item.ptr_lnk.get()==jtem.ptr.get();
            bool f1=jtem.ptr_lnk.get()==item.ptr.get();
            QapAssert(f0&&f1);
          }
        }else{
          QapDebugMsg("WTF?");
        }
      }
      int gg=1;
      return V.Result;
    }
    struct t_postponed_item{
      EnvDefTypePtr&def;
      TType*pType;
      TAutoPtr<DeclareType>&pDeclType;
    };
    static bool LinkFullDeclEnv(FullDeclareEnvRTTI&input,TSpaceDependsTools&Tools,TSpace&SpaceForFullDeclEnv,TContext&Context)
    {
      auto&Env=Context.machine.Tools.Env;
      auto&Space=Tools.Space;
      auto&output=Tools.DeclEnv.Arr;
      auto pass=[&Space,&SpaceForFullDeclEnv](DeclareType*pdType,const EnvDefTypePtr&def)->bool
      {
        auto fullname=def.Decl->GetFullName();
        if(fullname!=pdType->GetFullName())return false;
        auto&decl_BLT=Space[pdType->Self];
        auto&impl_BLT=decl_BLT.get_lnk(Space);
        auto&decl_LT=SpaceForFullDeclEnv[def.Decl->Self];
        auto&impl_LT=SpaceForFullDeclEnv[def.Impl.Self];
        impl_LT.ptr_lnk=impl_BLT.ptr;
        decl_LT.ptr_lnk=decl_BLT.ptr;
        impl_LT.name="Sys$$<"+fullname+">::GetRTTI()";
        decl_LT.name="Def$$<"+fullname+">::GetRTTI()";
        return true;
      };
      auto link_keyword=[&pass,&output](const EnvDefTypePtr&def)->bool
      {
        for(int i=0;i<output.size();i++)
        {
          auto&ex=output[i];
          auto*pdType=ex.get();
          //TODO_Adler: if(Space[pdType->Self].is_basedon(TType||DeclareType))continue;
          if(pass(pdType,def))return true;
        }
        return false;
      };
      auto&arr=input.ImplPtrs;
      auto IsBasedOn=EnvAdapter::IsBasedOn;
      vector<t_postponed_item> postponed;
      auto do_some=[&](EnvType&back,EnvDefTypePtr&ex,DeclareType*pdt)
      {
        auto*ptr=back.Type.get();
        //create_ttype_from_declaretype(Env,SpaceForFullDeclEnv,*ptr,*pdts);
        auto exDecl_fullname=ex.Decl->GetFullName();
        {
          string name="Sys$$<"+exDecl_fullname+">::GetRTTI()";
          TType*type=back.Type.Product.pType.get();
          bool flag1=IsBasedOn(type,Sys$$<TTypeStruct>::GetRTTI(Env));
          bool flag2=IsBasedOn(ptr,Context.machine.Tools.pTType);
          bool system=!flag1||flag2;
          Space.AddExt(EnvAdapter::get(ptr->Self),type,name).system=system;
        }
        QapAssert(output.capacity()!=output.size());
        auto&b=qap_add_back(output);
        //first pass
        {
          Context.machine.Tools.Solver.PreConv(ptr,b);
          if(DeclareTypeStruct::UberCast(pdt))
          {
            auto*pb=DeclareTypeStruct::UberCast(b.get());
            pb->Name=DeclareTypeStruct::UberCast(pdt)->Name;
          }
          if(DeclareTypeVector::UberCast(pdt))
          {
            auto*pb=DeclareTypeVector::UberCast(b.get());
            auto*pet=DeclareTypeVector::UberCast(pdt)->ElementType.get();
            auto etfn=pet->GetFullName();
            DeclareType*pet_from_output=nullptr;
            for(int i=0;i+1<output.size();i++)
            {
              auto&ex=output[i];
              if(ex->GetFullName()==etfn)
              {
                QapAssert(!pet_from_output);
                pet_from_output=ex.get();
              }
            }
            QapAssert(pet_from_output);
            pb->ElementType=pet_from_output;
            int gg=1;
          }
          auto&item=Space.AddExt(EnvAdapter::get(b.get()->Self),b.Product.pType.get());
          item.name="Def$$<"+exDecl_fullname+">::GetRTTI()";
          item.ptr_lnk.ptr=EnvAdapter::get(ptr->Self);
          item.ptr_lnk.type=back.Type.Product.pType.get();
        }
        ////second pass
        //{
        //  Context.machine.Tools.Solver.UseConv(ptr,b);
        //}
        postponed.push_back({ex,ptr,b});
        //pb->
        QapAssert(link_keyword(ex));
        //auto fn=ex.Decl->GetFullName();
        //CppString cpp;
        //cpp=fn;
        //QapDebugMsg("\""+cpp.data+"\" - type not found");//TODO: if type are not found then we need to make it!
        //return false;
      };
      // we can't join this loop thgether because it lead to TTypeVector::pElemType refer to undefined object.
      for(int j=0;j<arr.size();j++)
      {
        auto&ex=arr[j];
        if(!link_keyword(ex))
        {
          auto*pdecl=ex.Decl.get();
          QapAssert(pdecl);
          auto*pdts=DeclareTypeStruct::UberCast(pdecl);
          if(!pdts)continue;
          auto&back=qap_add_back(Context.machine.old_types);
          auto*ptr=back.Type.build<TTypeStruct>(Env);
          ptr->Info.Name=pdts->Name;
          QapAssert(!pdts->OwnType);
          auto*pf=back.Factory.build<TTypeStruct::TDynamicFactory>(Env);
          pf->pType=ptr;
          ptr->Factory=pf;
          ptr->Env=&Env;
          //
          do_some(back,ex,pdts);
        }
      }
      // we can't join this loop thgether because it lead to TTypeVector::pElemType refer to undefined object.
      for(int j=0;j<arr.size();j++)
      {
        auto&ex=arr[j];
        if(!link_keyword(ex))
        {
          auto&back=qap_add_back(Context.machine.old_types);
          auto*pdecl=ex.Decl.get();
          QapAssert(pdecl);
          auto*pdtv=DeclareTypeVector::UberCast(pdecl);
          if(!pdtv)
          {
            QapDebugMsg("smart loader does not support the type\n"+pdecl->GetFullName());
            continue;
          }
          //ptr->Info.Name=pdts->Name;
          auto*ptr=back.Type.build<TTypeVector>(Env);
          ptr->Info.Name="vector";
          auto*pf=back.Factory.build<TDynVectorFactory>(Env);
          //auto*pf=back.Factory.build<TTypeVector::TDynamicFactory>(Env);
          pf->pType=ptr;
          pf->VecAPI.pType=ptr;
          ptr->VecAPI=&pf->VecAPI;
          ptr->Factory=pf;
          ptr->Env=&Env;
          auto etfn=pdtv->ElementType->GetFullName();
          auto*pet=Context.machine.FindTypeByName(etfn);
          QapAssert(pet);
          ptr->ElementType=pet;
          //
          do_some(back,ex,pdtv);
        }
      }
      for(int i=0;i<postponed.size();i++)
      {
        auto&ex=postponed[i];
        auto*pdecl=ex.def.Decl.get();
        QapAssert(pdecl);
        auto*pdtv=DeclareTypeVector::UberCast(pdecl);
        auto*pdts=DeclareTypeStruct::UberCast(pdecl);
        if(pdts)
        {
          auto*pType=TTypeStruct::UberCast(ex.pType);
          QapAssert(pType);
          create_ttype_from_declaretype(Env,SpaceForFullDeclEnv,*pType,*pdts,Context);
        }
        if(pdtv)
        {
          auto*pType=TTypeVector::UberCast(ex.pType);
          QapAssert(pType);
          create_ttype_from_declaretype(Env,SpaceForFullDeclEnv,*pType,*pdtv,Context);
        }
        //second pass
        {
          Context.machine.Tools.Solver.UseConv(ex.pType,ex.pDeclType);
        }
      }
      //
      for(int i=0;i<postponed.size();i++)
      {
        auto&ex=postponed[i];
        auto*pdecl=ex.def.Decl.get();
        auto*pdts=DeclareTypeStruct::UberCast(pdecl);
        if(pdts)
        {
          auto*pType=TTypeStruct::UberCast(ex.pType);
          QapAssert(pType);
          if(pType->Info.Size>0)continue;
          DoReCalcMembersTypes(*pType);
          pType->ReCalcMembersOffset();// so we need to call it in right sequence!
        }
      }
      return true;
    }
    static void DoReCalcMembersTypes(TTypeStruct&ref)
    {
      auto*pst=TTypeStruct::UberCast(ref.SubType.get());
      if(pst)DoReCalcMembersTypes(*pst);
      auto&arr=ref.Members;
      for(int i=0;i<arr.size();i++)
      {
        auto&ex=arr[i];
        auto*ps=TTypeStruct::UberCast(ex.Type.get());
        if(!ps)continue;
        if(ps->Info.Size>0)continue;
        ps->ReCalcMembersOffset();
      }
    }
    static void create_ttype_from_declaretype(IEnvRTTI&Env,TSpace&SpaceForFullDeclEnv,TTypeVector&dest,DeclareTypeVector&source,TContext&Context)
    {
      auto*pet=Context.machine.FindTypeByName(source.ElementType->GetFullName());
      dest.ElementType=pet;
      dest.SizeType=Sys$$<int>::GetRTTI(Env);
      dest.Info.Size=dest.Factory->GetSize();
      dest.Info.Align=detail::QapAlignOf<TDynVector>::value;
    }
    static void create_ttype_from_declaretype(IEnvRTTI&Env,TSpace&SpaceForFullDeclEnv,TTypeStruct&dest,DeclareTypeStruct&source,TContext&Context)
    {
      //QapAssert(!source.SubType);
      QapAssert(!source.OwnType);
      dest.Info.Name=source.Name;
      if(source.SubType)
      {
        auto stn=source.SubType->GetFullName();
        auto*pst=Context.machine.FindTypeByName(stn);
        QapAssert(pst);
        dest.SubType=pst;
      }
      dest.Info.Align=-1;
      dest.Info.Size=-1;
      auto&arr=source.Members;
      dest.Members.resize(arr.size());
      for(int i=0;i<arr.size();i++)
      {
        auto&ex=arr[i];
        auto&ey=dest.Members[i];
        ey.Offset=-1;
        ey.Name=ex.Name;
        ey.Value=ex.Value;
        auto sft=ex.Type->GetFullName();
        auto*psft=Context.machine.FindTypeByName(sft);
        QapAssert(psft);
        ey.Type=psft;
        ey.Mode=ex.Mode;
      }
      //dest.ReCalcMembersOffset();
      int gg=1;
    }
  };

  class GP_BotC_LT:public GP_Bot{
  public:
    typedef GP_BotC_LT SelfClass;
    typedef GP_BotContext_LT TContext;
  public:
    TContext&Context;
  public:
    GP_BotC_LT(TContext&Context):Context(Context){Init();}
   ~GP_BotC_LT(){Free();}
  public:
   TSpace&get_space(){return Context.SpaceForRoot;}
  public:
    void Init()
    {
      auto&result=this->Context.result;
      auto&Space=this->Context.machine.Space;
      auto&SpaceForMetaSpec=this->Context.SpaceForMetaSpec;
      auto&SpaceForFullDeclEnv=this->Context.SpaceForFullDeclEnv;
      auto&SpaceForRoot=this->Context.SpaceForRoot;
      //LT
      auto&qbo=this->Context.machine.qbo;
      //LT
      copy_envitem_lnk_to_envitem_id(SpaceForRoot,SpaceForMetaSpec,Space);
      TPointsHolder PH;
      GP_BotB_LT::grab_from_implptrs_envitem_to_buff(qbo.FullDeclEnv.ImplPtrs,PH,Space,SpaceForFullDeclEnv);
      TSyncPointsHolder buff_sync;
      buff_sync.copy_envitem_lnk(PH,Space);
      buff_sync.off(Space);
      SpaceForRoot.resize(SpaceForFullDeclEnv.size());
      buff_sync.on(SpaceForRoot);
      buff_sync.off_all_objects(SpaceForRoot);
      buff_sync.on(Space);
      //buff_sync.on
    }
    void Free()
    {
      auto&result=this->Context.result;
      auto&Space=this->Context.machine.Space;
      auto&SpaceForMetaSpec=this->Context.SpaceForMetaSpec;
      auto&SpaceForFullDeclEnv=this->Context.SpaceForFullDeclEnv;
      auto&SpaceForRoot=this->Context.SpaceForRoot;
      //LT
      auto&qbo=this->Context.machine.qbo;
      auto&raw_ptr=this->Context.raw_ptr;
      //LT
      result=bool(qbo.RootType.get()==raw_ptr.pType);
    }
  };

  class GP_BotD_LT:public GP_Bot{
  public:
    typedef GP_BotD_LT SelfClass;
    typedef GP_BotContext_LT TContext;
  public:
    TContext&Context;
  public:
    GP_BotD_LT(TContext&Context):Context(Context){Init();}
   ~GP_BotD_LT(){Free();}
  public:
   TSpace&get_space(){return Context.SpaceForRoot;}
  public:
    void Init()
    {
      auto&result=this->Context.result;
      auto&Space=this->Context.machine.Space;
      auto&SpaceForMetaSpec=this->Context.SpaceForMetaSpec;
      auto&SpaceForFullDeclEnv=this->Context.SpaceForFullDeclEnv;
      auto&SpaceForRoot=this->Context.SpaceForRoot;
      //LT
      auto&BuffForRoot=this->Context.BuffForRoot;
      //LT
      for(int i=1;i<SpaceForRoot.size();i++)
      {
        BuffForRoot.Arr.push_back(std::move(EnvItem().copy(SpaceForRoot[i])));
      }
    }
    void Free()
    {
      //nothing
    }
  };
  //------------------------------------------- GP_BOTS ---------------------------------------------
public:
  template<class TYPE>
  struct SelectGoooDevContext{typedef void type;};
  template<>
  struct SelectGoooDevContext<IUberLoadDeviceLastHope>{typedef GP_BotContext_LT type;};
  bool Load(IUberLoadDeviceLastHope&Dev,TRawPtr&raw_ptr)
  {
    typedef std::remove_reference<decltype(Dev)>::type IConcreteDevice;
    typedef SelectGoooDevContext<IConcreteDevice>::type TContext;
    typedef TContext::GP_BotA::SelfClass GP_BotA;
    typedef TContext::GP_BotB::SelfClass GP_BotB;
    typedef TContext::GP_BotC::SelfClass GP_BotC;
    typedef TContext::GP_BotD::SelfClass GP_BotD;
    //GP
    typedef SpaceScopeBotLastHope ScopeBotLastHope;
    auto&machine=*this;
    ptrs.Root=raw_ptr;
    //GP
    TContext context(machine,raw_ptr);
    //GP
    {
      GP_BotA gpbot(context);
      ScopeBotLastHope temp(machine,gpbot.get_space());
      ScopeBotLastHope::Go(machine,Dev,ptrs.MetaSpec);
    }
    //GP
    if(!context.result)return false;
    //GP
    {
      GP_BotB gpbot(context);
      ScopeBotLastHope temp(machine,gpbot.get_space());
      ScopeBotLastHope::Go(machine,Dev,ptrs.FullDeclEnv);
    }
    //GP
    if(!context.result)return false;
    //GP
    {
      GP_BotC gpbot(context);
      ScopeBotLastHope temp(machine,gpbot.get_space());
      ScopeBotLastHope::Go(machine,Dev,ptrs.RootType);
    }  
    //GP
    if(!context.result)return false;
    //GP
    {
      GP_BotD gpbot(context);
      ScopeBotLastHope temp(machine,gpbot.get_space());
      ScopeBotLastHope::Go(machine,Dev,ptrs.Root);
    }
    return context.result;
  }
  bool FullLoad(IUberLoadDeviceLastHope&Dev,TRawPtr&raw_ptr)
  {
    return Dev.Load(*this,raw_ptr);
  }
};

template<bool flag>
class ScopeBotLastHope{
public:
  TDataIO Stream;
  TDataIO&Owner;
public:
  ScopeBotLastHope(TDataIO&Owner):Owner(Owner){Init<flag>();}
  ~ScopeBotLastHope(){Free<flag>();}
  TDataIO*operator->(){return Stream;}
  operator TDataIO&(){return Stream;}
public:
  template<bool>void Init();
  template<>void Init<false>(){/*see Free<false>();*/}
  template<>void Init<true>(){Stream.LoadFrom(Owner);}
public:
  template<bool>void Free();
  template<>void Free<false>(){Stream.SaveTo(Owner);}
  template<>void Free<true>(){/*see Init<true>();*/}
public:
};


class TSmartPtrSolverLastHope{
public:
  typedef int SaveIDType;
  static TServerPtr<IEsoteric>*const&get(const SaveIDType&SaveID){return (TServerPtr<IEsoteric>*const&)(void*const&)SaveID;}
  static const SaveIDType&get(TServerPtr<IEsoteric>*const&serv){return (const SaveIDType&)(void*const&)serv;}
public:
  typedef TServerPtrSpace TSpace;
  typedef TClientPtrSolver::TwoMetaType TwoMetaType;
public:
  bool Result;
  TConservationMachineLastHope&machine;
  TSpace&Space;
  TType*pDeclareType;
  TType*pTType;
public:
  TSmartPtrSolverLastHope(TConservationMachineLastHope&machine,TSpace&Space,TwoMetaType&tmt):Result(true),machine(machine),Space(Space),pTType(tmt.pTType),pDeclareType(tmt.pDeclareType){};
public:
  void SolveClient(TType*pType,void*pValue)
  {
    TClientPtrSolver ClientSolver(Space,TwoMetaType(*this));
    TSmartPtrVisitor SPG(ClientSolver,pValue);
    pType->Use(SPG);
  }
public:
  //LoadTime
  TType*PreSolve(THardPtr<TType>&pInput,TType*pBaseType)
  {
    TType*pChunkType=nullptr;
    const int tid=int(pInput.P.ptr);
    bool flag=Space.CheckServTID(tid);
    QapAssert(flag);
    auto&item=Space[tid];
    QapAssert(item.ptr.type);
    bool good=item.ptr.type->IsBasedOn(pTType);
    if(!good)
    {
      QapDebugMsg("way is blocked");
      bool good=item.ptr.type->IsBasedOn(pDeclareType);
      QapAssert(good);
      if(!good)return pChunkType;
      auto*pdType=(DeclareType*)item.ptr.get();
      auto&ex=Space[pdType->Self];
      QapAssert(ex.ptr_lnk.type);
      QapAssert(ex.ptr_lnk.type->IsBasedOn(pTType));
      pChunkType=(TType*)ex.ptr_lnk.get();
    }else{
      pChunkType=(TType*)item.ptr.get();
    }
    QapAssert(pChunkType->IsBasedOn(pBaseType));
    return pChunkType;
  }
  //LoadTime
  TType*PreSolveEx(THardPtr<TType>&pInput)
  {
    TType*pChunkType=nullptr;
    const int tid=int(pInput.P.ptr);
    bool flag=Space.CheckServTID(tid);
    QapAssert(flag);
    auto&item=Space[tid];
    QapAssert(item.ptr.type);
    bool good=item.ptr.type->IsBasedOn(pTType);
    if(!good)
    {
      QapAssert(good);
      return nullptr;
    }else{
      pChunkType=(TType*)item.ptr.get();
    }
    return pChunkType;
  }
public:
  void ST_CheckSaveID(SaveIDType&id,TType*pElemType)
  {
    QapAssert(Space.IsAllowed(id));
    auto&item=Space[id];
    QapAssert(item.ptr.ptr);
    int&sid=item.ptr.ptr->SaveID;
    if(!item.ptr.type->IsBasedOn(pElemType))
    {
      string pt_fn=item.ptr.type->GetFullName();
      string et_fn=pElemType->GetFullName();
      string msg="IsBasedOn(\""+pt_fn+"\",\""+et_fn+"\") == false";
      QapDebugMsg(msg);
    }
    QapAssert(&sid==&id);
  }
  int ST_GetSID(TServerPtr<IEsoteric>*serv,TType*pElemType)
  {
    if(serv)
    {
      auto&id=serv->SaveID;
      if(id)ST_CheckSaveID(id,pElemType);
      if(!id)
      {
        QapAssert(id);
      }
      return id;
    }
    return 0;
  }
public:
  //SaveTime
  void GetSaveID(SaveIDType&sid,TType*pElemType,TSelfPtr<IEsoteric>&ref)
  {
    auto&id=ref.P.SaveID;
    ST_CheckSaveID(id,pElemType);
    QapAssert(&Space[id].ptr.ptr->SaveID==&id);
    sid=id;
    typedef std::remove_reference<decltype(id)>::type id_type;
    static_assert(std::is_same<id_type,SaveIDType>::value,"WTF?");
  }
  //SaveTime
  void GetSaveID(SaveIDType&sid,TType*pElemType,TWeakPtr<IEsoteric>&ref)
  {
    static_assert(std::is_same<int,SaveIDType>::value,"WTF?");
    TServerPtr<IEsoteric>*serv=ref.P.ptr;
    sid=ST_GetSID(serv,pElemType);
  }
  //SaveTime
  void GetSaveID(SaveIDType&sid,TType*pElemType,THardPtr<IEsoteric>&ref)
  {
    static_assert(std::is_same<int,SaveIDType>::value,"WTF?");
    TServerPtr<IEsoteric>*serv=ref.P.ptr;
    sid=ST_GetSID(serv,pElemType);
  }
public:
  void LT_CheckSaveID(const SaveIDType&id,TType*pElemType)
  {
    if(!Space.IsAllowed(id))
    {
      QapDebugMsg("bad id("+IToS(id)+")");
      Result=false;
      return;
    }
  }
public:
  //LoadTime
  void SetSaveID(const SaveIDType&id,TType*pElemType,TSelfPtr<IEsoteric>&ref)
  {
    QapAssert(id);
    LT_CheckSaveID(id,pElemType);
    auto&item=Space[id];
    QapAssert(item.ptr.IsNull());
    item.ptr.ptr=&ref.P;
    item.ptr.type=pElemType;
    ref.P.SaveID=id;
  }
  //LoadTime
  void SetSaveID(const SaveIDType&id,TType*pElemType,TWeakPtr<IEsoteric>&ref)
  {
    static_assert(sizeof(id)==sizeof(void*),"WTF?");
    if(ref.P.ptr){
      QapDebugMsg("new way detected");
      ref.P.ptr->Off(&ref.P);
    }
    ref.P.ptr=get(id);
  }
  //LoadTime
  void SetSaveID(const SaveIDType&id,TType*pElemType,THardPtr<IEsoteric>&ref)
  {
    static_assert(sizeof(id)==sizeof(void*),"WTF?");
    if(ref.P.ptr){
      QapDebugMsg("new way detected");
      ref.P.ptr->Off(&ref.P);
    }
    ref.P.ptr=get(id);
  }
};

class TBinaryLoaderLastHope:public IVisitorRTTI{
public:
  typedef TBinaryLoaderLastHope SelfClass;
  typedef QapIO Stream;
  typedef ScopeBotLastHope<true> StreamBot;//loader
public:
  TSmartPtrSolverLastHope&Solver;
  void*pValue;
  vector<Stream*> streams;
  vector<void*> values;
  vector<TType*> types;
  bool drop_mode;
  vector<int> dropped;
  TBinaryLoaderLastHope(TSmartPtrSolverLastHope&Solver,Stream&IO,TType*pType,void*pValue):Solver(Solver),pValue(pValue),drop_mode(false)
  {
    streams.push_back(&IO);
    types.push_back(pType);
  }
public:
  void push(TType*pType)
  {
    types.push_back(pType);
  }
  void pop(TType*pType)
  {
    QapAssert(!types.empty());
    QapAssert(pType==types.back());
    types.pop_back();
  }
  void ptr_push(void*ptr)
  {
    values.push_back(pValue);
    pValue=ptr;
  }
  void ptr_pop(void*ptr)
  {
    QapAssert(!values.empty());
    QapAssert(ptr==pValue);
    pValue=values.back();
    values.pop_back();
  }
  void stream_push(TDataIO&IO)
  {
    streams.push_back(&IO);
  }
  void stream_pop(TDataIO&IO)
  {
    QapAssert(&IO==streams.back());
    streams.pop_back();
  }
public:
  static void clone_field(TTypeStruct*pd,TTypeStruct*ps,void*pdv,void*dsv,const string&dest,const string&source)
  {
    auto&d=pd->unsafe_find_member(dest);
    auto&s=ps->unsafe_find_member(source);
    QapAssert(d.Type.get()==s.Type.get());
    qap_simple_clone_unsafe(d.Type.get(),d.getValue(pdv),s.getValue(dsv));
  }
public:
  DeclareTypeStruct*get_old_declare_type(const string&fullname)
  {
    auto&last_hope_str=Solver.machine.last_hope_str;
    auto&last_hope=Solver.machine.last_hope;
    if(last_hope.empty())return nullptr;
    auto arr=qap_find_str(last_hope_str,fullname);
    if(arr.empty())return nullptr;
    QapAssert(arr.size()==1);
    int id=last_hope[arr[0]];
    auto&et=Solver.machine.qbo.FullDeclEnv.DeclEnv.Arr[id];
    QapAssert(et->GetFullName()==fullname);
    auto*pet=et.get();
    auto*ptr=DeclareTypeStruct::UberCast(pet);
    return ptr;
  }
  TType*FindTypeByName(IEnvRTTI&Env,const string&fn)
  {
    auto*pmt=Env.FindTypeByName_is_ug_but_work(fn);
    if(pmt)return pmt;
    auto&arr=this->Solver.machine.old_types;
    for(int i=0;i<arr.size();i++)
    {
      auto&ex=arr[i];
      QapAssert(ex.Type);
      if(ex.Type->GetFullName()==fn)return ex.Type.get();
    }
    return nullptr;
  }
  void LoadOldObjectAndConvertToNewFormat(TTypeStruct*pDest,DeclareTypeStruct*pSource)
  {
    auto&Env=Solver.machine.Tools.Env;
    if(pSource->SubType)
    {
      QapAssert(pDest->SubType);
      QapAssert(pSource->SubType->GetFullName()==pDest->SubType->GetFullName());
      auto*pDestSubType=pDest->SubType.get();
      ptr_push(pValue);
      pDestSubType->Use(*this);
      ptr_pop(pValue);
    }
    auto&dm=pDest->Members;
    auto&sm=pSource->Members;
    //QapAssert(dm.size()>sm.size());
    for(int i=0;i<sm.size();i++)
    {
      auto&sf=sm[i];
      auto*pdf=pDest->find_member(sf.Name);
      bool drop_field_value=!pdf;
      if(!drop_field_value)
      {
        //QapDebugMsg("\""+CppString(sf.Name).data+"\" - field not found inside new version of type:\n"+pDest->GetFullName());
        //return;
        auto&df=*pdf;
        drop_field_value=sf.Type->GetFullName()!=df.Type->GetFullName();
        if(sf.Name==df.Name)if(!drop_field_value)
        {
          auto*pdft=df.Type.get();
          auto*pfv=df.getValue(pValue);
          push(pdft);
          ptr_push(pfv);
          pdft->Use(*this);
          ptr_pop(pfv);
          pop(pdft);
          continue;
        }
      }
      if(!drop_field_value)QapDebugMsg("under construction");
      auto ftfn=sf.Type->GetFullName();
      //auto*pOldDeclareType=get_old_declare_type(ftfn);
      //QapAssert(!pOldDeclareType);
      auto*pmt=FindTypeByName(Env,ftfn);
      QapAssert(pmt);
      TAutoPtr<void> vap;
      QapAssert(!drop_mode);drop_mode=true;
      vap.unsafe_build(pmt);
      auto*pobj=vap.get();
      push(pmt);
      ptr_push(pobj);
      pmt->Use(*this);
      ptr_pop(pobj);
      pop(pmt);
      QapAssert(drop_mode);drop_mode=false;
      //vap->
    }
  }
public:
  void Do(TTypeStruct*p)
  {
    auto&last_hope_str=Solver.machine.last_hope_str;
    auto&last_hope=Solver.machine.last_hope;
    for(;;)
    {
      if(last_hope.empty())break;
      auto*ptr=get_old_declare_type(p->GetFullName());
      if(!ptr)break;
      auto&field=ptr->Members;
      LoadOldObjectAndConvertToNewFormat(p,ptr);
      int gg=1;
      return;
      break;
    }
    // [2014.07.31 21:39]: code for TRawRect migration.
    //if(false)if(p->GetFullName()=="TRawRect")
    //{
    //  TRawRect_old old;
    //  string inp_mem;
    //  auto&Env=*p->Env.get();
    //  auto*pOldType=Sys$$<TRawRect_old>::GetRTTI(Env);
    //  push(pOldType);
    //  ptr_push(&old);
    //  pOldType->Use(*this);
    //  ptr_pop(&old);
    //  pop(pOldType);
    //  clone_field(p,pOldType,pValue,&old,"point_size","border_size");
    //  auto prev=split("point_id,point_offset,down,border_size",",");
    //  auto next=split("case_id,pid_a,pid_b,off_a,off_b,down_a,down_b,point_size",",");
    //  for(int i=0;i<p->Members.size();i++)
    //  {
    //    auto&ex=p->Members[i];
    //    if(qap_find_str(prev,ex.Name).size())continue;
    //    if(qap_find_str(next,ex.Name).size())continue;
    //    clone_field(p,pOldType,pValue,&old,ex.Name,ex.Name);
    //  }
    //  int gg=1;
    //  return;
    //  return;
    //  auto&arr=p->Members;
    //  for(int i=0;i<arr.size();i++)
    //  {
    //    auto&ex=arr[i];
    //    if(qap_find_str(next,ex.Name).size()){
    //      QapDebugMsg("WTF?");
    //      continue;
    //    }
    //    if(qap_find_str(prev,ex.Name).size()){
    //      QapDebugMsg("WTF?");
    //      continue;
    //    }
    //    auto*ptr=ex.getValue(pValue);
    //    auto*exType=ex.Type.get();
    //    push(exType);
    //    ptr_push(ptr);
    //    exType->Use(*this);
    //    ptr_pop(ptr);
    //    pop(exType);
    //    if(!Solver.Result)break;
    //  }
    //  return;
    //}
    if(p->SubType)
    {
      auto*pSubType=p->SubType.get();
      ptr_push(pValue);
      pSubType->Use(*this);
      ptr_pop(pValue);
    }
    auto&arr=p->Members;
    for(int i=0;i<arr.size();i++)
    {
      auto&ex=arr[i];
      auto*ptr=ex.getValue(pValue);
      auto*exType=ex.Type.get();
      push(exType);
      ptr_push(ptr);
      exType->Use(*this);
      ptr_pop(ptr);
      pop(exType);
      if(!Solver.Result)break;
    }
  }
  void Do(TTypeSys*p)
  {
    auto&IO=*streams.back();
    p->BinIO->Load(IO,pValue);
  }
  void Do(TTypeVector*p)
  {
    auto*VecAPI=p->VecAPI.get();
    IVectorAPI::SizeType Count=0;
    {
      auto*pSizeType=p->SizeType.get();
      auto*ptr=&Count;
      ptr_push(ptr);
      pSizeType->Use(*this);
      ptr_pop(ptr);
    }
    auto&IO=*streams.back();
    if(!IO.TryLoad(Count))
    {
      string fn=p->GetFullName();
      QapDebugMsg(fn+"::resize("+IToS(Count)+")");
    }
    VecAPI->SetCount(pValue,Count);
    TType*elemType=p->ElementType.get();
    push(elemType);
    for(IVectorAPI::SizeType i=0;i<Count;i++)
    {
      void*ptr=VecAPI->GetItem(pValue,i);
      ptr_push(ptr);
      elemType->Use(*this);
      ptr_pop(ptr);
      if(!Solver.Result)return;
    }
    pop(elemType);
  }
  void Do(TTypeArray*p)
  {
    int Count=p->Size;
    TType*elemType=p->ElementType.get();
    int elemSize=elemType->Info.Size;
    push(elemType);
    for(int i=0;i<Count;i++){
      void*ptr=&((uchar*)pValue)[elemSize*i];
      ptr_push(ptr);
      elemType->Use(*this);
      ptr_pop(ptr);
      if(!Solver.Result)return;
    }
    pop(elemType);
  }
  void Do(TTypeSelfPtr*p)
  {
    TSelfPtr<IEsoteric>&value=*(TSelfPtr<IEsoteric>*)pValue;
    int id=0;
    {
      auto*ptr=&id;
      ptr_push(ptr);
      auto*pmSaveIDType=p->mSaveID.Type.get();
      pmSaveIDType->Use(*this);
      ptr_pop(ptr);
    }
    QapAssert(types.size()>=2);
    auto*pElemType=p->ElementType.get();
    auto*type=types[types.size()-2];
    QapAssert(TTypeSelfPtr::UberCast(types.back()));
    QapAssert(type->IsBasedOn(pElemType));
    if(drop_mode)
    {
      QapDebugMsg("Warrning! Some TSelfPtr are dropped by last_hope_loader.\n"+type->GetFullName());
      qap_add_back(dropped)=id;
      int gg=1;
    }
    if(!drop_mode)
    {
      Solver.SetSaveID(id,type,value);
    }
  }
  void Do(TTypeAutoPtr*p)
  {
    if(!p->ElementType)
    {
      QapAssert(p->ElementType);
      Solver.Result=false;
      return;
    }
    TAutoPtr<void>&value=*(TAutoPtr<void>*)pValue;
    THardPtr<TType>&type=value.Product.pType;
    if(value){
      value=nullptr;
    }
    {
      auto*ptr=(void*)&type;
      auto*pmTypeType=p->mType.Type.get();
      ptr_push(ptr);
      pmTypeType->Use(*this);
      ptr_pop(ptr);
    }
    if(!type)return;
    TType*elemType=p->ElementType.get();
    auto*pType=Solver.PreSolve(type,elemType);
    {
      TRTTIProduct prod;
      prod.pType=pType;
      prod.Init();
      value.Product.Memory=std::move(prod.Memory);
    }
    void*ptr=value.get();
    push(pType);
    {
      TDataIO Stream;
      auto&IO=*streams.back();
      Stream.LoadFrom(IO);
      ptr_push(ptr);
      stream_push(Stream);
      pType->Use(*this);
      stream_pop(Stream);
      ptr_pop(ptr);
    }
    pop(pType);
  }
  void Do(TTypeWeakPtr*p)
  {
    TWeakPtr<IEsoteric>&value=*(TWeakPtr<IEsoteric>*)pValue;
    int id=0;
    {
      auto*ptr=&id;
      ptr_push(ptr);
      auto*pmSaveIDType=p->mSaveID.Type.get();
      pmSaveIDType->Use(*this);
      ptr_pop(ptr);
    }
    Solver.SetSaveID(id,p->ElementType.get(),value);
  }
  void Do(TTypeHardPtr*p)
  {
    THardPtr<IEsoteric>&value=*(THardPtr<IEsoteric>*)pValue;
    int id=0;
    {
      auto*ptr=&id;
      ptr_push(ptr);
      auto*pmSaveIDType=p->mSaveID.Type.get();
      pmSaveIDType->Use(*this);
      ptr_pop(ptr);
    }
    Solver.SetSaveID(id,p->ElementType.get(),value);
  }
  static TType*getSaveIDTypeFrom(TTypeVoidPtr*p)
  {
    TType*pmPtrType=p->mPtr.Type.get();
    auto*pType=TTypeWeakPtr::UberCast(pmPtrType);
    QapAssert(pType);
    auto*pmSaveIDType=pType->mSaveID.Type.get();
    QapAssert(pmSaveIDType);
    return pmSaveIDType;
  }
  void Do(TTypeVoidPtr*p)
  {
    TVoidPtr&value=*(TVoidPtr*)pValue;
    TType*pmTypeType=p->mType.Type.get();
    {
      auto*ptr=&value.type;
      ptr_push(ptr);
      pmTypeType->Use(*this);
      ptr_pop(ptr);
    }
    if(!value.type.P.ptr)return;
    if(true)
    {
      //hard code
      auto*pSaveIDType=getSaveIDTypeFrom(p);
      int id=0;
      {
        auto*ptr=&id;
        ptr_push(ptr);
        pSaveIDType->Use(*this);
        ptr_pop(ptr);
      }
      Solver.SetSaveID(id,pmTypeType,value.ptr);
    }
    QapAssert(value.ptr.P.ptr);
  }
  static bool LT_IsObjectValid(TVoidPtr&object)
  {
    const int oid=int(object.ptr.P.ptr);
    return oid;
  }
  void Do(TTypeFieldPtr*p)
  {
    TFieldPtr&value=*(TFieldPtr*)pValue;
    TType*pmObjectType=p->mObject.Type.get();
    {
      auto*ptr=&value.object;
      ptr_push(ptr);
      pmObjectType->Use(*this);
      ptr_pop(ptr);
    }
    if(!LT_IsObjectValid(value.object))return;
    TType*pmTypeType=p->mType.Type.get();
    {
      auto*ptr=&value.type;
      ptr_push(ptr);
      pmTypeType->Use(*this);
      ptr_pop(ptr);
    }
    auto index=-1;
    TType*pmIndexType=p->mIndex.Type.get();
    {
      auto*ptr=&index;
      ptr_push(ptr);
      pmIndexType->Use(*this);
      ptr_pop(ptr);
    }
    {
      auto&object_type=value.object.type;
      auto*pObjectType=Solver.PreSolveEx(object_type);
      {}
      auto&field_type=value.type;
      TType*pFieldType=Solver.PreSolveEx(field_type);
      {}
      const auto Info=SubFieldsInfo::IndexToFieldInfo(pObjectType,index); //TODO_Adler: need using index of DeclareType.
      if(Info.pType!=pFieldType)
      {
        string base=pObjectType->GetFullName();
        string type1=(Info.pType?Info.pType->GetFullName():"void")+" "+base+"::*";
        string type2=pFieldType->GetFullName()+" "+base+"::*";
        QapDebugMsg("Index : "+IToS(index)+"\nOffset : "+IToS(Info.Offset)+"\ncannot convert from\n'"+type1+"'\nto\n'"+type2+"'");
        value.object.ptr.P.ptr=nullptr;
        value.object.type.P.ptr=nullptr;
        value.type.P.ptr=nullptr;
      }else{
        value.offset=Info.Offset;
      }
    }
  }
public:
  template<template<class> class TEMPL,class U>
  void LoadSpyPtr(TEMPL<U>&value)
  {
    QapDebugMsg("no way.");
    static TBinIO<int>&pod=*THeadBinIO<int>::Get();
    TServerPtr<U>*&serv=value.P.ptr;
    pod.Load(IO,&serv);
    //не пытайся тут решать клиентские указатели.
  }
};

class QapBinaryObjectLastHope{
public:
  typedef TServerPtrSpace TSpace;
  static bool DoLoad(TSmartPtrSolverLastHope&Solver,QapIO&IO,TType*pType,void*pValue)
  {
    if(!Solver.Result)return false;
    TBinaryLoaderLastHope BL(Solver,IO,pType,pValue);
    pType->Use(BL);
    if(!Solver.Result)return false;
    Solver.SolveClient(pType,pValue);
    return Solver.Result;
  }
};

struct UberLoadDeviceBinLastHope:IUberLoadDeviceLastHope
{
  IQapStream&Stream;
  struct TContext
  {
    QapIO&Content;
    TSmartPtrSolverLastHope&Solver;
  };
  TContext*pContext;
  UberLoadDeviceBinLastHope(IQapStream&Stream):Stream(Stream),pContext(nullptr){}
  void Begin(TContext&Ref){QapAssert(!pContext);pContext=&Ref;}
  void End(){QapAssert(pContext);pContext=nullptr;}
  bool Load(TMachine&machine,TRawPtr&raw_ptr)
  {
    if(!Stream.Load())return false;
    bool Result=false;
    auto&Dev=*this;
    auto&Content=Stream.GetContent();
    {
      TSmartPtrSolverLastHope Solver(machine,machine.Space,machine.Tools.get_tmt());
      TContext Context={Content,Solver};
      Begin(Context);
      Result=machine.Load(Dev,raw_ptr);
      End();
    }
    return Result;
  }
  bool DoLoad(TMachine&machine,TRawPtr&raw_ptr)
  {
    auto&Content=pContext->Content;
    auto&Solver=pContext->Solver;
    return QapBinaryObjectLastHope::DoLoad(Solver,Content,raw_ptr.pType,raw_ptr.pValue);
  }
};

bool QapUberFullLoaderLastHope(IEnvRTTI&Env,IQapRawObject&&Object,IUberLoadDeviceLastHope&Dev)
{
  auto&raw_ptr=Object.Get(Env);
  bool Result=false;
  //begin
  {
    TQapBinaryObjectLastHope qbo;
    TConservationMachineLastHope Machine(Env,qbo);
    Result=Machine.FullLoad(Dev,raw_ptr);
    qbo.FullDeclEnv.DeclEnv.Arr.clear();
  }
  //end
  return Result;
}

bool QapPublicUberFullLoaderBinLastHopeMem(IEnvRTTI&Env,IQapRawObject&&Object,const string&Inp)
{
  TQapMemoryStream Stream;
  Stream.IO.IO.mem=Inp;
  bool ok=QapUberFullLoaderLastHope(Env,std::move(Object),UberLoadDeviceBinLastHope(Stream));
  return ok;
}

bool QapPublicUberFullLoaderBinLastHope(IEnvRTTI&Env,IQapRawObject&&Object,const string&filename)
{
  return QapUberFullLoaderLastHope(Env,std::move(Object),UberLoadDeviceBinLastHope(TQapFileStream(filename,true)));
}


namespace
{
  struct ThisCompileUnit:public ICompileUnit
  {
    const char*get_filename()const
    {
      return __FILE__;
    }
    virtual void RegAll(IEnvRTTI&Env)
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
    void Run(IEnvRTTI&Env)
    {
    }
    ThisCompileUnit(){
    }
  };
  ThisCompileUnit ThisCompileUnitInstance;
}