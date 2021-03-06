/**							-*- c++ -*-
 * YaST2: Core system
 *
 * Description:
 *   YaST2 SCR: Ini file agent.
 *
 * Authors:
 *   Petr Blahos <pblahos@suse.cz>
 *
 * $Id: IniParser.h 20460 2004-12-01 17:33:24Z mvidner $
 */

#ifndef IniParser_h
#define IniParser_h

#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <regex.h>
#include <ca-mgm/String.hpp>
#include <ca-mgm/PosixRegEx.hpp>

#include <iosfwd>
#include <fstream>
#include <vector>

#include "INIParser/INIParserDescr.hpp"
#include "INIParser/IniFile.h"

namespace CA_MGM_NAMESPACE
{
namespace INI
{

/**
 * Tries to match a string against a regex and holds results
 */
class RegexMatch
{
public:
    /** Matched subexpressions (0 - the whole regex) */
    std::vector<std::string> matches;
    /** The unmatched part of the String */
    std::string rest;

    /** @return i-th match */
    const std::string& operator[] (size_t i) { return matches[i]; }
    /** did the String match */
    operator bool () { return matches.size () > 0; }

    /**
     * @param rx a compiled regex
     * @param s  a String to match
     * @param nmatch how many rm_matches to reserve
     */
    RegexMatch (PosixRegEx& rx, const std::string& s) {
	PosixRegEx::MatchArray  rm_matches;
	if( rx.execute(rm_matches, s) && !rm_matches.empty())
	{
	    // match
	    rest = s.substr (0, rm_matches[0].rm_so) +
		s.substr (rm_matches[0].rm_eo);
	}
	else
	{
	    // no match
	    rest = s;
	}
	for (size_t i = 0; i < rm_matches.size() ; ++i)
	{
	    matches.push_back (s.substr (rm_matches[i].rm_so,
					    rm_matches[i].rm_eo - rm_matches[i].rm_so));
	}
    }

};

/**
 * Eg. rx: "^ *Section +(.*)$", out: "Section %s"
 */
struct IoPattern
{
    PosixRegEx rx;
    std::string out;
};

/**
 * section description
 */
struct section
{
    IoPattern begin;
    IoPattern end;
    bool end_valid;
};

/**
 * Parametr description
 */
struct param
{
    /** single-line values, the normal case */
    IoPattern line;
    /** multiline begin */
    PosixRegEx begin;
    /** multiline end */
    PosixRegEx end;
    /** was multiline specified*/
    bool multiline_valid;
};

struct FileDescr
{
    /**
     * File name
     */
    std::string fn;
    /**
     * Section name
     */
    std::string sn;
    /**
     * Time of the last modification
     */
    time_t timestamp;
    FileDescr (char*fn);
    bool changed ();
    FileDescr () {}
};

/**
 * Contains info from scrconf file and ini file read routines.
 */
class IniParser
{
private:
    /**
     * Time of last modification of file, used in single file mode.
     */
    time_t timestamp;
    /**
     * Times of last modification of read files, used in multiple files
     * mode.
     */
    std::map<std::string,FileDescr> multi_files;
    /**
     * File name of the ini file -- single file mode only.
     */
    std::string file;
    /**
     * Get time stamp of file in sinble file mode.
     */
    time_t getTimeStamp();
    /** if there is \ at the end of line, next line is appended to the current one */
    bool line_can_continue;
    /** ignore case in regexps */
    bool ignore_case_regexps;
    /** ignore case in keys and section names */
    bool ignore_case;
    /** if ignore case, prefer upper case when saving */
    bool prefer_uppercase;
    /** if ignore case, outputs first upper and other lower
     * If not first_upper, nor prefer_uppercase is set, keys and values are
     * saved in lower case.
     */
    bool first_upper;
    /** nested sections are not allowed */
    bool no_nested_sections;
    /** values at the top level(not in section) are allowed */
    bool global_values;
    /** more values or sections of the same name are allowed */
    bool repeat_names;
    /** lines are parsed for comments after they are parsed for values */
    bool comments_last;
    /** multiline values are connected into one */
    bool join_multiline;
    /** do not kill empty lines at final comment at the end of top-level section */
    bool no_finalcomment_kill;
    /** read-only */
    bool read_only;
    /** ini file sections are created in flat-mode */
    bool flat;

