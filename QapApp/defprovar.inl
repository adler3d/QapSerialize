#define array(TYPE,N)array<TYPE,N>
#define ADD(TYPE,NAME,VALUE)TYPE NAME;
DEF_PRO_VARIABLE(ADD)
#undef ADD
//===
void DoReset()
{
  t_$ $;
  #define ADD(TYPE,NAME,VALUE)t_$::set(this->NAME,VALUE);
  DEF_PRO_VARIABLE(ADD)
  #undef ADD
}
#ifdef DEF_PRO_CLASSNAME
  typedef DEF_PRO_CLASSNAME() SelfClass;
  #ifdef DEF_PRO_COMPAREABLE
    bool oper_equal(const DEF_PRO_CLASSNAME()&ref)const
    {
      #define F(TYPE,NAME,VALUE)if(this->NAME!=std::move(ref.NAME))return false;
      DEF_PRO_VARIABLE(F);
      #undef F
      return true;
    }
    bool operator==(const DEF_PRO_CLASSNAME()&ref)const{return oper_equal(ref);}
    bool operator!=(const DEF_PRO_CLASSNAME()&ref)const{return !oper_equal(ref);}
    #undef DEF_PRO_COMPAREABLE
  #endif //DEF_PRO_COMPAREABLE
  #ifndef DEF_PRO_UNSAVEABLE
    struct ProxySys$${
      #ifdef DEF_PRO_TEMPLATE_DATAIO
        template<class QapIO>
      #endif //DEF_PRO_TEMPLATE_DATAIO
      static void Save(QapIO&IO,DEF_PRO_CLASSNAME()&ref)
      {
        #define F(TYPE,NAME,VALUE)Sys$$<TYPE>::Save(IO,ref.NAME);
        DEF_PRO_VARIABLE(F);
        #undef F
      }
      #ifdef DEF_PRO_TEMPLATE_DATAIO
        template<class QapIO>
      #endif //DEF_PRO_TEMPLATE_DATAIO
      static void Load(QapIO&IO,DEF_PRO_CLASSNAME()&ref)
      {
        #define F(TYPE,NAME,VALUE)Sys$$<TYPE>::Load(IO,ref.NAME);
        DEF_PRO_VARIABLE(F);
        #undef F
      }
      #ifdef DEF_PRO_TEMPLATE_DATAIO
        #undef DEF_PRO_TEMPLATE_DATAIO
      #endif //DEF_PRO_TEMPLATE_DATAIO
    };
  #endif //DEF_PRO_UNSAVEABLE
  void operator=(DEF_PRO_CLASSNAME()&&ref){oper_set(std::move(ref));}
  DEF_PRO_CLASSNAME()(DEF_PRO_CLASSNAME()&&ref){DoReset();oper_set(std::move(ref));}
  void oper_set(DEF_PRO_CLASSNAME()&&ref)
  {
    #define F(TYPE,NAME,VALUE)this->NAME=std::move(ref.NAME);
    DEF_PRO_VARIABLE(F);
    #undef F
  }
  DEF_PRO_CLASSNAME()(){DoReset();}
  #undef DEF_PRO_CLASSNAME
#endif //DEF_PRO_CLASSNAME

#ifdef DEF_PRO_UNSAVEABLE
  #undef DEF_PRO_UNSAVEABLE
#endif

#undef DEF_PRO_VARIABLE
#undef array