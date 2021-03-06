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

  File:       RequestGenerationData.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/

#ifndef    CA_MGM_REQUEST_GENERATION_DATA_HPP
#define    CA_MGM_REQUEST_GENERATION_DATA_HPP

#include  <ca-mgm/config.h>
#include  <ca-mgm/CommonData.hpp>
#include  <ca-mgm/DNObject.hpp>
#include  <ca-mgm/X509v3RequestExtensions.hpp>
#include <ca-mgm/PtrTypes.hpp>

namespace CA_MGM_NAMESPACE
{

	class CA;
	class CAConfig;
	class RequestGenerationDataImpl;

    /**
     * @brief Data representation for generating a certificate request
     *
     * This class is a data representation for generating a certificate request
     */
	class RequestGenerationData {
	public:
		RequestGenerationData();

		/**
		 * Read the Request generation defaults
		 */
		RequestGenerationData(CAConfig* caConfig, Type type);
		RequestGenerationData(const RequestGenerationData& data);
		virtual ~RequestGenerationData();

#ifndef SWIG

		RequestGenerationData&
		operator=(const RequestGenerationData& data);

#endif

		void
		setSubjectDN(const DNObject dn);

		DNObject
		getSubjectDN() const;

		DNObject&
		subjectDN();

		void
		setKeysize(uint32_t size);

		uint32_t
		getKeysize() const;

		void
		setMessageDigest(MD md);

		MD
		getMessageDigest() const;

		void
		setChallengePassword(const std::string &passwd);

		std::string
		getChallengePassword() const;

		void
		setUnstructuredName(const std::string &name);

		std::string
		getUnstructuredName() const;

		void
		setExtensions(const X509v3RequestExts &ext);

		X509v3RequestExts
		getExtensions() const;

		X509v3RequestExts&
		extensions();

		/**
		 * write configuration file
		 */
		void
		commit2Config(CA& ca, Type type) const;

		virtual bool
		valid() const;

		virtual std::vector<std::string>
		verify() const;

		virtual std::vector<std::string>
		dump() const;

	private:
		ca_mgm::RWCOW_pointer<RequestGenerationDataImpl> m_impl;

	};
}
#endif //CA_MGM_REQUEST_GENERATION_DATA_HPP
