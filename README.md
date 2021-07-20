# A simple C++ URL class

### The `Url` object API
`Url` is a C++ URL handling class with a very simple API. It's use is straightforward.

* Construct an `Url` object
  ``` C++
  Url u1;
  Url u2("http://www.example.com/");
  Url u3(u2);
  ```

* Assigning an url to an `Url` object
  ``` C++
  u1=u2;
  u2="http://www.example.com/";
  ```

* Getting the url as a string
  ``` C++
  std::string url_str=url.str();
  ```

* Clearing an `Url` object
  ``` C++
  u1.clear();
  ```

* Getting the different fields of an `Url` object
  ``` C++
  std::string scheme_str=u1.scheme();
  std::string user_info_str=u1.user_info();
  std::string host_str=u1.host();
  std::uint8_t ip_version=u1.ip_version();
  // 0=name, 4=IPv4, 6=IPv6,-1=undefined
  std::string port_str=u1.port();
  std::string path_str=u1.path();
  Url::Query query=u1.query();
  std::string fragment_str=u1.fragment();
  ```

* Setting the different fields of an `Url` object
  ``` C++
  u1.clear().scheme("http").host("www.example.com").path("/");
  u1.add_query("q","some search key").add_query("fast");
  assert(u1.str()=="http://www.example.com/?q=some+search+key&fast");
  ```

* Accessing the key and values in the query
  ``` C++
  u1.clear().add_query("a","b").add_query("c","d");
  assert(u1.query(0).val()=="b");
  assert(u1.query(1).key()=="c");
  assert(u1.str()=="?a=b&c=d");
  u1.set_query(0).val("B");
  u1.set_query(1).key("C");
  assert(u1.str()=="?a=B&C=d");
  ```

* Output of an `Url` object
  ``` C++
  u1.str("http://www.example.com/?q=some+search+key&fast");
  std::stringstream str;
  str << u1;
  assert(str.str()=="Url:{url(http://www.example.com/?q=some+search+key&fast) "
         "scheme(http) host(www.example.com) IPv(0) path(/) "
         "query(<key(q) val(some search key)> <key(fast) val()>)}");
  ```

### Implementation detail

The `Url` object uses lazy URL parsing and building. When a URL as string is
assigned to the object, it is not immediatly parsed. It can then be assigned or
moved to another `Url` object efficiently. The URL is parsed only when needed,
which is before one of the fields is accessed or set, or when the object is 
output.

The URL is built only when the `str()` method is called or when the `Url` object
is output.

### Testing the library

In order to quickly build a static library and test the code, the following 
sequence of bash instructions may work for you. 

```
g++ -c url.cpp
ar rvs CxxUrl.a url.o
g++ main.cpp CxxUrl.a
./a.out
```
