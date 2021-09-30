#include <iostream>
#include <iterator>
#include <ctime>
#include <cassert>
#include <sstream>
#include "url.hpp"

using namespace std;

#define test_parse(str, m, expect) {\
    try {\
        Url url(str);\
        if(url.m()!=expect)\
            cout << "FAILED: ('" << str << "') -> "#m"('" << url.m() << "') (expect " << expect << ")" << endl;\
        else\
            cout << "PASSED: ('" << str << "') -> "#m"('" << url.m() << "')" << endl;\
    } catch(std::exception &e) {\
        cout << "FAILED: ('" << str << "') -> \"" << e.what() << "\" (expect " << expect << ")" << endl;\
    }\
}


#define test_valid(m1,str,m2,expect,ip_v) {\
    try {\
        Url url;\
        if(url.m1(str).m2()!=expect)\
            cout << "FAILED: "#m1"('" << str << "') -> "#m2"('" << url.m2() << "') (expect " << expect << ")" << endl;\
        else if (url.ip_version()!=ip_v)\
            cout << "FAILED: "#m1"('" << str << "') -> "#m2"('" << url.m2() << "') (IP version " << (int)url.ip_version() << " instead of " << ip_v << ")" << endl;\
        else\
            cout << "PASSED: "#m1"('" << str << "') -> "#m2"('" << url.m2() << "')" << endl;\
    } catch(std::exception &e) {\
        cout << "FAILED: "#m1"('" << str << "') -> \"" << e.what() << "\" (expect " << expect << ")" << endl;\
    }\
}

#define test_invalid(m1,str,m2,expect) {\
    try {\
        Url url;\
        std::string out=url.m1(str).m2();\
        cout << "FAILED: "#m1"('" << str << "') -> "#m2"('" << out << "') (expect \"" << expect << "\")" << endl;\
    } catch(std::exception &e) {\
        if (e.what()!=std::string(expect))\
            cout << "FAILED: "#m1"('" << str << "') -> " << e.what() << " (expect \"" << expect << "\")" << endl;\
        else\
            cout << "PASSED: "#m1"('" << str << "') -> " << e.what() << endl;\
    }\
}


