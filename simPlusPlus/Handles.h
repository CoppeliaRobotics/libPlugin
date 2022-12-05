#ifndef SIMPLUSPLUS_HANDLES_H_INCLUDED
#define SIMPLUSPLUS_HANDLES_H_INCLUDED

#include <string>
#include <map>
#include <set>
#include <sstream>

#include <simPlusPlus/Lib.h>

namespace sim
{
    /*! \brief A tool for converting pointers to strings and vice versa.
     */
    template<typename T>
    struct Handles
    {
        Handles() : typeStr("ptr") {}

        Handles(const std::string &s) : typeStr(s) {}

        template<typename U>
        void * toPtr(U* t)
        {
            return t;
        }

        template<typename U>
        void * toPtr(std::shared_ptr<U> t)
        {
            return t.get();
        }

        std::string toHandle(T t)
        {
            std::stringstream ss;
            ss << typeStr << ':' << toPtr(t);
            return ss.str();
        }

        std::string add(T t, int scriptID)
        {
            int sceneID = getSceneID(scriptID);
            handlesf[sceneID][scriptID].insert(t);
            handlesr[t][sceneID] = scriptID;
            std::string h = toHandle(t);
            auto it = byhandle.find(h);
            if(it != byhandle.end())
            {
                if(it->second != t)
                    throw std::runtime_error("fatal error: handle already exists but points to different object");
            }
            else
            {
                byhandle[h] = t;
            }
            return h;
        }

        T remove(T t)
        {
            std::string h = toHandle(t);
            auto it0 = byhandle.find(h);
            if(it0 == byhandle.end())
                throw std::runtime_error("invalid object handle");
            byhandle.erase(it0);
            auto it = handlesr.find(t);
            if(it == handlesr.end()) return t;
            for(const auto &m : it->second)
            {
                int sceneID = m.first;
                int scriptID = m.second;
                auto it1 = handlesf.find(sceneID);
                if(it1 == handlesf.end()) continue;
                auto it2 = it1->second.find(scriptID);
                if(it2 == it1->second.end()) continue;
                it2->second.erase(t);
            }
            handlesr.erase(it);
            return t;
        }

        T get(std::string h) const
        {
            auto it = byhandle.find(h);
            if(it == byhandle.end())
                throw std::runtime_error("invalid object handle");
            return it->second;
        }

        std::set<T> find(int scriptID) const
        {
            int sceneID = getSceneID(scriptID);
            auto it = handlesf.find(sceneID);
            if(it == handlesf.end()) return {};
            auto it2 = it->second.find(scriptID);
            if(it2 == it->second.end()) return {};
            return it2->second;
        }

        std::set<T> findBySceneOfScript(int scriptID) const
        {
            return findByScene(getSceneID(scriptID));
        }

        std::set<T> findByScene(int sceneID) const
        {
            auto it = handlesf.find(sceneID);
            if(it == handlesf.end()) return {};
            std::set<T> r;
            for(const auto &x : it->second)
                for(auto t : x.second)
                    r.insert(t);
            return r;
        }

        std::set<T> all() const
        {
            std::set<T> r;
            for(const auto &x : handlesr)
                r.insert(x.first);
            return r;
        }

        std::set<std::string> handles() const
        {
            std::set<std::string> r;
            for(const auto &x : handlesr)
                r.insert(toHandle(x.first));
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

        // handle -> object
        std::map<std::string, T> byhandle;

        // sceneID -> (scriptID -> [objects])
        std::map<int, std::map<int, std::set<T>>> handlesf;

        // object -> (sceneID -> scriptID)
        std::map<T, std::map<int, int>> handlesr;

        std::string typeStr;
    };
}

#endif // SIMPLUSPLUS_HANDLES_H_INCLUDED
