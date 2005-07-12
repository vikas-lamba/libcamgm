/*---------------------------------------------------------------------\
|                                                                      |
|                     _     _   _   _     __     _                     |
|                    | |   | | | \_/ |   /  \   | |                    |
|                    | |   | | | |_| |  / /\ \  | |                    |
|                    | |__ | | | | | | / ____ \ | |__                  |
|                    |____||_| |_| |_|/ /    \ \|____|                 |
|                                                                      |
|                             ca-mgm library                           |
|                                                                      |
|                                         (C) SUSE Linux Products GmbH |
\----------------------------------------------------------------------/

  File:       CRLData_Priv.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/
#ifndef    LIMAL_CA_MGM_CRL_DATA_PRIV_HPP
#define    LIMAL_CA_MGM_CRL_DATA_PRIV_HPP

#include  <limal/ca-mgm/config.h>
#include  <limal/ca-mgm/CommonData.hpp>
#include  <limal/ca-mgm/CRLData.hpp>

namespace LIMAL_NAMESPACE {

namespace CA_MGM_NAMESPACE {

    class RevocationEntry_Priv : public RevocationEntry {
    public:
        RevocationEntry_Priv();
        RevocationEntry_Priv(const String&    serial, 
                             time_t           revokeDate,
                             const CRLReason& reason);
        RevocationEntry_Priv(const RevocationEntry_Priv& entry);
        virtual ~RevocationEntry_Priv();
        
        RevocationEntry_Priv& operator=(const RevocationEntry_Priv& entry);

        void        setSerial(const String& serial);

        void        setRevocationDate(time_t date);
        void        setReason(const CRLReason& reason);
        
    };

    class CRLData_Priv : public CRLData {
    public:
        CRLData_Priv();
        CRLData_Priv(const String &caName);
        CRLData_Priv(const CRLData_Priv& data);
        virtual ~CRLData_Priv();

        void   setVersion(blocxx::Int32 version);
        void   setValidityPeriod(time_t last,
                                 time_t next);
        void   setIssuerDN(const DNObject& issuer);
        void   setSignatureAlgorithm(SigAlg sigAlg);
        void   setSignature(const String& sig);
        void   setExtensions(const X509v3CRLExtensions& ext);
        void   setRevocationData(const blocxx::Map<String, RevocationEntry>& data);

    private:
        
        CRLData_Priv& operator=(const CRLData_Priv& data);
    };

}
}

#endif // LIMAL_CA_MGM_CRL_DATA_PRIV_HPP
