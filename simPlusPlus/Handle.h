#ifndef SIMPLUSPLUS_HANDLE_H_INCLUDED
#define SIMPLUSPLUS_HANDLE_H_INCLUDED

#include <string>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <simPlusPlus/Lib.h>

namespace sim
{
    /*! \brief A tool for converting pointers to strings and vice versa.
     *
     * Usage: specialize the Handle<T>::tag() method for your class, e.g.:
     *
     * template<> std::string Handle<OcTree>::tag() { return "octomap.OcTree"; }
     *
     * Note: tag must not contain ":"
     */
    template<typename T>
    struct Handle
    {
        static std::string str(const T *t)
        {
            static boost::format fmt("%s:%lld:%d");
            return (fmt % tag() % reinterpret_cast<long long int>(t) % crc_ptr(t)).str();
        }

        static T * obj(std::string h)
        {
            boost::cmatch m;
            static boost::regex re("([^:]+):([^:]+):([^:]+)");
            if(boost::regex_match(h.c_str(), m, re) && m[1] == tag())
            {
                T *t = reinterpret_cast<T*>(boost::lexical_cast<long long int>(m[2]));
                int crc = boost::lexical_cast<int>(m[3]);
                if(crc == crc_ptr(t)) return t;
            }
            return nullptr;
        }

    private:
        static std::string tag()
        {
            return "ptr";
        }

        static int crc_ptr(const T *t)
        {
            auto x = reinterpret_cast<long long int>(t);
            x = x ^ (x >> 32);
            x = x ^ (x >> 16);
            x = x ^ (x >> 8);
            x = x ^ (x >> 4);
            x = x & 0x000000000000000F;
            x = x ^ 0x0000000000000008;
            return int(x);
        }
    };

    template<typename T>
    struct Handles
    {
        std::string add(const T *t, int scriptID)
        {
            int sceneID = getSceneID(scriptID);
            handlesf[sceneID][scriptID].insert(t);
            handlesr[t][sceneID] = scriptID;
            return Handle<T>::str(t);
        }

        const T * remove(const T *t)
        {
            for(const auto &m : handlesr.at(t))
            {
                int sceneID = m.first;
                int scriptID = m.second;
                handlesf.at(sceneID).at(scriptID).erase(t);
            }
            handlesr.erase(t);
            return t;
        }

        T * get(std::string h)
        {
            T *ret = Handle<T>::obj(h);
            if(!ret)
                throw std::runtime_error("invalid object handle");
            if(handlesr.find(ret) == handlesr.end())
                throw std::runtime_error("non-existent object handle");
            return ret;
        }

        std::set<const T*> find(int scriptID)
        {
            int sceneID = getSceneID(scriptID);
            return handlesf.at(sceneID).at(scriptID);
        }

        std::set<std::string> handles()
        {
            std::set<std::string> r;
            for(const auto &x : handlesr)
                r.insert(x.first);
            return r;
        }

    private:
        static int getSceneID(int scriptID)
        {
            int scriptType, objectHandle;
            sim::getScriptProperty(scriptID, &scriptType, &objectHandle);
            if(0
                    || scriptType == sim_scripttype_mainscript
                    || scriptType == sim_scripttype_childscript
                    || scriptType == sim_scripttype_customizationscript
            )
                return sim::getInt32Parameter(sim_intparam_scene_unique_id);
            else
                return -1;
        }

        // Tables of created objects (for methods: add, remove, find)

        // sceneID -> (scriptID -> [objects])
        std::map<int, std::map<int, std::set<const T*>>> handlesf;

        // object -> (sceneID -> scriptID)
        std::map<const T*, std::map<int, int>> handlesr;
    };
}

#endif // SIMPLUSPLUS_HANDLE_H_INCLUDED
