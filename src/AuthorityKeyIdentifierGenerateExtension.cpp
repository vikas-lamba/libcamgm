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

  File:       AuthorityKeyIdentifierGenerateExtension.cpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/

#include  <limal/ca-mgm/AuthorityKeyIdentifierGenerateExtension.hpp>
#include  <limal/Exception.hpp>

using namespace limal;
using namespace limal::ca_mgm;
using namespace blocxx;

AuthorityKeyIdentifierGenerateExtension::AuthorityKeyIdentifierGenerateExtension()
    : ExtensionBase(), keyid(KeyID_none), issuer(Issuer_none)
{}

AuthorityKeyIdentifierGenerateExtension::AuthorityKeyIdentifierGenerateExtension(CA& ca, Type type)
    : ExtensionBase(), keyid(KeyID_none), issuer(Issuer_none)
{}

AuthorityKeyIdentifierGenerateExtension::AuthorityKeyIdentifierGenerateExtension(KeyID kid, Issuer iss)
    : ExtensionBase(), keyid(kid), issuer(iss)
{
    if(keyid == KeyID_none && issuer == Issuer_none) {
        setPresent(false);
    } else {
        setPresent(true);
    }
}

AuthorityKeyIdentifierGenerateExtension::AuthorityKeyIdentifierGenerateExtension(const AuthorityKeyIdentifierGenerateExtension& extension)
    : ExtensionBase(extension), keyid(extension.keyid), issuer(extension.issuer)
{}

AuthorityKeyIdentifierGenerateExtension::~AuthorityKeyIdentifierGenerateExtension()
{}


AuthorityKeyIdentifierGenerateExtension& 
AuthorityKeyIdentifierGenerateExtension::operator=(const AuthorityKeyIdentifierGenerateExtension& extension)
{
    if(this == &extension) return *this;

    ExtensionBase::operator=(extension);
    keyid  = extension.keyid;
    issuer = extension.issuer;
    
    return *this;
}

void
AuthorityKeyIdentifierGenerateExtension::setKeyID(KeyID kid)
{
    if(kid == KeyID_none && issuer == Issuer_none) {
        BLOCXX_THROW(limal::ValueException, 
                     "Invalid value for keyid and issuer. At least one of both must be set");
    }
    setPresent(true);
    keyid = kid;
}

AuthorityKeyIdentifierGenerateExtension::KeyID
AuthorityKeyIdentifierGenerateExtension::getKeyID() const
{
    return keyid;
}

void
AuthorityKeyIdentifierGenerateExtension::setIssuer(Issuer iss)
{
    if(keyid == KeyID_none && iss == Issuer_none) {
        BLOCXX_THROW(limal::ValueException, 
                     "Invalid value for keyid and issuer. At least one of both must be set");
    }
    setPresent(true);
    issuer = iss;
}

AuthorityKeyIdentifierGenerateExtension::Issuer
AuthorityKeyIdentifierGenerateExtension::getIssuer() const
{
    return issuer;
}

void
AuthorityKeyIdentifierGenerateExtension::commit2Config(CA& ca, Type type)
{

}

bool
AuthorityKeyIdentifierGenerateExtension::valid() const
{
    if(!isPresent()) return true;
    if(keyid == KeyID_none && issuer == Issuer_none) {
        return false;
    }
    return true;
}

blocxx::StringArray
AuthorityKeyIdentifierGenerateExtension::verify() const
{
    blocxx::StringArray result;

    if(!isPresent()) return result;
    if(keyid == KeyID_none && issuer == Issuer_none) {
        result.append(String("Invalid value for keyid and issuer. At least one of both must be set"));
    }
    return result;

}