void test_all_valid() {

    cout << "------------------------------------------------------" << endl;
    cout << "Test scheme" << endl;
    cout << "------------------------------------------------------" << endl;
    test_valid(scheme,"a",scheme,"a",-1);
    test_valid(scheme,"HTTP",scheme,"http",-1);
    test_valid(scheme,"HTTPs",scheme,"https",-1);
    test_valid(scheme,"X-1.0",scheme,"x-1.0",-1);
    test_valid(scheme,"X+a",scheme,"x+a",-1);
    test_invalid(scheme,"",scheme,"Invalid scheme ''");
    test_invalid(scheme,"1A",scheme,"Invalid scheme '1A'");
    test_invalid(scheme,"a/5",scheme,"Invalid scheme 'a/5'");

    cout << "------------------------------------------------------" << endl;
    cout << "Test user info" << endl;
    cout << "------------------------------------------------------" << endl;
    test_valid(user_info,"",user_info,"",-1);
    test_valid(user_info,"user:passwd",user_info,"user:passwd",-1);
    test_parse("http://user:pa%24%24wd@www.example.com",user_info,"user:pa$$wd");

    cout << "------------------------------------------------------" << endl;
    cout << "Test host" << endl;
    cout << "------------------------------------------------------" << endl;
    test_valid(host,"",host,"",-1);
    test_valid(host,"www.example.com",host,"www.example.com",0);
    test_valid(host,"!$+",host,"!$+",0);
    test_valid(host,"12.34.56.78.com",host,"12.34.56.78.com",0);
    test_invalid(host,"!#",host,"Invalid host '!#'");
    test_valid(host,"12.34.56.78",host,"12.34.56.78",4);
    test_valid(host,"0.0.0.0",host,"0.0.0.0",4);
    test_invalid(host,"12.34.56.257",host,"Invalid IPv4 address '12.34.56.257'");
    test_valid(host,"1:2:3:4:5:6:7:8",host,"1:2:3:4:5:6:7:8",6);
    test_valid(host,"1:2:3:4:5:6:9.9.9.9",host,"1:2:3:4:5:6:9.9.9.9",6);
    test_valid(host,"::9.9.9.9",host,"::9.9.9.9",6);
    test_valid(host,"::2:3:4:5:6:7:8",host,"0:2:3:4:5:6:7:8",6);
    test_valid(host,"::2:3:4:5:6:9.9.9.9",host,"0:2:3:4:5:6:9.9.9.9",6);
    test_valid(host,"1::4:5:6:7:8:9",host,"1:0:4:5:6:7:8:9",6);
    test_valid(host,"1::4:5:6:7:8",host,"1::4:5:6:7:8",6);
    test_valid(host,"1::4:5:6:9.9.9.9",host,"1::4:5:6:9.9.9.9",6);
    test_valid(host,"1:2::5:6:7:8",host,"1:2::5:6:7:8",6);
    test_valid(host,"1:2::5:6:9.9.9.9",host,"1:2::5:6:9.9.9.9",6);
    test_valid(host,"1:2::5:6:7:8",host,"1:2::5:6:7:8",6);
    test_valid(host,"1:2:3::6:9.9.9.9",host,"1:2:3::6:9.9.9.9",6);
    test_valid(host,"1:2:3:4::7:8",host,"1:2:3:4::7:8",6);
    test_valid(host,"1:2:3:4::9.9.9.9",host,"1:2:3:4::9.9.9.9",6);
    test_valid(host,"1:2:3:4:5::8",host,"1:2:3:4:5::8",6);
    test_valid(host,"1:2:3:4::9.9.9.9",host,"1:2:3:4::9.9.9.9",6);
    test_valid(host,"1:2:3:4:5::8",host,"1:2:3:4:5::8",6);
    test_valid(host,"1:2:3:4:5:6::",host,"1:2:3:4:5:6::",6);
    test_valid(host,"1:2:3:4:5:6:7::",host,"1:2:3:4:5:6:7:0",6);
    test_valid(host,"1:2:3:4:5::",host,"1:2:3:4:5::",6);
    test_valid(host,"1:2:3:4::",host,"1:2:3:4::",6);
    test_valid(host,"1:2:3::",host,"1:2:3::",6);
    test_valid(host,"1111:2222:3333:4444:5555:6666:7777:8888",host,"1111:2222:3333:4444:5555:6666:7777:8888",6);
    test_valid(host,"0111:0022:0333:4444:0555:0006:0777:8888",host,"111:22:333:4444:555:6:777:8888",6);
    test_valid(host,"::ffff:9.9.9.9",host,"::ffff:9.9.9.9",6);
    test_valid(host,"DEAD::FACE:9.9.9.9",host,"dead::face:9.9.9.9",6);
    test_valid(host,"::",host,"::",6);
    test_valid(host,"::1",host,"::1",6);
    test_valid(host,"1::",host,"1::",6);
    test_valid(host,"::6:7:8",host,"::6:7:8",6);
    test_valid(host,"1::8",host,"1::8",6);
    test_valid(host,"1:2:3:4:5:6:7:8",host,"1:2:3:4:5:6:7:8",6);
    test_valid(host,"::2:3:4:5:6:7:8",host,"0:2:3:4:5:6:7:8",6);
    test_valid(host,"1:2:3:4:5:6:7::",host,"1:2:3:4:5:6:7:0",6);
    test_valid(host,"1:2:3::5:6:7:8",host,"1:2:3:0:5:6:7:8",6);
    test_valid(host,"::3:4:5:6:7:8",host,"::3:4:5:6:7:8",6);
    test_valid(host,"0:0:3:4:5:6:7:8",host,"::3:4:5:6:7:8",6);
    test_valid(host,"0:0:3:4:0:0:7:8",host,"::3:4:0:0:7:8",6);
    test_valid(host,"0:0:3:4:0:0:0:8",host,"0:0:3:4::8",6);
    test_valid(host,"0:0:3:4:0:0::",host,"0:0:3:4::",6);
    test_valid(host,"0:0:3:4::",host,"0:0:3:4::",6);
    test_valid(host,"0:AbC:d:00ef::",host,"0:abc:d:ef::",6);
    test_valid(host,"::ffff:9.9.9.9",host,"::ffff:9.9.9.9",6);
    test_valid(host,"0:0:0::ffff:255.199.19.9",host,"::ffff:255.199.19.9",6);
    test_valid(host,"xxx",host,"xxx",0);
    test_invalid(host,"xxx::",host,"Invalid host 'xxx::'");
    test_invalid(host,"x:x:x::x:x:x",host,"Invalid host 'x:x:x::x:x:x'");
    test_invalid(host,"x:x:x::x:x:x:y.y.y.y",host,"Invalid host 'x:x:x::x:x:x:y.y.y.y'");
    test_invalid(host,"1111:2222:3333:4444:5555:6666:7777:88889",host,"Invalid IPv6 address '1111:2222:3333:4444:5555:6666:7777:88889'");
    test_invalid(host,"1:2:3:4:5:6:7:8:9",host,"Invalid IPv6 address '1:2:3:4:5:6:7:8:9'");
    test_invalid(host,":1:2:3:4:5:6:7:8",host,"Invalid IPv6 address ':1:2:3:4:5:6:7:8'");
    test_invalid(host,":1:2:3::6:7:8",host,"Invalid IPv6 address ':1:2:3::6:7:8'");
    test_invalid(host,"1:2:3:4:5:6:7:8:",host,"Invalid IPv6 address '1:2:3:4:5:6:7:8:'");
    test_invalid(host,"1:2:3:4:5:6:7:8::",host,"Invalid IPv6 address '1:2:3:4:5:6:7:8::'");
    test_invalid(host,"1:2::5:6:7:8:",host,"Invalid IPv6 address '1:2::5:6:7:8:'");
    test_invalid(host,"1:2::5:6:7:8::",host,"Invalid IPv6 address '1:2::5:6:7:8::'");
    test_invalid(host,"::3:4:5:6::",host,"Invalid IPv6 address '::3:4:5:6::'");
    test_invalid(host,"1::4::7:8",host,"Invalid IPv6 address '1::4::7:8'");
    test_invalid(host,"1:2:3:4:5:6:9.257.9.9",host,"Invalid IPv6 address '1:2:3:4:5:6:9.257.9.9'");
    test_invalid(host,"1:2:3:4:5:6:.9.9.9",host,"Invalid IPv6 address '1:2:3:4:5:6:.9.9.9'");

    cout << "------------------------------------------------------" << endl;
    cout << "Test port" << endl;
    cout << "------------------------------------------------------" << endl;
    test_valid(port,"123",port,"123",-1);
    test_valid(port,"0",port,"0",-1);
    test_valid(port,"65535",port,"65535",-1);
    test_invalid(port,"65536",port,"Invalid port '65536'");
    test_invalid(port,"12a55",port,"Invalid port '12a55'");

    cout << "------------------------------------------------------" << endl;
    cout << "Test path" << endl;
    cout << "------------------------------------------------------" << endl;
    test_valid(path,"",path,"",-1);
    test_valid(path,"a",path,"a",-1);
    test_valid(path,".",path,"",-1);
    test_valid(path,"..",path,"",-1);
    test_valid(path,"a/",path,"a/",-1);
    test_valid(path,"./",path,"",-1);
    test_valid(path,"../",path,"",-1);
    test_valid(path,"/a",path,"/a",-1);
    test_valid(path,"/.",path,"/",-1);
    test_valid(path,"/..",path,"/",-1);
    test_valid(path,"/a/",path,"/a/",-1);
    test_valid(path,"/./",path,"/",-1);
    test_valid(path,"/../",path,"/",-1);
    test_valid(path,"/a/",path,"/a/",-1);
    test_valid(path,"/./",path,"/",-1);
    test_valid(path,"/../",path,"/",-1);
    test_valid(path,"/a/a",path,"/a/a",-1);
    test_valid(path,"/./a",path,"/a",-1);
    test_valid(path,"/../a",path,"/a",-1);
    test_valid(path,"/a/.",path,"/a/",-1);
    test_valid(path,"/./.",path,"/",-1);
    test_valid(path,"/../.",path,"/",-1);
    test_valid(path,"/a/..",path,"/",-1);
    test_valid(path,"/./..",path,"/",-1);
    test_valid(path,"/../..",path,"/",-1);
    test_valid(path,"/a/a/",path,"/a/a/",-1);
    test_valid(path,"/./a/",path,"/a/",-1);
    test_valid(path,"/../a/",path,"/a/",-1);
    test_valid(path,"/a/./",path,"/a/",-1);
    test_valid(path,"/././",path,"/",-1);
    test_valid(path,"/.././",path,"/",-1);
    test_valid(path,"/a/../",path,"/",-1);
    test_valid(path,"/./../",path,"/",-1);
    test_valid(path,"/../../",path,"/",-1);
    test_valid(path,"a/a",path,"a/a",-1);
    test_valid(path,"./a",path,"a",-1);
    test_valid(path,"../a",path,"a",-1);
    test_valid(path,"a/.",path,"a/",-1);
    test_valid(path,"./.",path,"",-1);
    test_valid(path,"../.",path,"",-1);
    test_valid(path,"a/..",path,"",-1);
    test_valid(path,"./..",path,"",-1);
    test_valid(path,"../..",path,"",-1);
    test_valid(path,"a/a/",path,"a/a/",-1);
    test_valid(path,"./a/",path,"a/",-1);
    test_valid(path,"../a/",path,"a/",-1);
    test_valid(path,"a/./",path,"a/",-1);
    test_valid(path,"././",path,"",-1);
    test_valid(path,".././",path,"",-1);
    test_valid(path,"a/../",path,"",-1);
    test_valid(path,"./../",path,"",-1);
    test_valid(path,"../../",path,"",-1);
    test_valid(path,"a/./a",path,"a/a",-1);
    test_valid(path,"a/./..",path,"",-1);
    test_valid(path,"ab cd",path,"ab cd",-1);

    cout << "------------------------------------------------------" << endl;
    cout << "Test fragment" << endl;
    cout << "------------------------------------------------------" << endl;
    test_valid(fragment,"xxx",fragment,"xxx",-1);
    test_valid(fragment,"ab cd",fragment,"ab cd",-1);

    cout << "------------------------------------------------------" << endl;
    cout << "Test url" << endl;
    cout << "------------------------------------------------------" << endl;
    test_valid(str,"",str,"",-1);
    test_valid(str,"http:",str,"http:",-1);
    test_valid(str,"http:",scheme,"http",-1);
    test_valid(scheme,"http",str,"http:",-1);
    test_valid(str,"test/path",str,"test/path",-1);
    test_valid(str,"test/path/..",str,"test",-1);
    test_valid(str,"test/path/../..",str,"",-1);
    test_valid(str,"test/path/../../.",str,"",-1);
    test_valid(str,"/test/path/../../",str,"/",-1);
    test_valid(str,"/test/path/../../.",str,"/",-1);
    test_valid(str,"test/path/.././..",str,"",-1);
    test_valid(str,"test/path/../../..",str,"",-1);
    test_invalid(str,"test path",str,"Path 'test path' in 'test path' is invalid");
    test_valid(str,"test+path",str,"test+path",-1);
    test_valid(str,"test%2bpath",str,"test+path",-1);
    test_valid(str,"test%20path",str,"test%20path",-1);
    test_valid(str,"test%2Fpath",str,"test/path",-1);
    test_valid(str,"/test+path",str,"/test+path",-1);
    test_valid(str,"/test%2bpath",str,"/test+path",-1);
    test_valid(str,"/test%20path",str,"/test%20path",-1);
    test_valid(str,"/test%2Fpath",str,"/test/path",-1);
    test_valid(str,"%2Ftest+path%2F",str,"/test+path/",-1);
    test_valid(str,"%2Ftest%2bpath%2F",str,"/test+path/",-1);
    test_valid(str,"%2Ftest%20path%2F",str,"/test%20path/",-1);
    test_valid(str,"%2Ftest%2Fpath%2F",str,"/test/path/",-1);
    test_valid(str,"",path,"",-1);
    test_valid(str,"test/path",path,"test/path",-1);
    test_valid(str,"test/path/..",path,"test",-1);
    test_valid(str,"test/path/../..",path,"",-1);
    test_valid(str,"test/path/../../.",path,"",-1);
    test_valid(str,"/test/path/../../",path,"/",-1);
    test_valid(str,"/test/path/../../.",path,"/",-1);
    test_valid(str,"test/path/.././..",path,"",-1);
    test_valid(str,"test/path/../../..",path,"",-1);
    test_valid(str,"test+path",path,"test+path",-1);
    test_valid(str,"test%2bpath",path,"test+path",-1);
    test_valid(str,"test%20path",path,"test path",-1);
    test_valid(str,"test%2Fpath",path,"test/path",-1);
    test_valid(str,"/test+path",path,"/test+path",-1);
    test_valid(str,"/test%2bpath",path,"/test+path",-1);
    test_valid(str,"/test%20path",path,"/test path",-1);
    test_valid(str,"/test%2Fpath",path,"/test/path",-1);
    test_valid(str,"%2Ftest+path%2F",path,"/test+path/",-1);
    test_valid(str,"%2Ftest%2bpath%2F",path,"/test+path/",-1);
    test_valid(str,"%2Ftest%20path%2F",path,"/test path/",-1);
    test_valid(str,"%2Ftest%2Fpath%2F",path,"/test/path/",-1);
    test_valid(path,"test/path",str,"test/path",-1);
    test_valid(path,"test/path/..",str,"test",-1);
    test_valid(path,"test/path/../..",str,"",-1);
    test_valid(path,"test/path/../../.",str,"",-1);
    test_valid(path,"/test/path/../../",str,"/",-1);
    test_valid(path,"/test/path/../../.",str,"/",-1);
    test_valid(path,"test/path/.././..",str,"",-1);
    test_valid(path,"test/path/../../..",str,"",-1);
    test_valid(path,"test path",str,"test%20path",-1);
    test_valid(path,"test+path",str,"test+path",-1);
    test_valid(path,"test%2bpath",str,"test%252bpath",-1);
    test_valid(path,"test%20path",str,"test%2520path",-1);
    test_valid(path,"test%2Fpath",str,"test%252Fpath",-1);
    test_valid(path,"/test+path",str,"/test+path",-1);
    test_valid(path,"/test%2bpath",str,"/test%252bpath",-1);
    test_valid(path,"/test%20path",str,"/test%2520path",-1);
    test_valid(path,"/test%2Fpath",str,"/test%252Fpath",-1);
    test_valid(path,"%2Ftest+path%2F",str,"%252Ftest+path%252F",-1);
    test_valid(path,"%2Ftest%2bpath%2F",str,"%252Ftest%252bpath%252F",-1);
    test_valid(path,"%2Ftest%20path%2F",str,"%252Ftest%2520path%252F",-1);
    test_valid(path,"%2Ftest%2Fpath%2F",str,"%252Ftest%252Fpath%252F",-1);
    test_valid(path,"test=path",str,"test=path",-1);
    test_valid(str,"test=path",path,"test=path",-1);
    test_valid(str,"http:test/path",scheme,"http",-1);
    test_valid(str,"http:test/path",host,"",-1);
    test_valid(str,"http:test/path",path,"test/path",-1);
    test_valid(str,"http:test/path/..",str,"http:test",-1);
    test_valid(str,"http:test/path/../..",str,"http:",-1);
    test_valid(str,"http:test/path/../../.",str,"http:",-1);
    test_valid(str,"http:/test/path/../../",str,"http:/",-1);
    test_valid(str,"http:/test/path/../../.",str,"http:/",-1);
    test_valid(str,"http:test/path/.././..",str,"http:",-1);
    test_valid(str,"http:test/path/../../..",str,"http:",-1);
    test_invalid(str,"http:test path",str,"Path 'test path' in 'http:test path' is invalid");
    test_valid(str,"http:test+path",str,"http:test+path",-1);
    test_valid(str,"http:test%2bpath",str,"http:test+path",-1);
    test_valid(str,"http:test%20path",str,"http:test%20path",-1);
    test_valid(str,"http:test%2Fpath",str,"http:test/path",-1);
    test_valid(str,"http:/test+path",str,"http:/test+path",-1);
    test_valid(str,"http:/test%2bpath",str,"http:/test+path",-1);
    test_valid(str,"http:/test%20path",str,"http:/test%20path",-1);
    test_valid(str,"http:/test%2Fpath",str,"http:/test/path",-1);
    test_valid(str,"http:%2Ftest+path%2F",str,"http:/test+path/",-1);
    test_valid(str,"http:%2Ftest%2bpath%2F",str,"http:/test+path/",-1);
    test_valid(str,"http:%2Ftest%20path%2F",str,"http:/test%20path/",-1);
    test_valid(str,"http:%2Ftest%20path%2F",path,"/test path/",-1);
    test_valid(str,"http:%2Ftest%2Fpath%2F",str,"http:/test/path/",-1);
    test_valid(str,"?",str,"",-1);
    test_valid(str,"?#",str,"",-1);
    test_valid(str,"?ab",str,"?ab",-1);
    test_valid(str,"?ab%20cd",str,"?ab+cd",-1);
    test_valid(str,"?ab+cd",str,"?ab+cd",-1);
    test_valid(str,"?ab%2bcd",str,"?ab%2Bcd",-1);
    test_valid(str,"?a=b;c=d",str,"?a=b&c=d",-1);
    test_valid(str,"?a=b&c=d",str,"?a=b&c=d",-1);
    test_valid(str,"?a=b?c=d",str,"?a=b?c=d",-1);
    test_valid(str,"?a?b=c!d;q=http://test/path?m=n#xxx",str,"?a?b=c!d&q=http://test/path?m=n#xxx",-1);
    test_valid(str,"?a=b;c=d#xxx",str,"?a=b&c=d#xxx",-1);
    test_valid(str,"?a=b;c=d#xxx",fragment,"xxx",-1);
    test_valid(str,"?a=b;c=d#xx/?x",str,"?a=b&c=d#xx/?x",-1);
    test_valid(str,"?a=b;c=d#xx/?x",fragment,"xx/?x",-1);
    test_valid(str,"?a=b;c=d",str,"?a=b&c=d",-1);
    test_valid(str,"?a=b;c=d#xxx",str,"?a=b&c=d#xxx",-1);
    test_valid(str,"?a=b;c=d#xx/?x",str,"?a=b&c=d#xx/?x",-1);
    test_valid(str,"#xxx",str,"#xxx",-1);
    test_valid(str,"#xxx",fragment,"xxx",-1);
    test_valid(fragment,"xxx",str,"#xxx",-1);
    test_valid(fragment,"#xxx",str,"#%23xxx",-1);
    test_valid(str,"http://www.example.net/test/",str,"http://www.example.net/test/",0);
    test_valid(str,"http://www.example.net:343/test/",str,"http://www.example.net:343/test/",0);
    test_valid(str,"http://www.example.net/",str,"http://www.example.net/",0);
    test_valid(str,"http://www.example.net",str,"http://www.example.net",0);
    test_valid(str,"http://www.example.net:343",str,"http://www.example.net:343",0);
    test_valid(str,"http://tutu@www.example.net:343",str,"http://tutu@www.example.net:343",0);
    test_valid(str,"http://toto:pwd@www.example.net:343",str,"http://toto:pwd@www.example.net:343",0);
    test_valid(str,"http://www.example.net/test",str,"http://www.example.net/test",0);
    test_valid(str,"http://www.example.net/test?aa",str,"http://www.example.net/test?aa",0);
    test_valid(str,"//www.example.net/test?aa",str,"//www.example.net/test?aa",0);
    test_valid(str,"/test/",str,"/test/",-1);
    test_valid(str,"img.jpg",str,"img.jpg",-1);
    test_valid(str,"/test/?a=b",str,"/test/?a=b",-1);
    test_valid(str,"/test%2F%2E?a=b%2E&c%20#f%20f",str,"/test/?a=b.&c+#f%20f",-1);
    test_valid(str,"HTTP://www.example.net?a=b#f",str,"http://www.example.net?a=b#f",0);
    test_valid(str,"?a.?/@a%3db=c",str,"?a.?/@a%3Db=c",-1);
    test_valid(str,"?a.?/@a%3db=c",query(0).key,"a.?/@a=b",-1);
    test_valid(str,"?a.?/@a%3db=c",query(0).val,"c",-1);
    test_valid(str,"?a=b;cd+ef",query(1).key,"cd ef",-1);
    test_valid(str,"?a=b;cd+ef",query(1).val,"",-1);
    test_invalid(str,"?a=b;cd+ef",query(2).key,"Invalid Url query index (2)");

    test_valid(str,"http://bavaria.com",str,"http://bavaria.com",0);
    test_valid(str,"http://1.2.3.4",str,"http://1.2.3.4",4);
    test_valid(str,"http://[1:2:3:4:5:6:7:8]",str,"http://[1:2:3:4:5:6:7:8]",6);
    test_valid(str,"http://[v7.1:2:3:4:5:6:7:8]",str,"http://[v7.1:2:3:4:5:6:7:8]",7);
    test_invalid(str,"http://[v7.1:2:3:4:5:6:7:8]:",str,"Port '' in 'http://[v7.1:2:3:4:5:6:7:8]:' is invalid");
    test_valid(str,"http://bavaria.com:12",str,"http://bavaria.com:12",0);
    test_valid(str,"http://1.2.3.4:12",str,"http://1.2.3.4:12",4);
    test_valid(str,"http://[1:2:3:4:5:6:7:8]:0",str,"http://[1:2:3:4:5:6:7:8]:0",6);
}


