#include "v8_base.h"

CV8Initializer* CV8Worker::m_pInitializer = NULL;

namespace NSJSBase
{

    class CCacheDataScript
    {
    private:
        BYTE* Data;
        int Length;

        v8::ScriptCompiler::Source* Source;
        v8::ScriptCompiler::CachedData* CachedData;

        std::wstring Path;

    public:
        CCacheDataScript(const std::wstring& sPath)
        {
            Data = NULL;
            Length = 0;

            if (!sPath.empty())
            {
                BYTE* _data = NULL;
                DWORD _data_length = 0;
                if (NSFile::CFileBinary::ReadAllBytes(sPath, &_data, _data_length))
                {
                    Data = _data;
                    Length = (int)_data_length;
                }
            }

            Source      = NULL;
            CachedData  = NULL;
            Path        = sPath;
        }
        ~CCacheDataScript()
        {
            //RELEASEOBJECT(Source);
            //RELEASEOBJECT(CachedData);
            RELEASEARRAYOBJECTS(Data);
        }

        v8::Local<v8::Script> Compile(const v8::Local<v8::Context>& _context, const v8::Local<v8::String>& source)
            {
                v8::Local<v8::Script> script;
                if (NULL == Data)
                {
                    Source = new v8::ScriptCompiler::Source(source);
                    script = v8::ScriptCompiler::Compile(_context, Source, v8::ScriptCompiler::kProduceCodeCache).ToLocalChecked();

                    const v8::ScriptCompiler::CachedData* _cachedData = Source->GetCachedData();
                    NSFile::CFileBinary oFileTest;
                    if (oFileTest.CreateFileW(Path))
                    {
                        oFileTest.WriteFile(_cachedData->data, (DWORD)_cachedData->length);
                        oFileTest.CloseFile();
                    }
                }
                else
                {
                    CachedData = new v8::ScriptCompiler::CachedData(Data, Length);
                    Source = new v8::ScriptCompiler::Source(source, CachedData);
                    script = v8::ScriptCompiler::Compile(_context, Source, v8::ScriptCompiler::kConsumeCodeCache).ToLocalChecked();
                }
                return script;
            }

        bool IsInit()
        {
            return Data != NULL && Length > 0;
        }
    };

    class CJSIsolateScopeV8 : public CJSIsolateScope
    {
    public:
        v8::Isolate::Scope  isolate_scope;
        v8::Locker          isolate_locker;

    public:
        CJSIsolateScopeV8(v8::Isolate* isolate) : CJSIsolateScope(),
            isolate_scope(isolate),
            isolate_locker(isolate)
        {
        }
        virtual ~CJSIsolateScopeV8()
        {
        }
    };

    class CJSContextScopeV8 : public CJSContextScope
    {
    public:
        v8::Context::Scope m_scope;

    public:
        CJSContextScopeV8(v8::Local<v8::Context> context) : m_scope(context)
        {
        }
        virtual ~CJSContextScopeV8()
        {
        }
    };

    class CJSLocalScopeV8 : public CJSLocalScope
    {
    public:
        v8::HandleScope m_scope;

    public:
        CJSLocalScopeV8() : m_scope(CV8Worker::GetCurrent())
        {
        }
        virtual ~CJSLocalScopeV8()
        {
        }
    };


    CJSContext::CJSContext()
    {
        m_internal = new CJSContextPrivate();
    }
    CJSContext::~CJSContext()
    {
        RELEASEOBJECT(m_internal);
    }

    CJSTryCatch* CJSContext::GetExceptions()
    {
        return new CV8TryCatch();
    }

    void CJSContext::Initialize()
    {
        CV8Worker::Initialize();
        m_internal->m_isolate = CV8Worker::getInitializer()->CreateNew();
    }
    void CJSContext::Dispose()
    {
        m_internal->m_isolate->Dispose();
        CV8Worker::Dispose();
    }

    void CJSContext::CreateContext()
    {
        m_internal->m_context = v8::Context::New(CV8Worker::GetCurrent(), NULL, m_internal->m_global);
    }

    void CJSContext::CreateGlobalForContext()
    {
        m_internal->m_global = v8::ObjectTemplate::New(CV8Worker::GetCurrent());
    }

    CJSObject* CJSContext::GetGlobal()
    {
        CJSObjectV8* ret = new CJSObjectV8();
        ret->value = m_internal->m_context->Global();
        return ret;
    }

