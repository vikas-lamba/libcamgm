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

  File:       CAConfig.cpp

  Author:     <Michael Calmer>     <mc@suse.de>
  Maintainer: <Michael Calmer>     <mc@suse.de>

  Purpose:

/-*/

#include <fstream>
#include <iostream>
#include  <limal/ca-mgm/CAConfig.hpp>
#include  "Utils.hpp"
#include  <blocxx/COWIntrusiveCountableBase.hpp>

namespace LIMAL_NAMESPACE
{
namespace CA_MGM_NAMESPACE
{

using namespace limal;
using namespace blocxx;
using namespace limal::INI;
using namespace std;

class CAConfigImpl : public blocxx::COWIntrusiveCountableBase
{
	public:
	CAConfigImpl()
		: parser(INIParser ())
		, srcFilename(String())
	{}

	CAConfigImpl(const String &file)
		: parser(INIParser ())
		, srcFilename(file)
	{}

	CAConfigImpl(const CAConfigImpl& impl)
		: COWIntrusiveCountableBase(impl)
		, parser(impl.parser)
		, srcFilename(impl.srcFilename)
	{}

	virtual ~CAConfigImpl()
	{}
	
	CAConfigImpl* clone() const
	{
		return new CAConfigImpl(*this);
	}

	INI::INIParser  parser;
	String          srcFilename;

};

	
CAConfig::CAConfig(const String &file)
	: m_impl(new CAConfigImpl(file))
{
	blocxx::Array<Options>          options;
	blocxx::StringArray             commentsDescr;
	blocxx::Array<SectionDescr>     sectionDescr;
	blocxx::Array<EntryDescr>       entryDescr;
	blocxx::Array<IoPatternDescr>   rewrites;

	options.append (NO_NESTED_SECTIONS);
	options.append (LINE_CAN_CONTINUE);

	commentsDescr.append ("^[ \t]*#.*$");
	commentsDescr.append ("#.*");
	commentsDescr.append ("^[ \t]*$");
	commentsDescr.append ("^[ \t]*;[^;]+.*$");    

	IoPatternDescr pattern1 = { "^[ \t]*([^=;]*[^ \t;=])[ \t]*=[ \t]*(.*[^ \t]|)[ \t]*$" , "   %s = %s"};
	EntryDescr eDescr1 =  {pattern1, "", "" , false};    
	entryDescr.append (eDescr1);

	IoPatternDescr pattern2 = {"^[ \t]*;;[ \t]*([^=]*[^ \t=])[ \t]*=[ \t]*(.*[^ \t]|)[ \t]*$" , ";;   %s = %s"};
	EntryDescr eDescr2 =  {pattern2, "", "" , false};
	entryDescr.append (eDescr2);

	IoPatternDescr patternBegin1 = {"^[ \t]*\\[[ \t]*(.*[^ \t])[ \t]*\\][ \t]*", "[%s]"};
	IoPatternDescr patternBegin2 = {"^[ \t]*;;[ \t]*\\[[ \t]*(.*[^ \t])[ \t]*\\][ \t]*", ";; [%s]"};    
	IoPatternDescr patternEnd;
	SectionDescr sDescr1 =  {patternBegin1, patternEnd , false };
	sectionDescr.append (sDescr1);
	SectionDescr sDescr2 =  {patternBegin2, patternEnd , false };
	sectionDescr.append (sDescr2);   

	m_impl->parser.initMachine (options, commentsDescr, sectionDescr, entryDescr, rewrites);
	m_impl->parser.initFiles (file);
	m_impl->parser.parse();

	validateAndFix();
}

CAConfig::~CAConfig()
{}

void
CAConfig::dumpTree(Section *section, int level)
{
	String tab = "";
	for (int i = 0; i <= level; i++) tab += "  ";

	if (level == 0)
		LOGIT_INFO (tab);

	String sectionComment = section->getComment();
	if (sectionComment.length() > 0)
		LOGIT_INFO (tab <<
		            "SectionComment " << section->getComment());

	EntryMap eMap= section->getEntries();
	for (EntryMap::iterator i = eMap.begin(); i != eMap.end(); i++)
	{
		Entry entry = i->second;
		String comment = entry.getComment();
		if (comment.length() > 0)
			LOGIT_INFO (tab <<
			            "Comment " << i->first << " : " << entry.getComment());
		LOGIT_INFO (tab <<
		            "Entry   " << i->first << " : " << entry.getValue());
	}

	SectionMap sMap = section->getSections();
	for (SectionMap::iterator i = sMap.begin(); i != sMap.end(); i++)
	{
		Section sec = i->second;
		LOGIT_INFO (tab <<
		            "Section " << i->first);
		dumpTree (&sec, level+1);
	}
}


void
CAConfig::setValue(const String &section, const String &key, const String &value)
{
	if (m_impl->parser.iniFile.contains (section) == NO)
	{
		// creating the section at first
		Section newSection (section, m_impl->parser.iniFile);
		newSection.addValue(key, value);
	}
	else
	{
		// add entry only
		(m_impl->parser.iniFile.getSection (section)).addValue(key, value);
	}
	// and save
	m_impl->parser.write();
}

void
CAConfig::deleteValue(const String &section, const String &key)
{
	if (m_impl->parser.iniFile.contains (section) == SECTION)
	{
		// delete entry
		(m_impl->parser.iniFile.getSection (section)).delEntry (key);
		// and save
		m_impl->parser.write();	
	}
}

blocxx::String
CAConfig::getValue(const String &section, const String &key) const
{
	if (m_impl->parser.iniFile.contains (section) == SECTION)
	{
		// delete entry
		return (m_impl->parser.iniFile.getSection (section)).getValue (key);
	}
	return "";
}

bool
CAConfig::exists(const String &section, const String &key) const
{
	if (m_impl->parser.iniFile.contains(section) == SECTION)
	{
		if((m_impl->parser.iniFile.getSection(section)).contains(key) == VALUE)
		{
			return true;
		}
	}
	return false;
}

blocxx::List<blocxx::String>
CAConfig::getKeylist(const String &section) const
{
	blocxx::List<String> keylist;

	if (m_impl->parser.iniFile.contains(section) == SECTION)
	{
		keylist = m_impl->parser.iniFile.getSection(section).getEntryKeys();
	}
	return keylist;
}

void
CAConfig::copySection(const String &srcSection, const String &destSection)
{
	blocxx::List<String> keylist;

	keylist = getKeylist(srcSection);

	blocxx::List<String>::const_iterator it = keylist.begin();

	for(; it != keylist.end(); ++it)
	{
		setValue(destSection, *it, getValue(srcSection, *it));
	}
}

CAConfig*
CAConfig::clone(const String &file)
{
	ifstream in (m_impl->srcFilename.c_str());
	ofstream out (file.c_str());

	if (!in)
	{
		LOGIT_ERROR ("Cannot open filename " << m_impl->srcFilename );
		return NULL;
	}
	if (!out)
	{
		LOGIT_ERROR ("Cannot open filename " << file );
		return NULL;
	}	

	// coppying
	out << in.rdbuf();
    
	in.close();
	out.close();
	
	return new CAConfig (file);	
}

void
CAConfig::dump()
{
	dumpTree(&(m_impl->parser.iniFile));
}    

// private

CAConfig::CAConfig()
{
}

CAConfig::CAConfig(const CAConfig&)
{
}

CAConfig&
CAConfig::operator=(const CAConfig&)
{
	return *this;
}

void
CAConfig::validateAndFix()
{
	bool didChanges = false;
	
	limal::INI::SectionMap sections = m_impl->parser.iniFile.getSections();

	if(sections.find("req") != sections.end())
	{
		LOGIT_INFO("Found req section: converting req to req_ca, req_client, req_server");

		if(sections.find("req_ca") == sections.end())
		{
			// convert req to req_ca
			copySection("req", "req_ca");
			setValue("req_ca", "req_extensions", "v3_req_ca");
		}
		if(sections.find("req_client") == sections.end())
		{
			// convert req to req_client
			copySection("req", "req_client");
			setValue("req_client", "req_extensions", "v3_req_client");
		}
		if(sections.find("req_server") == sections.end())
		{
			// convert req to req_server
			copySection("req", "req_server");
			setValue("req_server", "req_extensions", "v3_req_server");
		}
		m_impl->parser.iniFile.delSection("req");
		didChanges = true;
		
	}
	if(sections.find("v3_req") != sections.end())
	{
		LOGIT_INFO("Found v3_req section: converting v3_req to v3_req_ca, v3_req_client, v3_req_server");
		
		if(sections.find("v3_req_ca") == sections.end())
		{
			// convert v3_req to v3_req_ca
			copySection("v3_req", "v3_req_ca");
			setValue("v3_req_ca", "basicConstraints", "CA:TRUE");
		}
		if(sections.find("v3_req_client") == sections.end())
		{
			// convert v3_req to v3_req_client
			copySection("v3_req", "v3_req_client");
		}
		if(sections.find("v3_req_server") == sections.end())
		{
			// convert v3_req to v3_req_server
			copySection("v3_req", "v3_req_server");
		}
		m_impl->parser.iniFile.delSection("v3_req");
		didChanges = true;
	}
	if(didChanges)
	{
		m_impl->parser.write();
	}
}


}
}