int main()
{
    cout << "Starting..." << endl;
    clock_t start = clock();

    test_all_valid();

    cout << endl << endl << "Usage Examples..." << endl;

    Url u("http://www.example.com");
    cout << u << endl;

    u.path("/path with spaces").add_query("q","some search key").add_query("fast");
    cout << u << endl;
    cout << u.str() << endl;

    u.host("").host("test");
    cout << u << endl;
    cout << u.str() << endl;

    u.path("").path("path").host("");
    cout << u << endl;
    cout << u.str() << endl;


    Url u1, u2, u3;
    u2.scheme("http");
    cout << u2.str() << endl;

    u1.clear().scheme("http").host("www.example.com").path("/");
    u1.add_query("q","some search key").add_query("fast");
    assert(u1.str()=="http://www.example.com/?q=some+search+key&fast");

    u1.clear().add_query("a","b").add_query("c","d");
    assert(u1.query(0).val()=="b");
    assert(u1.query(1).key()=="c");
    cout << u1.str() << endl;
    assert(u1.str()=="?a=b&c=d");
    u1.set_query(0).val("B");
    u1.set_query(1).key("C");
    assert(u1.str()=="?a=B&C=d");

    u1.str("http://www.example.com/?q=some+search+key&fast");
    std::stringstream str;
    str << u1;
    cout << u1 << endl;
    assert(str.str()=="Url:{url(http://www.example.com/?q=some+search+key&fast) "
           "scheme(http) host(www.example.com) IPv(0) path(/) "
           "query(<key(q) val(some search key)> <key(fast) val()>)}");

    u1.clear();
    u1.str("http://www.example.com/?q=some+search+key&fast");
    u3=u1;
    Url u4(std::move(u1));
    u2=std::move(u3);
    cout << u2.str() << endl;

    u1.str("http://bavaria.com");
    cout << u1.str() << endl;
    assert(u1.str()=="http://bavaria.com");

    u1.str("http://[::]");
    cout << u1.str() << endl;
    assert(u1.str()=="http://[::]");


    cout << "processing time: " <<  double(clock() - start) / CLOCKS_PER_SEC << endl;
    return 0;
}

