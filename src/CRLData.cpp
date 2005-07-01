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

  File:       CRLData.cpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/

#include  <limal/ca-mgm/CRLData.hpp>
#include  <limal/ValueRegExCheck.hpp>
#include  <limal/Exception.hpp>
#include  <blocxx/Format.hpp>

#include  "Utils.hpp"
#include  "X509v3CRLExtensions_Priv.hpp"

using namespace limal;
using namespace limal::ca_mgm;
using namespace blocxx;

inline static ValueCheck initHexCheck() {
    ValueCheck check =
        ValueCheck(new ValuePosixRECheck("^[0-9a-fA-F]+[0-9a-fA-F:]*$"));

    return check;
}

RevocationEntry::RevocationEntry()
    : serial(0), revocationDate(0), revocationReason(CRLReason())
{
}

RevocationEntry::RevocationEntry(const String&    serial, 
                      //                                 time_t           revokeDate,
                                 const CRLReason& reason)
    : serial(serial), revocationDate(0),
      revocationReason(reason)
{
    StringArray r = this->verify();
    if(!r.empty()) {
        BLOCXX_THROW(limal::ValueException, r[0].c_str());
    }
}

RevocationEntry::RevocationEntry(const RevocationEntry& entry)
    : serial(entry.serial), revocationDate(entry.revocationDate),
      revocationReason(entry.revocationReason)
{}

RevocationEntry::~RevocationEntry()
{}

RevocationEntry& 
RevocationEntry::operator=(const RevocationEntry& entry)
{
    if(this == &entry) return *this;
    
    serial           = entry.serial;
    revocationDate   = entry.revocationDate;
    revocationReason = entry.revocationReason;
    
    return *this;
}


void
RevocationEntry::setSerial(const String& serial)
{
    String oldSerial = this->serial;

    this->serial = serial;

    StringArray r = this->verify();
    if(!r.empty()) {
        this->serial = oldSerial;

        BLOCXX_THROW(limal::ValueException, r[0].c_str());
    }
}

/*
void
RevocationEntry::setRevocationDate(time_t date)
{
    revocationDate = date;
}
*/

void
RevocationEntry::setReason(const CRLReason& reason)
{
    if(!reason.valid()) {
        BLOCXX_THROW(limal::ValueException, "invalid CRL reason");
    }
    revocationReason = reason;
}

blocxx::String
RevocationEntry::getSerial() const
{
    return serial;
}

time_t
RevocationEntry::getRevocationDate() const
{
    return revocationDate;
}

CRLReason
RevocationEntry::getReason() const
{
    return revocationReason;
}

bool
RevocationEntry::valid() const
{
    ValueCheck check = initHexCheck();
    if(!check.isValid(serial)) {
        LOGIT_DEBUG("invalid serial: "<< serial);
        return false;
    }
    return revocationReason.valid();
}

blocxx::StringArray
RevocationEntry::verify() const
{
    StringArray result;
    
    ValueCheck check = initHexCheck();
    if(!check.isValid(serial)) {
        result.append(Format("invalid serial: %1", serial).toString());
    }
    result.appendArray(revocationReason.verify());

    LOGIT_DEBUG_STRINGARRAY("CRLData::verify()", result);
    
    return result;
}


// ##################################################################

CRLData::CRLData(const CRLData& data)
    : version(data.version), lastUpdate(data.lastUpdate),
      nextUpdate(data.nextUpdate), issuer(data.issuer),
      signatureAlgorithm(data.signatureAlgorithm), signature(data.signature),
      extensions(data.extensions), revocationData(data.revocationData)
{
}

CRLData::~CRLData()
{
}

CRLData& 
CRLData::operator=(const CRLData& data)
{
    if(this == &data) return *this;

    version            = data.version;
    lastUpdate         = data.lastUpdate;
    nextUpdate         = data.nextUpdate;
    issuer             = data.issuer;
    signatureAlgorithm = data.signatureAlgorithm;
    signature          = data.signature;
    extensions         = data.extensions;
    revocationData     = data.revocationData;

    return *this;
}

blocxx::Int32
CRLData::getVersion() const
{
    return version;
}

time_t
CRLData::getLastUpdateDate() const
{
    return lastUpdate;
}

time_t
CRLData::getNextUpdateDate() const
{
    return nextUpdate;
}

DNObject
CRLData::getIssuerDN() const
{
    return issuer;
}

SigAlg
CRLData::getSignatureAlgorithm() const
{
    return signatureAlgorithm;
}

blocxx::String
CRLData::getSignatureAlgorithmAsString() const
{
    return String();
}

blocxx::String
CRLData::getSignature() const
{
    return signature;
}

X509v3CRLExtensions
CRLData::getExtensions() const
{
    return extensions;
}

blocxx::Map<blocxx::String, RevocationEntry>
CRLData::getRevocationData() const
{
    return revocationData;
}

RevocationEntry
CRLData::getRevocationEntry(const String& oid)
{
    return RevocationEntry();
}

bool
CRLData::valid() const
{
    if(version < 1 || version > 2) {
        LOGIT_DEBUG("invalid version: " << version);
        return false;
    }
    if(lastUpdate == 0) {
        LOGIT_DEBUG("invalid lastUpdate:" << lastUpdate);
        return false;
    }
    if(nextUpdate <= lastUpdate) {
        LOGIT_DEBUG("invalid nextUpdate:" << nextUpdate);
        return false;
    }
    if(!issuer.valid())  return false;

    ValueCheck checkHex = initHexCheck();
    if(!checkHex.isValid(signature)) {
        LOGIT_DEBUG("invalid signature:" << signature);
        return false;
    }

    if(!extensions.valid()) return false;

    blocxx::Map<String, RevocationEntry>::const_iterator it = revocationData.begin();
    for(; it != revocationData.end(); ++it) {
        if(!((*it).second).valid()) {
            return false;
        }
    }
    return true;
}

blocxx::StringArray
CRLData::verify() const
{
    StringArray result;
    
    if(version < 1 || version > 2) {
        result.append(Format("invalid version: %1", version).toString());
    }
    if(lastUpdate == 0) {
        result.append(Format("invalid lastUpdate: %1", lastUpdate).toString());
    }
    if(nextUpdate <= lastUpdate) {
        result.append(Format("invalid nextUpdate: %1", nextUpdate).toString());
    }
    result.appendArray(issuer.verify());

    ValueCheck checkHex = initHexCheck();
    if(!checkHex.isValid(signature)) {
        result.append(Format("invalid signature: %1", signature).toString());
    }

    result.appendArray(extensions.verify());

    blocxx::Map<String, RevocationEntry>::const_iterator it = revocationData.begin();
    for(; it != revocationData.end(); ++it) {
        result.appendArray(((*it).second).verify());
    }
    LOGIT_DEBUG_STRINGARRAY("CRLData::verify()", result);
    
    return result;
}


//    protected:
CRLData::CRLData()
    : extensions(X509v3CRLExtensions_Priv())
{
}

