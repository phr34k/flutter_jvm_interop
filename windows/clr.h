#include <string>

class clr
{
public:
    struct container;
    virtual void start(container*& s, const char* logDirectory, const char* assemblyName, const char* className);
    virtual void stop(container* s);
    virtual bool setProperty(container* s, const char* propertyName, const char* propertyValue);
    virtual bool ready(container* s);    
    virtual std::string get_version(container* s);        
};
