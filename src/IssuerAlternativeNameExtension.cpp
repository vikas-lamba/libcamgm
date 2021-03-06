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

  File:       IssuerAlternativeNameExtension.cpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/

#include  <ca-mgm/IssuerAlternativeNameExtension.hpp>
#include  <ca-mgm/CA.hpp>
#include  <ca-mgm/Exception.hpp>


#include  "Utils.hpp"

namespace CA_MGM_NAMESPACE
{

using namespace ca_mgm;

class IssuerAlternativeNameExtImpl
{
public:
	IssuerAlternativeNameExtImpl()
		: issuerCopy(false)
		, altNameList(std::list<LiteralValue>())
	{}

	IssuerAlternativeNameExtImpl(bool copyIssuer,
	                             const std::list<LiteralValue> &alternativeNameList)
		: issuerCopy(copyIssuer)
		, altNameList(alternativeNameList)
	{}

	IssuerAlternativeNameExtImpl(const IssuerAlternativeNameExtImpl& impl)
		: issuerCopy(impl.issuerCopy)
		, altNameList(impl.altNameList)
	{}

	~IssuerAlternativeNameExtImpl() {}

	IssuerAlternativeNameExtImpl* clone() const
	{
		return new IssuerAlternativeNameExtImpl(*this);
	}

