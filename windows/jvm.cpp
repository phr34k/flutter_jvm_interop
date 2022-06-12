//#include <windows.h>
#include <jni.h>
#include <string>

typedef jint(JNICALL *CreateJavaVM)(JavaVM **, void **, void *);    

class clr
{
public:
    struct container
    {
        JavaVM *jvm; //Pointer to the JVM (Java Virtual Machine)
        JNIEnv *env; //Pointer to native interface
        std::string logDirectory;            
        CreateJavaVM createJavaVM;
    };

    virtual void start(container*& s, const char* logDirectory, const char* assemblyName, const char* className);
    virtual void stop(container* s);
    virtual bool setProperty(container* s, const char* propertyName, const char* propertyValue);
    virtual bool ready(container* s);
    virtual std::string get_version(container* s);
};



#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <windows.h>



void clr::start(clr::container*& s, const char* logDirectory, const char* assemblyName, const char* className)
{
    std::string s_jreHome = assemblyName;
    std::string jvmDLLPath = s_jreHome + "\\bin\\server\\jvm.dll";
    std::string _binPath = s_jreHome + "\\bin\\";
    std::string _serverPath = s_jreHome + "\\bin\\server\\";
    std::wstring binPath( _binPath.begin(), _binPath.end() );
    std::wstring serverPath( _serverPath.begin(), _serverPath.end() );    
    AddDllDirectory(binPath.c_str());
    AddDllDirectory(serverPath.c_str());
    printf("Loading JVM library from %s\n", jvmDLLPath.c_str());
    HINSTANCE jvmDll = LoadLibraryExA(jvmDLLPath.c_str(), NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    if (jvmDll == nullptr) {
        DWORD lastErrorCode = GetLastError();
        if (lastErrorCode == 126) {
            std::cout << "Failed to load jvm.dll." << std::endl;
        }
        printf("Error: %d\n", lastErrorCode);        
    }
 
    if (jvmDll != nullptr) 
    {
        //s->createJavaVM = (CreateJavaVM)GetProcAddress(jvmDll, "JNI_CreateJavaVM");


        std::string classPaths = "-Djava.class.path=";
        classPaths.append(className);


        s = new clr::container();
        s->logDirectory = logDirectory;
        //================== prepare loading of Java VM ============================
        JavaVMInitArgs vm_args;                        // Initialization arguments
        JavaVMOption* options = new JavaVMOption[3];   // JVM invocation options
        options[0].optionString = (char*)classPaths.c_str();   // where to find java .class

        //options[0].optionString = (char *) "-Djava.class.path=";
        options[1].optionString = (char *) "-Djava.library.path=";
        options[2].optionString = (char *) "-verbose:jni";

        vm_args.version = JNI_VERSION_10;             // minimum Java version
        vm_args.nOptions = 3;                          // number of options
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
        //=============== load and initialize Java VM and JNI interface =============
        jint res = JNI_CreateJavaVM(&s->jvm, (void**)&s->env, &vm_args);  // YES !!
        delete options;    // we then no longer 

        if (res < 0)
        {
            return;
        } 


        jclass    mainClass; 
        //jmethodID classConstructor; 
        //jobject   classObject;  
        jmethodID methodid;

        mainClass = s->env->FindClass("main");    
        if (mainClass==0) 
        {
            printf("Failed to find class\n");
            return;
        }

        /*
        classConstructor = s->env->GetMethodID(mainClass, "<init>", "()V"); 
        if (classConstructor==0) 
        {
            printf("Failed to find class constructor\n");
            return;
        }

        classObject = s->env->NewObject(mainClass, classConstructor); 
        if (classObject==0) 
        {
            printf("Failed to instantiate new object\n");
            return;
        }
        */    

        methodid = s->env->GetStaticMethodID(mainClass, "main", "()V");
        if (methodid==0) 
        {
            printf("Failed run method\n");
            return;
        }

        s->env->CallStaticVoidMethod (mainClass, methodid);
    }
}

void clr::stop(clr::container*s)
{
    printf("stop java vm\n");
    s->jvm->DestroyJavaVM();
    delete s;
}
    
bool clr::setProperty(container* s, const char* propertyName, const char* propertyValue)
{
    /*
    BOOL result = false;
    if( s->appDomainManager != NULL )
    {    
        HRESULT hr = s->appDomainManager->SetProperty(propertyName, propertyValue, &result);
        if (hr == S_OK)
            return result == 0 ? false : true;
        else
            return false;
    }
    else
    {
        return false;
    }
    */

    printf("setProperty\n");
    return false;
}


bool clr::ready(container* s)
{
    /*
    BOOL result = false;
    if( s->appDomainManager != NULL )
    {        
        HRESULT hr = s->appDomainManager->WaitForReady(&result);
        if (hr == S_OK)
            return result == 0 ? false : true;
        else
            return false;
    }
    else
    {
        return false;
    } 
    */

    printf("ready\n");
    return true;       
}

std::string clr::get_version(container* s)
{
    std::cout << "JVM load succeeded: Version ";
    jint ver = s->env->GetVersion();
    std::cout << ((ver>>16)&0x0f) << "."<<(ver&0x0f) << std::endl;      

    std::stringstream ss;
    ss  << ((ver>>16)&0x0f) << "."<<(ver&0x0f) << std::endl;
    return ss.str();
}

