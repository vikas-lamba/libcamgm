#include <ca-mgm/String.hpp>
#include <ca-mgm/PerlRegEx.hpp>
#include <ca-mgm/LogControl.hpp>
#include <ca-mgm/PathInfo.hpp>
#include <ca-mgm/PathUtils.hpp>
#include <ca-mgm/Exception.hpp>
#include <ca-mgm/ByteBuffer.hpp>
#include <ca-mgm/CA.hpp>
#include <ca-mgm/LocalManagement.hpp>

#include <iostream>
#include <fstream>
#include <unistd.h>

#include "TestLineFormater.hpp"

using namespace ca_mgm;
using namespace std;

int main()
{
    PerlRegEx r("ENCRYPTED");

    try
    {
        cout << "START" << endl;

        // Logging
	boost::shared_ptr<LogControl::LineFormater> formater(new TestLineFormater());
        LogControl logger = LogControl::instance();
        logger.setLineFormater( formater );
        logger.setLogLevel( logger::E_INFO );
        logger.logToStdErr();

        CA ca("SUSEIPsecCA", "system", "./TestRepos4/");

        cout << "==================== ca.exportCACert(PEM); ======================" << endl;

        ByteBuffer ba = ca.exportCACert(E_PEM);

        CertificateData cd = LocalManagement::getCertificate(ba, E_PEM);

        cout << "Subject: " << cd.getSubjectDN().getOpenSSLString() << endl;

        cout << "==================== ca.exportCACert(DER); ======================" << endl;

        ba = ca.exportCACert(E_DER);

        cd = LocalManagement::getCertificate(ba, E_DER);

        cout << "Subject: " << cd.getSubjectDN().getOpenSSLString() << endl;

        cout << "==================== ca.exportCAKeyAsPEM(''); ======================" << endl;

        ba = ca.exportCAKeyAsPEM("");

        LocalManagement::writeFile(ba, "./TestRepos4/testCAKey.key");

        path::PathInfo pi("./TestRepos4/testCAKey.key");

        if(pi.exists())
        {
            cout << "Key exists" << endl;

            if(!r.match(std::string(ba.data(), ba.size())))
            {
                cout << "Key is decrypted" << endl;

                path::removeFile("./TestRepos4/testCAKey.key");
            }
            else
            {
                cout << "Key is encrypted" << endl;
            }
        }

        cout << "==================== ca.exportCAKeyAsPEM('tralla'); ======================" << endl;

        ba = ca.exportCAKeyAsPEM("tralla");

        LocalManagement::writeFile(ba, "./TestRepos4/testCAKey2.key");

        pi.stat("./TestRepos4/testCAKey2.key");

        if(pi.exists())
        {
            cout << "Key exists" << endl;

            if(!r.match(std::string(ba.data(), ba.size())))
            {
                cout << "Key is decrypted" << endl;
            }
            else
            {
                cout << "Key is encrypted" << endl;

                path::removeFile("./TestRepos4/testCAKey2.key");
            }
        }

        cout << "==================== ca.exportCAKeyAsDER(); ======================" << endl;

        ba = ca.exportCAKeyAsDER();

        LocalManagement::writeFile(ba, "./TestRepos4/testCAKeyDER.key");

        pi.stat("./TestRepos4/testCAKeyDER.key");

        if(pi.exists() && pi.size() > 1000)
        {
            cout << "Key exists" << endl;

            path::removeFile("./TestRepos4/testCAKeyDER.key");
        }

        cout << "=============== ca.exportCAasPKCS12('tralla', false); ===============" << endl;

        ba = ca.exportCAasPKCS12("tralla", false);

        LocalManagement::writeFile(ba, "./TestRepos4/testCA.p12");

        pi.stat("./TestRepos4/testCA.p12");

        if(pi.exists() && pi.size() > 2500)
        {
            cout << "Certificate exists" << endl;

            path::removeFile("./TestRepos4/testCA.p12");
        }

        cout << "================= ca.exportCAasPKCS12('tralla', true); =================" << endl;

        ba = ca.exportCAasPKCS12("tralla", true);

        LocalManagement::writeFile(ba, "./TestRepos4/testCAChain.p12");

        pi.stat("./TestRepos4/testCAChain.p12");

        if(pi.exists() && pi.size() > 4700)
        {
            cout << "Certificate exists" << endl;

            path::removeFile("./TestRepos4/testCAChain.p12");
        }

        CA ca2("SUSEUserCA", "system", "./TestRepos4/");

        cout << "==================== ca.exportCertificate(PEM); ======================" << endl;

        ba = ca2.exportCertificate("01:64758469504eccf1543930224f60315d-1423243460", E_PEM);

        cd = LocalManagement::getCertificate(ba, E_PEM);

        cout << "Subject: " << cd.getSubjectDN().getOpenSSLString() << endl;

        cout << "==================== ca.exportCertificate(DER); ======================" << endl;

        ba = ca2.exportCertificate("01:64758469504eccf1543930224f60315d-1423243460", E_DER);

        cd = LocalManagement::getCertificate(ba, E_DER);

        cout << "Subject: " << cd.getSubjectDN().getOpenSSLString() << endl;

        cout << "================== ca.exportCertificateKeyAsPEM(decr); ===================" << endl;

        ba = ca2.exportCertificateKeyAsPEM("01:64758469504eccf1543930224f60315d-1423243460",
                                           "system", "");

        LocalManagement::writeFile(ba, "./TestRepos4/testKey.key");

        pi.stat("./TestRepos4/testKey.key");

        if(pi.exists())
        {
            cout << "Key exists" << endl;

            if(!r.match(std::string(ba.data(), ba.size())))
            {
                cout << "Key is decrypted" << endl;

                path::removeFile("./TestRepos4/testKey.key");

            }
            else
            {
                cout << "Key is encrypted" << endl;
            }
        }

        cout << "============ ca.exportexportCertificateKeyAsPEM('tralla'); =============" << endl;

        ba = ca2.exportCertificateKeyAsPEM("01:64758469504eccf1543930224f60315d-1423243460",
                                           "system", "tralla");

        LocalManagement::writeFile(ba, "./TestRepos4/testKey2.key");

        pi.stat("./TestRepos4/testKey2.key");

        if(pi.exists())
        {
            cout << "Key exists" << endl;

            if(!r.match(std::string(ba.data(), ba.size())))
            {
                cout << "Key is decrypted" << endl;

            }
            else
            {
                cout << "Key is encrypted" << endl;

                path::removeFile("./TestRepos4/testKey2.key");
            }
        }

        cout << "==================== ca.exportCertificateKeyAsDER(); ======================" << endl;

        ba = ca2.exportCertificateKeyAsDER("01:64758469504eccf1543930224f60315d-1423243460",
                                           "system");

        LocalManagement::writeFile(ba, "./TestRepos4/testKeyDER.key");

        pi.stat("./TestRepos4/testKeyDER.key");

        if(pi.exists() && pi.size() > 1000)
        {
            cout << "Key exists" << endl;

            path::removeFile("./TestRepos4/testKeyDER.key");
        }

        cout << "============= ca.exportCertificateAsPKCS12(..., false); ==================" << endl;

        ba = ca2.exportCertificateAsPKCS12("01:64758469504eccf1543930224f60315d-1423243460",
                                           "system", "tralla", false);

        LocalManagement::writeFile(ba, "./TestRepos4/testCert.p12");

        pi.stat("./TestRepos4/testCert.p12");

        if(pi.exists() && pi.size() > 2500)
        {
            cout << "Certificate exists" << endl;

            path::removeFile("./TestRepos4/testCert.p12");
        }

        cout << "=============== ca.exportCertificateAsPKCS12(..., true); ================" << endl;

        ba = ca2.exportCertificateAsPKCS12("01:64758469504eccf1543930224f60315d-1423243460",
                                           "system", "tralla", true);

        LocalManagement::writeFile(ba, "./TestRepos4/testCertChain.p12");

        pi.stat("./TestRepos4/testCertChain.p12");

        if(pi.exists() && pi.size() > 4700)
        {
            cout << "Certificate exists" << endl;

            path::removeFile("./TestRepos4/testCertChain.p12");
        }

        cout << "==================== ca.exportCRL(PEM); ======================" << endl;

        ba = ca2.exportCRL(E_PEM);

        CRLData crl = LocalManagement::getCRL(ba, E_PEM);

        cout << "Issuer: " << crl.getIssuerDN().getOpenSSLString() << endl;

        cout << "==================== ca.exportCRL(DER); ======================" << endl;

        ba = ca2.exportCRL(E_DER);

        crl = LocalManagement::getCRL(ba, E_DER);

        cout << "Issuer: " << crl.getIssuerDN().getOpenSSLString() << endl;

        cout << "DONE" << endl;
    }
    catch(ca_mgm::Exception& e)
    {
        cerr << e << endl;
    }

    return 0;
}

/* vim: set ts=8 sts=8 sw=8 ai noet: */