	bool issuerCopy;
	std::list<LiteralValue> altNameList;
};


IssuerAlternativeNameExt::IssuerAlternativeNameExt()
	: ExtensionBase()
	, m_impl(new IssuerAlternativeNameExtImpl())
{}

IssuerAlternativeNameExt::IssuerAlternativeNameExt(bool copyIssuer,
	const std::list<LiteralValue> &alternativeNameList)
	: ExtensionBase()
	, m_impl(new IssuerAlternativeNameExtImpl(copyIssuer, alternativeNameList))
{
	std::vector<std::string> r = checkLiteralValueList(alternativeNameList);
	if(!r.empty())
	{
		LOGIT_ERROR(r[0]);
		CA_MGM_THROW(ca_mgm::ValueException, r[0].c_str());
	}
	setPresent(true);
}

IssuerAlternativeNameExt::IssuerAlternativeNameExt(CAConfig* caConfig, Type type)
	: ExtensionBase()
	, m_impl(new IssuerAlternativeNameExtImpl())
{
	// These types are not supported by this object
	if(type == E_Client_Req || type == E_Server_Req || type == E_CA_Req)
	{
		LOGIT_ERROR("wrong type" << type);
		CA_MGM_THROW(ca_mgm::ValueException,
		             str::form(__("Wrong type: %1."), type).c_str());
	}

	bool p = caConfig->exists(type2Section(type, true), "issuerAltName");
	if(p)
	{
		std::vector<std::string>   sp   = PerlRegEx("\\s*,\\s*")
			.split(caConfig->getValue(type2Section(type, true), "issuerAltName"));
		if(0 == str::compareCI(sp[0], "critical"))  setCritical(true);

		std::vector<std::string>::const_iterator it = sp.begin();
		for(; it != sp.end(); ++it)
		{
			if((*it).find_first_of(":") != std::string::npos)
			{
				if(0 == str::compareCI(*it, "issuer:copy"))
					m_impl->issuerCopy = true;
				else
				{
					try
					{
						LiteralValue lv = LiteralValue(*it);
						m_impl->altNameList.push_back(lv);
					}
					catch(ca_mgm::Exception& e)
					{
						LOGIT_ERROR("invalid value: " << *it);
					}
				}
			}
		}
	}
	setPresent(p);
}

IssuerAlternativeNameExt::IssuerAlternativeNameExt(const IssuerAlternativeNameExt& extension)
	: ExtensionBase(extension)
	, m_impl(extension.m_impl)
{}

IssuerAlternativeNameExt::~IssuerAlternativeNameExt()
{}

IssuerAlternativeNameExt&
IssuerAlternativeNameExt::operator=(const IssuerAlternativeNameExt& extension)
{
	if(this == &extension) return *this;

	ExtensionBase::operator=(extension);
	m_impl = extension.m_impl;

	return *this;
}

void
IssuerAlternativeNameExt::setCopyIssuer(bool copyIssuer)
{
	m_impl->issuerCopy = copyIssuer;
	setPresent(true);
}

bool
IssuerAlternativeNameExt::getCopyIssuer() const
{
	if(!isPresent())
	{
		LOGIT_ERROR("IssuerAlternativeNameExt is not present");
		CA_MGM_THROW(ca_mgm::RuntimeException,
		             __("IssuerAlternativeNameExt is not present."));
	}
	return m_impl->issuerCopy;
}

void
IssuerAlternativeNameExt::setAlternativeNameList(const std::list<LiteralValue> &alternativeNameList)
{
	std::vector<std::string> r = checkLiteralValueList(alternativeNameList);
	if(!r.empty())
	{
		LOGIT_ERROR(r[0]);
		CA_MGM_THROW(ca_mgm::ValueException, r[0].c_str());
	}
	m_impl->altNameList = alternativeNameList;
	setPresent(true);
}

std::list<LiteralValue>
IssuerAlternativeNameExt::getAlternativeNameList() const
{
	if(!isPresent())
	{
		LOGIT_ERROR("IssuerAlternativeNameExt is not present");
		CA_MGM_THROW(ca_mgm::RuntimeException,
		             __("IssuerAlternativeNameExt is not present."));
	}
	return m_impl->altNameList;
}

void
IssuerAlternativeNameExt::addIssuerAltName(const LiteralValue& altName)
{
	if(!altName.valid())
	{
		LOGIT_ERROR("invalid literal value for IssuerAlternativeNameExt");
		CA_MGM_THROW(ca_mgm::ValueException,
		             __("Invalid literal value for IssuerAlternativeNameExt."));
	}
	m_impl->altNameList.push_back(altName);
	setPresent(true);
}

void
IssuerAlternativeNameExt::commit2Config(CA& ca, Type type) const
{
	if(!valid())
	{
		LOGIT_ERROR("invalid IssuerAlternativeNameExt object");
		CA_MGM_THROW(ca_mgm::ValueException,
		             __("Invalid IssuerAlternativeNameExt object."));
	}

	// These types are not supported by this object
	if(type == E_Client_Req || type == E_Server_Req || type == E_CA_Req)
	{
		LOGIT_ERROR("wrong type" << type);
		CA_MGM_THROW(ca_mgm::ValueException,
		             str::form(__("Wrong type: %d."), type).c_str());
	}

	if(isPresent())
	{
		std::string extString;

		if(isCritical()) extString += "critical,";

		if(m_impl->issuerCopy) extString += "issuer:copy,";

		std::string val;
		std::list<LiteralValue>::const_iterator it = m_impl->altNameList.begin();
		for(int j = 0;it != m_impl->altNameList.end(); ++it, ++j)
		{
			val = "";
			if( (val = (*it).commit2Config(ca, type, j)) != "")
			{
				extString += val+",";
			}
		}

		ca.getConfig()->setValue(type2Section(type, true), "issuerAltName",
		                         extString.erase(extString.length()-1));
	}
	else
	{
		ca.getConfig()->deleteValue(type2Section(type, true), "issuerAltName");
	}
}

bool
IssuerAlternativeNameExt::valid() const
{
	if(!isPresent())
	{
		LOGIT_DEBUG("return IssuerAlternativeNameExt::valid() is true");
		return true;
	}

	if(!m_impl->issuerCopy && m_impl->altNameList.empty())
	{
		LOGIT_DEBUG("return IssuerAlternativeNameExt::valid() is false");
		return false;
	}
	std::vector<std::string> r = checkLiteralValueList(m_impl->altNameList);
	if(!r.empty())
	{
		LOGIT_DEBUG(r[0]);
		return false;
	}
	LOGIT_DEBUG("return IssuerAlternativeNameExt::valid() is true");
	return true;
}

std::vector<std::string>
IssuerAlternativeNameExt::verify() const
{
	std::vector<std::string> result;

	if(!isPresent()) return result;

	if(!m_impl->issuerCopy && m_impl->altNameList.empty())
	{
		result.push_back(std::string("invalid value for IssuerAlternativeNameExt"));
	}
	appendArray(result, checkLiteralValueList(m_impl->altNameList));

	LOGIT_DEBUG_STRINGARRAY("IssuerAlternativeNameExt::verify()", result);

	return result;
}

std::vector<std::string>
IssuerAlternativeNameExt::dump() const
{
	std::vector<std::string> result;
	result.push_back("IssuerAlternativeNameExt::dump()");

	appendArray(result, ExtensionBase::dump());
	if(!isPresent()) return result;

	result.push_back("Issuer:copy = " + str::toString(m_impl->issuerCopy));

	std::list< LiteralValue >::const_iterator it = m_impl->altNameList.begin();
	for(; it != m_impl->altNameList.end(); ++it)
	{
		appendArray(result, (*it).dump());
	}

	return result;
}

}
