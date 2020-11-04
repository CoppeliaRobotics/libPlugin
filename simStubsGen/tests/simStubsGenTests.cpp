#include "simPlusPlus/Plugin.h"
#include "stubs.h"
#include <cmath>
#include <boost/optional/optional_io.hpp>

class Plugin : public sim::Plugin
{
public:
    int verbosityFromString(const std::string &s)
    {
        if(s == "none")
            return sim_verbosity_none;
        if(s == "errors")
            return sim_verbosity_errors;
        if(s == "warnings")
            return sim_verbosity_warnings;
        if(s == "loadinfos")
            return sim_verbosity_loadinfos;
        if(s == "questions")
            return sim_verbosity_questions;
        if(s == "scripterrors")
            return sim_verbosity_scripterrors;
        if(s == "scriptwarnings")
            return sim_verbosity_scriptwarnings;
        if(s == "scriptinfos")
            return sim_verbosity_scriptinfos;
        if(s == "msgs")
            return sim_verbosity_msgs;
        if(s == "infos")
            return sim_verbosity_infos;
        if(s == "debug")
            return sim_verbosity_debug;
        if(s == "trace")
            return sim_verbosity_trace;
        if(s == "tracelua")
            return sim_verbosity_tracelua;
        if(s == "traceall")
            return sim_verbosity_traceall;
        if(s == "default")
            return sim_verbosity_default;
        return sim_verbosity_useglobal;
    }

    void onStart()
    {
        if(!registerScriptStuff())
            throw std::runtime_error("failed to register script stuff");

        setExtVersion("simStubsGen tests");

#if 0
        auto v = sim::getStringNamedParam("simStubsGenTests.verbosity");
        if(v)
            sim::setModuleInfo(sim_moduleinfo_verbosity, v ? verbosityFromString(*v) : sim_verbosity_infos);
#endif
    }

    void onFirstInstancePass(const sim::InstancePassFlags &flags)
    {
#if 1
        auto v = sim::getStringNamedParam("simStubsGenTests.verbosity");
        if(v)
            sim::setModuleInfo(sim_moduleinfo_verbosity, v ? verbosityFromString(*v) : sim_verbosity_infos);
#endif
    }
};

SIM_PLUGIN("StubsGenTests", 1, Plugin)
#include "stubsPlusPlus.cpp"
