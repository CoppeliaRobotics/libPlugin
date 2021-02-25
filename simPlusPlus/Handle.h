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

        static T * get(std::string h)
        {
            T *ret = obj(h);
            if(!ret)
                throw std::runtime_error("invalid object handle");
            return ret;
        }

        static std::string add(const T *t, int scriptID)
        {
            int sceneID = getSceneID(scriptID);
            handles[sceneID][scriptID].insert(t);
            handlesr[t][sceneID] = scriptID;
            return str(t);
        }

        static const T * remove(const T *t)
        {
            for(const auto &m : handlesr.at(t))
            {
                int sceneID = m.first;
                int scriptID = m.second;
                handles.at(sceneID).at(scriptID).remove(t);
            }
            handlesr.erase(t);
            return t;
        }

        static std::set<const T*> find(int scriptID)
        {
            int sceneID = getSceneID(scriptID);
            return handles.at(sceneID).at(scriptID);
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

        // Tables of created objects (for methods: add, remove, find)

        // sceneID -> (scriptID -> [objects])
        static std::map<int, std::map<int, std::set<const T*>>> handles;

        // object -> (sceneID -> scriptID)
        static std::map<const T*, std::map<int, int>> handlesr;
    };
}

#endif // SIMPLUSPLUS_HANDLE_H_INCLUDED
