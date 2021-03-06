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

  File:       CRLReason.hpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/
#ifndef    CA_MGM_CRL_REASON_HPP
#define    CA_MGM_CRL_REASON_HPP

#include  <ca-mgm/config.h>
#include  <ca-mgm/CommonData.hpp>
#include <ca-mgm/PtrTypes.hpp>

namespace CA_MGM_NAMESPACE {

	class CRLReasonImpl;

	class CRLReason {
	public:

		CRLReason();

		/**
		 * Constructor
		 *
		 * @param reason revocation reason. Valid strings are: none,
		 * unspecified, keyCompromise, CACompromise, affiliationChanged,
		 * superseded, cessationOfOperation, certificateHold and removeFromCRL
		 */
		CRLReason(const std::string& reason);
		CRLReason(const CRLReason& reason);
		virtual ~CRLReason();

#ifndef SWIG

		CRLReason&
		operator=(const CRLReason& reason);

#endif

		/**
		 * Set a new revocation reason
		 *
		 * @param reason revocation reason. Valid strings are: none,
		 * unspecified, keyCompromise, CACompromise, affiliationChanged,
		 * superseded, cessationOfOperation, certificateHold and removeFromCRL
		 */
		void
		setReason(const std::string& reason);

		/**
		 * Return the revocation reason as string
		 */
		std::string
		getReason() const;

		/**
		 * Set reason to certificateHold and add a hold instruction
		 *
		 * @param holdInstruction valid strings are: holdInstructionNone,
		 * holdInstructionCallIssuer, holdInstructionReject or an OID
		 */
		void
		setHoldInstruction(const std::string& holdInstruction);

		std::string
		getHoldInstruction() const;

		/**
		 * Set reason to keyCompromise and add the compromise date
		 *
		 * @param compromiseDate the date when the key was compromised
		 */
		void
		setKeyCompromiseDate(time_t compromiseDate);

		time_t
		getKeyCompromiseDate() const;

		std::string
		getKeyCompromiseDateAsString() const;

		/**
		 * Set reason to CACompromise and add the compromise date
		 *
		 * @param compromiseDate the date when the CA was compromised
		 */
		void
		setCACompromiseDate(time_t compromiseDate);

		time_t
		getCACompromiseDate() const;

		std::string
		getCACompromiseDateAsString() const;

		virtual bool
		valid() const;

		virtual std::vector<std::string>
		verify() const;

		virtual std::vector<std::string>
		dump() const;

	private:
		ca_mgm::RWCOW_pointer<CRLReasonImpl> m_impl;

		std::string
		checkHoldInstruction(const std::string& hi) const;

		bool
		checkReason(const std::string& reason) const;

	};

}

#endif // CA_MGM_CRL_REASON_HPP
