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

  File:       RequestData_Priv.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/
#ifndef    CA_MGM_REQUEST_DATA_PRIV_HPP
#define    CA_MGM_REQUEST_DATA_PRIV_HPP

#include  <ca-mgm/config.h>
#include  <ca-mgm/CommonData.hpp>
#include  <ca-mgm/RequestData.hpp>
#include  <ca-mgm/ByteBuffer.hpp>

#include <openssl/x509.h>


namespace CA_MGM_NAMESPACE {

class RequestData_Priv : public RequestData {
public:
	RequestData_Priv();
	RequestData_Priv(const std::string& requestPath,
	                 FormatType formatType = E_PEM);
	RequestData_Priv(const ByteBuffer& request,
	                 FormatType formatType = E_PEM);
	RequestData_Priv(const RequestData_Priv& data);
	virtual ~RequestData_Priv();

	void                setVersion(uint32_t v);
	void                setKeysize(uint32_t size);
	void                setSubjectDN(const DNObject dn);
	void                setKeyAlgorithm(KeyAlg alg);
	void                setPublicKey(const ByteBuffer key);
	void                setSignatureAlgorithm(SigAlg alg);
	void                setSignature(const ByteBuffer &sig);
	void                setExtensions(const X509v3RequestExts &ext);
	void                setChallengePassword(const std::string &passwd);
	void                setUnstructuredName(const std::string &name);

private:

	RequestData_Priv&   operator=(const RequestData_Priv& data);

	void                parseRequest(X509_REQ *x509);
	void                init(const ByteBuffer& request,
	                         FormatType formatType);
};

}

#endif // CA_MGM_REQUEST_DATA_PRIV_HPP
