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

  File:       RequestData.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/
#ifndef    LIMAL_CA_MGM_REQUEST_DATA_HPP
#define    LIMAL_CA_MGM_REQUEST_DATA_HPP

#include  <limal/ca-mgm/config.h>
#include  <limal/ca-mgm/CommonData.hpp>
#include  <limal/ca-mgm/X509v3RequestExtensions.hpp>
#include  <limal/ca-mgm/DNObject.hpp>
#include  <limal/ByteBuffer.hpp>

namespace LIMAL_NAMESPACE {

namespace CA_MGM_NAMESPACE {

    /**
     * @brief Read-only data representation of a request
     *
     * This class is a read-only data representation of a request
     */
    class RequestData {
    public:
        RequestData(const RequestData& data);
        virtual ~RequestData();

        RequestData&
        operator=(const RequestData& data);

        blocxx::UInt32
        getVersion() const;
        
        blocxx::UInt32
        getKeysize() const;
        
        DNObject
        getSubjectDN() const;
        
        KeyAlg
        getKeyAlgorithm() const;
        
        limal::ByteBuffer
        getPublicKey() const;
        
        SigAlg
        getSignatureAlgorithm() const;
        
        limal::ByteBuffer
        getSignature() const;
        
        X509v3RequestExts
        getExtensions() const;
        
        String
        getChallengePassword() const;
        
        String
        getUnstructuredName() const;

    	/**
    	 * Return the Request in a human readable format
    	 * (Format may change)
    	 */
    	String
    	getRequestAsText() const;

    	/**
    	 * Return the Request extensions in a human readable format
    	 * (Format may change)
    	 */
    	String
    	getExtensionsAsText() const;
    	
        virtual bool
        valid() const;
        
        virtual blocxx::StringArray
        verify() const;

        virtual blocxx::StringArray
        dump() const;

    protected:
        RequestData();

        blocxx::UInt32    version;

        DNObject          subject;
        blocxx::UInt32    keysize;

        KeyAlg            pubkeyAlgorithm; // oder enum?

        // DER des public key
        //   man EVP_PKEY_set1_RSA
        //   man EVP_PKEY_get1_RSA
        //   man i2d_RSAPublicKey     => i2d == internal to DER
        //   man d2i_RSAPublicKey     => d2i == DER to internal
        ByteBuffer        publicKey;  

        SigAlg            signatureAlgorithm;
        ByteBuffer        signature;    
                                        // mit private key verschlüsselter Hash wert
                                        // des Requests

        X509v3RequestExts extensions;

        // attributes 
        String            challengePassword;
        String            unstructuredName;

    	String            text;
    	String            extText;

    };

}
}

#endif // LIMAL_CA_MGM_REQUEST_DATA_HPP
