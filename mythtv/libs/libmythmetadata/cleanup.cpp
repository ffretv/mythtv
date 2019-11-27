#include <list>
#include <algorithm>

#include "cleanup.h"

class CleanupHooksImp
{
  private:
    typedef std::list<CleanupProc *> clean_list;

  private:
    clean_list m_cleanList;

  public:
    void addHook(CleanupProc *clean_proc)
    {
        m_cleanList.push_back(clean_proc);
    }

    void removeHook(CleanupProc *clean_proc)
    {
        clean_list::iterator p = std::find(m_cleanList.begin(),
                                           m_cleanList.end(), clean_proc);
        if (p != m_cleanList.end())
        {
            m_cleanList.erase(p);
        }
    }

    void cleanup()
    {
        for (clean_list::iterator p = m_cleanList.begin();
             p != m_cleanList.end();++p)
        {
            (*p)->doClean();
        }
        m_cleanList.clear();
    }
};

namespace
{
    CleanupHooks *g_cleanup_hooks = nullptr;
}

CleanupHooks *CleanupHooks::getInstance()
{
    if (!g_cleanup_hooks)
    {
        g_cleanup_hooks = new CleanupHooks();
    }
    return g_cleanup_hooks;
}

void CleanupHooks::addHook(CleanupProc *clean_proc)
{
    m_imp->addHook(clean_proc);
}

void CleanupHooks::removeHook(CleanupProc *clean_proc)
{
    m_imp->removeHook(clean_proc);
}

void CleanupHooks::cleanup()
{
    m_imp->cleanup();
    delete g_cleanup_hooks;
    g_cleanup_hooks = nullptr;
}

CleanupHooks::CleanupHooks()
{
    m_imp = new CleanupHooksImp();
}

CleanupHooks::~CleanupHooks()
{
    delete m_imp;
}
