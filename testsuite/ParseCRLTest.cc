#include <ca-mgm/String.hpp>
#include <ca-mgm/PerlRegEx.hpp>
#include <ca-mgm/LogControl.hpp>
#include <ca-mgm/CA.hpp>
#include <ca-mgm/Exception.hpp>

#include <iostream>
#include <fstream>
#include <unistd.h>

// FIXME: need to be removed
#include <Utils.hpp>

#include "TestLineFormater.hpp"

using namespace ca_mgm;
using namespace std;

int main(int argc, char **argv)
{

    if ( argc != 2 )
    {
        cerr << "Usage: ParseCRLTest <filepath>" << endl;
        exit( 1 );
    }

    // Logging
    boost::shared_ptr<LogControl::LineFormater> formater(new TestLineFormater());
    LogControl logger = LogControl::instance();
    logger.setLineFormater( formater );
    logger.setLogLevel( logger::E_DEBUG );
    logger.logToStdErr();

    std::string file = argv[ 1 ];

    cout << "START" << endl;
    cout << "file: " << file << endl;

    ifstream in( file.c_str() );
    if ( in.fail() )
    {
        cerr << "Unable to load '" << file << "'" << endl;
        exit( 2 );
    }

    while( in )
    {
        try
        {
            std::string     line = str::getline( in );
            if(line == "EOF" || line.empty()) break;

            std::vector<std::string> params = PerlRegEx("\\s").split(line);
            if(params.size() != 2) break;

            CA ca(params[1], "system", params[0]);

            cout << "Parse CRL in " << params[0] << "/" << params[1] << endl;

            CRLData crl = ca.getCRL();

            cout << "got the data" << endl;

            std::vector<std::string> ret = crl.dump();

            cout << "dump the data" << endl;

            std::vector<std::string>::const_iterator it;

            for(it = ret.begin(); it != ret.end(); ++it)
            {
                cout << (*it) << endl;
            }

            cout << "=================== call verify ======================" << endl;

            ret = crl.verify();

            for(it  = ret.begin(); it != ret.end(); ++it)
            {
                cout << "> " << (*it) << endl;
            }

            cout << crl.getCRLAsText() << endl;
            cout << crl.getExtensionsAsText() << endl;
        }
        catch(ca_mgm::Exception& e)
        {
            cerr << e << endl;
        }
    }

    cout << "DONE" << endl;
    return 0;
}

/* vim: set ts=8 sts=8 sw=8 ai noet: */
