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

  File:       CertificateData_Priv.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/
#ifndef    CA_MGM_CERTIFICATE_DATA_PRIV_HPP
#define    CA_MGM_CERTIFICATE_DATA_PRIV_HPP

#include  <ca-mgm/config.h>
#include  <ca-mgm/CommonData.hpp>
#include  <ca-mgm/CertificateData.hpp>
#include  <ca-mgm/ByteBuffer.hpp>

#include <openssl/x509.h>


namespace CA_MGM_NAMESPACE {

class CertificateData_Priv : public CertificateData {
public:
	CertificateData_Priv();

	CertificateData_Priv(const ByteBuffer &certificate,
	                     FormatType formatType = E_PEM);

	/**
	 * Construct a CertificateData object by parsing a certificate
	 * file.
	 *
	 *
	 */
	CertificateData_Priv(const std::string &certificatePath,
	                     FormatType formatType = E_PEM);

	CertificateData_Priv(const CertificateData_Priv& data);

	virtual ~CertificateData_Priv();

	void
	setVersion(uint32_t v);

	void
	setSerial(const std::string& serial);

	void
	setCertifyPeriode(time_t start, time_t end);

	void
	setIssuerDN(const DNObject& issuer);

	void
	setSubjectDN(const DNObject& subject);

	void
	setKeysize(uint32_t size);

	void
	setPublicKeyAlgorithm(KeyAlg pubKeyAlg);

	void
	setPublicKey(const ByteBuffer derPublicKey);

	void
	setSignatureAlgorithm(SigAlg sigAlg);

	void
	setSignature(const ByteBuffer& sig);

	void
	setExtensions(const X509v3CertificateExts& ext);

	void
	setFingerprint(const std::string& fp);

private:
	CertificateData_Priv& operator=(const CertificateData_Priv& data);

	void
	init(const ByteBuffer &certificate, FormatType formatType);

	void
	parseCertificate(X509 *x509);
};

}

#endif // CA_MGM_CERTIFICATE_DATA_PRIV_HPP
