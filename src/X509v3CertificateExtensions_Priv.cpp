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

  File:       X509v3CertificateExtensions_Priv.cpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/
#include  "X509v3CertificateExtensions_Priv.hpp"

using namespace limal;
using namespace limal::ca_mgm;
using namespace blocxx;

X509v3CertificateExtensions_Priv::X509v3CertificateExtensions_Priv()
    : X509v3CertificateExtensions()
{
}

X509v3CertificateExtensions_Priv::X509v3CertificateExtensions_Priv(X509* cert)
    : X509v3CertificateExtensions()
{
}

X509v3CertificateExtensions_Priv::~X509v3CertificateExtensions_Priv()
{
}

void
X509v3CertificateExtensions_Priv::setNsBaseUrl(const NsBaseUrlExtension &ext)
{
    nsBaseUrl = ext;
}

void
X509v3CertificateExtensions_Priv::setNsRevocationUrl(const NsRevocationUrlExtension &ext)
{
    nsRevocationUrl = ext;
}

void
X509v3CertificateExtensions_Priv::setNsCaRevocationUrl(const NsCaRevocationUrlExtension &ext)
{
    nsCaRevocationUrl = ext;
}

void
X509v3CertificateExtensions_Priv::setNsRenewalUrl(const NsRenewalUrlExtension &ext)
{
    nsRenewalUrl = ext;
}

void
X509v3CertificateExtensions_Priv::setNsCaPolicyUrl(const NsCaPolicyUrlExtension &ext)
{
    nsCaPolicyUrl = ext;
}

void
X509v3CertificateExtensions_Priv::setNsSslServerName(const NsSslServerNameExtension &ext)
{
    nsSslServerName = ext;
}

void
X509v3CertificateExtensions_Priv::setNsComment(const NsCommentExtension &ext)
{
    nsComment = ext;
}

void
X509v3CertificateExtensions_Priv::setNsCertType(const NsCertTypeExtension &ext)
{
    nsCertType = ext;
}

void
X509v3CertificateExtensions_Priv::setKeyUsage(const KeyUsageExtension &ext)
{
    keyUsage = ext;
}

void
X509v3CertificateExtensions_Priv::setBasicConstraints(const BasicConstraintsExtension &ext)
{
    basicConstraints = ext;
}

void
X509v3CertificateExtensions_Priv::setExtendedKeyUsage(const ExtendedKeyUsageExtension &ext)
{
    extendedKeyUsage = ext;
}

void
X509v3CertificateExtensions_Priv::setSubjectKeyIdentifier(const SubjectKeyIdentifierExtension &ext)
{
    subjectKeyIdentifier = ext;
}

void
X509v3CertificateExtensions_Priv::setAuthorityKeyIdentifier(const AuthorityKeyIdentifierExtension &ext)
{
    authorityKeyIdentifier = ext;
}

void
X509v3CertificateExtensions_Priv::setSubjectAlternativeName(const SubjectAlternativeNameExtension &ext)
{
    subjectAlternativeName = ext;
}

void
X509v3CertificateExtensions_Priv::setIssuerAlternativeName(const IssuerAlternativeNameExtension &ext)
{
    issuerAlternativeName = ext;
}

void
X509v3CertificateExtensions_Priv::setAuthorityInfoAccess(const AuthorityInfoAccessExtension &ext)
{
    authorityInfoAccess = ext;
}

void
X509v3CertificateExtensions_Priv::setCRLDistributionPoints(const CRLDistributionPointsExtension &ext)
{
    crlDistributionPoints = ext;
}

void
X509v3CertificateExtensions_Priv::setCertificatePolicies(const CertificatePoliciesExtension &ext)
{
    certificatePolicies = ext;
}


//    private:
X509v3CertificateExtensions_Priv::X509v3CertificateExtensions_Priv(const X509v3CertificateExtensions_Priv& extensions)
    : X509v3CertificateExtensions(extensions)
{
}

X509v3CertificateExtensions_Priv&
X509v3CertificateExtensions_Priv::operator=(const X509v3CertificateExtensions_Priv& extensions)
{
    return *this;
}
