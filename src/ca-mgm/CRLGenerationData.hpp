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

  File:       CRLGenerationData.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/

#ifndef    CA_MGM_CRL_GENERATION_DATA_HPP
#define    CA_MGM_CRL_GENERATION_DATA_HPP

#include  <ca-mgm/config.h>
#include  <ca-mgm/CommonData.hpp>
#include  <ca-mgm/X509v3CRLGenerationExtensions.hpp>
#include <ca-mgm/PtrTypes.hpp>

namespace CA_MGM_NAMESPACE
{

	class CA;
	class CRLGenerationDataImpl;
	/**
	 * @brief Data representation to generate a CRL
	 *
	 * This class is a data representation to generate a CRL.
	 */
	class CRLGenerationData {
	public:
		CRLGenerationData();
		CRLGenerationData(CAConfig* caConfig, Type type);
		CRLGenerationData(uint32_t hours, 
		                  const X509v3CRLGenerationExts& ext);
		CRLGenerationData(const CRLGenerationData& data);
		virtual ~CRLGenerationData();
        
#ifndef SWIG

		CRLGenerationData&
		operator=(const CRLGenerationData& data);

#endif
		
		void
		setCRLLifeTime(uint32_t hours);
        
		uint32_t
		getCRLLifeTime() const;

		void
		setExtensions(const X509v3CRLGenerationExts& ext);
        
		X509v3CRLGenerationExts
		getExtensions() const;

		X509v3CRLGenerationExts&
		extensions();

		void
		commit2Config(CA& ca, Type type) const;

		virtual bool
		valid() const;
        
		virtual std::vector<std::string>
		verify() const;

		virtual std::vector<std::string>
		dump() const;

	private:
		ca_mgm::RWCOW_pointer<CRLGenerationDataImpl> m_impl;

	};

}

#endif // CA_MGM_CRL_GENERATION_DATA_HPP