    CJSIsolateScope* CJSContext::CreateIsolateScope()
    {
        return new CJSIsolateScopeV8(m_internal->m_isolate);
    }

    CJSContextScope* CJSContext::CreateContextScope()
    {
        return new CJSContextScopeV8(m_internal->m_context);
    }

    CJSLocalScope* CJSContext::CreateLocalScope()
    {
        return new CJSLocalScopeV8();
    }

    CJSValue* CJSContext::createUndefined()
    {
        CJSValueV8* _value = new CJSValueV8();
        _value->doUndefined();
        return _value;
    }

    CJSValue* CJSContext::createNull()
    {
        CJSValueV8* _value = new CJSValueV8();
        _value->doNull();
        return _value;
    }

    CJSValue* CJSContext::createBool(const bool& value)
    {
        CJSValueV8* _value = new CJSValueV8();
        _value->value = v8::Boolean::New(CV8Worker::GetCurrent(), value);
        return _value;
    }

    CJSValue* CJSContext::createInt(const int& value)
    {
        CJSValueV8* _value = new CJSValueV8();
        _value->value = v8::Integer::New(CV8Worker::GetCurrent(), value);
        return _value;
    }

    CJSValue* CJSContext::createDouble(const double& value)
    {
        CJSValueV8* _value = new CJSValueV8();
        _value->value = v8::Number::New(CV8Worker::GetCurrent(), value);
        return _value;
    }

    CJSValue* CJSContext::createString(const char* value, const int& length)
    {
        CJSValueV8* _value = new CJSValueV8();
        _value->value = v8::String::NewFromUtf8(CV8Worker::GetCurrent(), value, v8::String::kNormalString, length);
        return _value;
    }

    CJSValue* CJSContext::createString(const std::string& value)
    {
        CJSValueV8* _value = new CJSValueV8();
        _value->value = v8::String::NewFromUtf8(CV8Worker::GetCurrent(), value.c_str(), v8::String::kNormalString, (int)value.length());
        return _value;
    }

    CJSValue* CJSContext::createString(const std::wstring& value)
    {
        std::string sReturn = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(value);
        return createString(sReturn);
    }

    CJSObject* CJSContext::createObject()
    {
        CJSObjectV8* _value = new CJSObjectV8();
        _value->value = v8::Object::New(CV8Worker::GetCurrent());
        return _value;
    }

    CJSArray* CJSContext::createArray(const int& count)
    {
        CJSArrayV8* _value = new CJSArrayV8();
        _value->value = v8::Array::New(CV8Worker::GetCurrent(), count);
        _value->m_count = count;
        return _value;
    }

    CJSTypedArray* CJSContext::createUint8Array(BYTE* data, int count)
    {
        CJSTypedArrayV8* _value = new CJSTypedArrayV8(data, count);
        return _value;
    }

    JSSmart<CJSValue> CJSContext::runScript(const std::string& script, JSSmart<CJSTryCatch> exception, const std::wstring& scriptPath)
    {
        v8::Local<v8::String> _source = v8::String::NewFromUtf8(CV8Worker::GetCurrent(), script.c_str());
        
        v8::Local<v8::Script> _script;
        if(scriptPath.length() > 0)
        {
            std::wstring sCachePath = scriptPath.substr(0, scriptPath.find(L".")) + L".cache";
            CCacheDataScript oCachedScript(sCachePath);
            //            if (!oCachedScript.IsInit())
            //            {
            //                _script = v8::Script::Compile(_source);
            //            }
            //            else
            //            {
            _script = oCachedScript.Compile(m_internal->m_context, _source);
            //            }
        }
        else
        {
            _script = v8::Script::Compile(_source);
        }
        
        CJSValueV8* _return = new CJSValueV8();
        
        if (exception.is_init())
        {
            if (!exception->Check())
                _return->value = _script->Run();
        }
        else
        {
            _return->value = _script->Run();
        }
        
        return _return;
    }

    CJSContext* CJSContext::GetCurrent()
    {
        CJSContext* ret = new CJSContext();
        ret->m_internal->m_isolate = CV8Worker::GetCurrent();
        ret->m_internal->m_context = ret->m_internal->m_isolate->GetCurrentContext();
        // global???
        return ret;
    }
}