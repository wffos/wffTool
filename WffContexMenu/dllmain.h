// dllmain.h: 模块类的声明。

class CWffContexMenuModule : public ATL::CAtlDllModuleT< CWffContexMenuModule >
{
public :
	DECLARE_LIBID(LIBID_WffContexMenuLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WFFCONTEXMENU, "{cf384bcc-50dd-4a67-8025-b17005091e51}")
};

extern class CWffContexMenuModule _AtlModule;
