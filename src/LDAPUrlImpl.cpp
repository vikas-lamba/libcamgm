/*---------------------------------------------------------------------\
|                                                                      |
|                     _     _   _   _     __     _                     |
|                    | |   | | | \_/ |   /  \   | |                    |
|                    | |   | | | |_| |  / /\ \  | |                    |
|                    | |__ | | | | | | / ____ \ | |__                  |
|                    |____||_| |_| |_|/ /    \ \|____|                 |
|                                                                      |
|                             core library                             |
|                                                                      |
|                                         (C) SUSE Linux Products GmbH |
\----------------------------------------------------------------------/

  File:       LDAPUrlImpl.cpp

  Maintainer: Marius Tomaschewski

/-*/

#include <ca-mgm/PosixRegEx.hpp>
#include <ca-mgm/String.hpp>

#include "LDAPUrlImpl.hpp"
#include "Utils.hpp"


// -------------------------------------------------------------------
namespace CA_MGM_NAMESPACE
{
namespace url
{

// ---------------------------------------------------------------
LDAPUrlImpl::LDAPUrlImpl()
	: UrlBase()
{
	configure();
}

// -------------------------------------------------------------------
LDAPUrlImpl::LDAPUrlImpl(const LDAPUrlImpl &url)
	: UrlBase(url)
{}

// -------------------------------------------------------------------
UrlBase *
LDAPUrlImpl::clone() const
{
	return new LDAPUrlImpl(*this);
}

// -------------------------------------------------------------------
std::vector<std::string>
LDAPUrlImpl::getKnownSchemes() const
{
	std::vector<std::string> schemes(2);
	schemes[0] = "ldap";
	schemes[1] = "ldaps";
	return schemes;
}

// -------------------------------------------------------------------
void
LDAPUrlImpl::configure()
{
	config("sep_pathparams",  "");
	config("psep_querystr",   "?");
	config("vsep_querystr",   "");

	// host is required (isValid=>false)
	// but not mandatory (see RFC 2255),
	// that is, accept empty host.
	config("require_host",    "y");

	// not allowed here
	config("rx_username",     "");
	config("rx_password",     "");
	config("rx_fragment",     "");
	config("rx_pathparams",   "");
}

// -------------------------------------------------------------------
ca_mgm::url::ParamMap
LDAPUrlImpl::getQueryStringMap(ca_mgm::url::EEncoding eflag) const
{
	static const char * const keys[] = {
		"attrs", "scope", "filter", "exts", NULL
	};
	ca_mgm::url::ParamMap pmap;
	std::vector<std::string>          parr( getQueryStringArray());
	if( parr.size() <= 4)
	{
		for(size_t i=0; i<parr.size(); i++)
		{
			if(eflag == ca_mgm::url::E_ENCODED)
				pmap[keys[i]] = parr[i];
			else
				pmap[keys[i]] = ca_mgm::url::decode( parr[i]);
		}
	}
	else
	{
		CA_MGM_THROW(ca_mgm::url::UrlNotSupportedException,
			str::form(__("Unable to parse LDAP URL query string '%s'."),
			       getQueryString().c_str()).c_str()
		);
	}
	return pmap;
}

// -------------------------------------------------------------------
void
LDAPUrlImpl::setQueryStringMap(const ca_mgm::url::ParamMap &pmap)
{
	static const char * const keys[] = {
		"attrs", "scope", "filter", "exts", NULL
	};

	// remove psep ("?") from safe chars
	std::string join_safe;
	std::string safe(config("safe_querystr"));
	std::string psep(config("psep_querystr"));
	for(size_t i=0; i<safe.length(); i++)
	{
		if( psep.find_first_of(safe[i]) == std::string::npos)
			join_safe += safe[i];
	}

	std::vector<std::string>                  parr(4);
	ca_mgm::url::ParamMap::const_iterator p;
	for(p=pmap.begin(); p!=pmap.end(); ++p)
	{
		bool found=false;
		for(size_t i=0; i<4; i++)
		{
			if(p->first == keys[i])
			{
				found=true;
				parr[i] = ca_mgm::url::encode(p->second, join_safe);
			}
		}
		if( !found)
		{
			CA_MGM_THROW(UrlNotSupportedException,
				str::form(__("Invalid LDAP URL query parameter '%s'."),
				       p->first.c_str()).c_str()
			);
		}
	}
	setQueryStringArray(parr);
}


// -------------------------------------------------------------------
}      // End of url namespace
}      // End of CA_MGM_NAMESPACE
// vim: set ts=8 sts=8 sw=8 ai noet:
