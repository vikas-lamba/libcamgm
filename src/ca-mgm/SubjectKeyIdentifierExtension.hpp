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

  File:       SubjectKeyIdentifierExtension.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/
#ifndef    CA_MGM_SUBJECT_KEY_IDENTIFIER_EXTENSION_HPP
#define    CA_MGM_SUBJECT_KEY_IDENTIFIER_EXTENSION_HPP

#include  <ca-mgm/config.h>
#include  <ca-mgm/CommonData.hpp>
#include  <ca-mgm/ExtensionBase.hpp>
#include <ca-mgm/PtrTypes.hpp>

namespace CA_MGM_NAMESPACE {

	class CA;
	class CAConfig;
	class SubjectKeyIdentifierExtImpl;

	class SubjectKeyIdentifierExt : public ExtensionBase {
	public:
		SubjectKeyIdentifierExt();
		SubjectKeyIdentifierExt(CAConfig* caConfig, Type type);
		SubjectKeyIdentifierExt(bool autoDetect, const std::string& keyid = std::string());
		SubjectKeyIdentifierExt(const SubjectKeyIdentifierExt& extension);
		virtual ~SubjectKeyIdentifierExt();

#ifndef SWIG

		SubjectKeyIdentifierExt&
		operator=(const SubjectKeyIdentifierExt& extension);

#endif

		void
		setSubjectKeyIdentifier(bool autoDetect, const std::string& keyId = std::string());

		bool
		isAutoDetectionEnabled() const;

		/**
		 * Get the keyID.
		 *
		 * @return the keyID
		 */
		std::string
		getKeyID() const;

		virtual void
		commit2Config(CA& ca, Type type) const;

		virtual bool
		valid() const;

		virtual std::vector<std::string>
		verify() const;

		virtual std::vector<std::string>
		dump() const;

	private:
		ca_mgm::RWCOW_pointer<SubjectKeyIdentifierExtImpl> m_impl;
	};

}

#endif // CA_MGM_SUBJECT_KEY_IDENTIFIER_EXTENSION_HPP
