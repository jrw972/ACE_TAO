// -*- c++ -*-
//
// $Id$
//
// Hand-modified by Chris Cleeland.
//
// ******  Code generated by the The ACE ORB (TAO) IDL Compiler *******
// TAO ORB and the TAO IDL Compiler have been developed by Washington 
// University Computer Science's Distributed Object Computing Group.
//
// Information on TAO is available at
//                 http://www.cs.wustl.edu/~schmidt/TAO.html

#if !defined (_TAO_IDL_OBJKEYC_H_)
#define _TAO_IDL_OBJKEYC_H_

#if !defined (_TAO__TAO_SEQ_OCTET_CH_)
#define _TAO__TAO_SEQ_OCTET_CH_

// *************************************************************
// TAO_tao_seq_Octet
// *************************************************************
  
typedef TAO_Unbounded_Sequence<CORBA::Octet > TAO_tao_seq_Octet;
typedef TAO_tao_seq_Octet* TAO_tao_seq_Octet_ptr;


#endif // end #if !defined


#if !defined (_TAO__TAO_SEQ_OCTET___VAR_CH_)
#define _TAO__TAO_SEQ_OCTET___VAR_CH_

// *************************************************************
// class TAO_tao_seq_Octet_var
// *************************************************************

class TAO_tao_seq_Octet_var
{
public:
  TAO_tao_seq_Octet_var (void); // default constructor
  TAO_tao_seq_Octet_var (TAO_tao_seq_Octet *);
  TAO_tao_seq_Octet_var (const TAO_tao_seq_Octet_var &); // copy constructor
  ~TAO_tao_seq_Octet_var (void); // destructor
    
  TAO_tao_seq_Octet_var &operator= (TAO_tao_seq_Octet *);
  TAO_tao_seq_Octet_var &operator= (const TAO_tao_seq_Octet_var &);
  TAO_tao_seq_Octet *operator-> (void);
  const TAO_tao_seq_Octet *operator-> (void) const;
    
  operator const TAO_tao_seq_Octet &() const;
  operator TAO_tao_seq_Octet &();
  operator TAO_tao_seq_Octet &() const;
  CORBA::Octet &operator[] (CORBA::ULong index);
  // in, inout, out, _retn 
  const TAO_tao_seq_Octet &in (void) const;
  TAO_tao_seq_Octet &inout (void);
  TAO_tao_seq_Octet *&out (void);
  TAO_tao_seq_Octet *_retn (void);
  TAO_tao_seq_Octet *ptr (void) const;

private:
  TAO_tao_seq_Octet *ptr_;
};


#endif // end #if !defined


#if !defined (_TAO__TAO_SEQ_OCTET___OUT_CH_)
#define _TAO__TAO_SEQ_OCTET___OUT_CH_

class TAO_tao_seq_Octet_out
{
public:
  TAO_tao_seq_Octet_out (TAO_tao_seq_Octet *&);
  TAO_tao_seq_Octet_out (TAO_tao_seq_Octet_var &);
  TAO_tao_seq_Octet_out (TAO_tao_seq_Octet_out &);
  TAO_tao_seq_Octet_out &operator= (TAO_tao_seq_Octet_out &);
  TAO_tao_seq_Octet_out &operator= (TAO_tao_seq_Octet *);
  operator TAO_tao_seq_Octet *&();
  TAO_tao_seq_Octet *&ptr (void);
  TAO_tao_seq_Octet *operator-> (void);
  CORBA::Octet &operator[] (CORBA::ULong index);
    
private:
  TAO_tao_seq_Octet *&ptr_;
  // assignment from T_var not allowed
  void operator= (const TAO_tao_seq_Octet_var &);
};


#endif // end #if !defined

typedef TAO_tao_seq_Octet TAO_ObjectKey;
typedef TAO_tao_seq_Octet_var TAO_ObjectKey_var;
typedef TAO_tao_seq_Octet_out TAO_ObjectKey_out;


#if defined (__ACE_INLINE__)
#include "objkeyC.i"
#endif // defined INLINE


#endif // if !defined