    /** this String is printed before each line in subsections */
    std::string subindent;
    /**
     * Regular expression for comments over whole line.
     */
    std::vector<PosixRegEx> linecomments;
    /**
     * Regular expressions for comments over part of the line.
     */
    std::vector<PosixRegEx> comments;
    /**
     * Regular expressions for sections.
     */
    std::vector<section> sections;
    /**
     * Regular expressions for parameters (keys/values).
     */
    std::vector<param> params;
    /**
     * Regular expressions for rewrite rules.
     */
    std::vector<IoPattern> rewrites;

    /**
     * opened file for scanner
     */
    std::ifstream scanner;
    /**
     * name of scanned file
     */
    std::string scanner_file;
    /**
     * line number of scanned file
     */
    int scanner_line;

    /**
     * set to true in initMachine (after internal parsing structures are
     * initialized, when IniParser is ready to work).
     */
    bool started;

    /**
     * Multiple files mode or single file mode?
     */
    bool multiple_files;
    /**
     * Array of globe-expressions.
     */
    std::vector<std::string> files;

    /**
     * Open ini file.
     */
    int scanner_start(const char*fn);
    /**
     * Close ini file.
     */
    void scanner_stop();
    /**
     * get line from ini file.
     */
    int scanner_get(std::string&s);

    /**
     * Parse one ini file and build a structure of IniSection.
     */
    int parse_helper(IniSection&ini);
    /**
     * Write one ini file.
     */
    int write_helper(IniSection&ini, std::ofstream&of,int depth);

    // create Logger instance
    //Logger logger;

    IniParser(const IniParser &);
    IniParser& operator=(const IniParser &);

public:

    /**
     * Toplevel ini section.
     */
    IniSection inifile;
    // apparently the uninitialized members are filled in
    // by the grammar definition
    IniParser () :
	linecomments (), comments (),
	sections (), params (), rewrites (),
	started (false), multiple_files (false),
	//logger (INIPARSER),
//	inifile ("toplevel")
	inifile (this)
	    {}
    virtual ~IniParser ();
    /**
     * Sets parser to single file mode and sets the file name to read.
     * @param fn file name of ini file
     */
    void initFiles (const char*fn);
    /**
     * Sets parser to multiple file mode and sets the glob-expressions.
     * @param f list of glob-expressions
     */
    void initFiles (const std::vector<std::string>&f);
    /**
     * Sets flags and regular expressions.
     */
    void initOptions (const std::vector<std::string>&options);
    /**
     * Sets flags and regular expressions.
     */
    void initRewrite (const std::vector<IoPatternDescr>&rewriteArray);
    /**
     * Sets flags and regular expressions.
     */
    void initSubident (const std::string ident);
    /**
     * Sets flags and regular expressions.
     */
    void initComments (const std::vector<std::string>&comm);
    /**
     * Sets flags and regular expressions.
     */
    void initSection (const std::vector<SectionDescr>& sect);
    /**
     * Sets flags and regular expressions.
     */
    void initParam (const std::vector<EntryDescr>& entries);

    bool isStarted() { return started; }
    void reset() {
	ignore_case_regexps = ignore_case = prefer_uppercase = first_upper
	    = line_can_continue = no_nested_sections = global_values =
	    repeat_names = comments_last = join_multiline =
	    no_finalcomment_kill = read_only = flat = false;
	started = false; }

    void initCommit() { started = true; }

    /**
     * Parse the ini files. Parser must be started before this function
     * is called.
     */
    int parse();

    /**
     * Check the ini files and in case some of them changed externally,
     * reload it.
     */
    void UpdateIfModif ();

    /**
     * Write changed ini files on disk
     */
    int write ();

    /**
     * Does a section have end-mark defined?
     * @param i index of section rule
     * @return true if yes
     */
    bool sectionNeedsEnd (int i) { return sections[i].end_valid; }

    /**
     * Get the file name of section. If there is a rewrite rule rb,
     * rewrites section name to file name using the rule rb.
     * @param sec section name
     * @param rb index of rewrite rule
     * @return rewritten file name
     */
    std::string getFileName (const std::string&sec, int rb);
    /**
     * Using file name rewriting?
     */
    bool HaveRewrites () const { return rewrites.size () > 0; }

    //! accessor method
    bool repeatNames () const { return repeat_names; }
    //! accessor method
    bool isFlat () const { return flat; }

    /**
     * change case of String
     * @param str String to change
     * @return changed String
     */
    std::string changeCase (const std::string&str) const;
};

}      // End of INI namespace
}      // End of CA_MGM_NAMESPACE

#endif//IniParser_h
