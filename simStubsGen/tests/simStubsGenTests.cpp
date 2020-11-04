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

    void basic(basic_in *in, basic_out *out)
    {
        out->i = in->i;
        out->f = in->f;
        out->d = in->d;
        out->s = in->s;
        out->b = in->b;
        out->ti = in->ti;
        out->z = in->z;
        sim::addLog(sim_verbosity_debug, "basic: i=%d", in->i);
        sim::addLog(sim_verbosity_debug, "basic: f=%f", in->f);
        sim::addLog(sim_verbosity_debug, "basic: d=%f", in->d);
        sim::addLog(sim_verbosity_debug, "basic: b=%d", in->b);
        sim::addLog(sim_verbosity_debug, "basic: s=%s", in->s);
        sim::addLog(sim_verbosity_debug, "basic: ti=<%d values>", in->ti.size());
        sim::addLog(sim_verbosity_debug, "basic: z=%08x", &in->z);
    }

    void nullable(nullable_in *in, nullable_out *out)
    {
        if(in->i) out->i = in->i;
        if(in->f) out->f = in->f;
        if(in->d) out->d = in->d;
        if(in->s) out->s = in->s;
        if(in->b) out->b = in->b;
        if(in->ti) out->ti = in->ti;
        if(in->z) out->z = in->z;
        sim::addLog(sim_verbosity_debug, "nullable: i=%d", in->i);
        sim::addLog(sim_verbosity_debug, "nullable: f=%f", in->f);
        sim::addLog(sim_verbosity_debug, "nullable: d=%f", in->d);
        sim::addLog(sim_verbosity_debug, "nullable: b=%d", in->s);
        sim::addLog(sim_verbosity_debug, "nullable: s=%s", in->b);
        sim::addLog(sim_verbosity_debug, "nullable: ti=<%d values>", in->ti ? in->ti->size() : 0);
        sim::addLog(sim_verbosity_debug, "nullable: z=%08x", in->z ? &(*in->z) : nullptr);
    }

    void struct_table(struct_table_in *in, struct_table_out *out)
    {
        if(in->s == "i") out->i = in->tz.at(in->i).i;
        if(in->s == "f") out->f = in->tz.at(in->i).f;
        if(in->s == "d") out->d = in->tz.at(in->i).d;
        if(in->s == "s") out->s = in->tz.at(in->i).s;
        if(in->s == "b") out->b = in->tz.at(in->i).b;
    }
};

SIM_PLUGIN("StubsGenTests", 1, Plugin)
#include "stubsPlusPlus.cpp"