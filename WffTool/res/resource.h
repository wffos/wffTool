//stamp:07636ba71883cd3a
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
			}
			const TCHAR * XML_MAINWND;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _IMG{
			public:
			_IMG(){
			}
		}IMG;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
		class _translator{
			public:
			_translator(){
				lang_cn = _T("translator:lang_cn");
				lang_jp = _T("translator:lang_jp");
			}
			const TCHAR * lang_cn;
			const TCHAR * lang_jp;
		}translator;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"_name_start",65535},
		{L"btn_close",65536},
		{L"btn_max",65537},
		{L"btn_min",65539},
		{L"btn_restore",65538},
		{L"jp",65541},
		{L"zh_cn",65540}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			_name_start = namedXmlID[0].strName;
			btn_close = namedXmlID[1].strName;
			btn_max = namedXmlID[2].strName;
			btn_min = namedXmlID[3].strName;
			btn_restore = namedXmlID[4].strName;
			jp = namedXmlID[5].strName;
			zh_cn = namedXmlID[6].strName;
		}
		 const wchar_t * _name_start;
		 const wchar_t * btn_close;
		 const wchar_t * btn_max;
		 const wchar_t * btn_min;
		 const wchar_t * btn_restore;
		 const wchar_t * jp;
		 const wchar_t * zh_cn;
		}name;

		class _id{
		public:
		const static int _name_start	=	65535;
		const static int btn_close	=	65536;
		const static int btn_max	=	65537;
		const static int btn_min	=	65539;
		const static int btn_restore	=	65538;
		const static int jp	=	65541;
		const static int zh_cn	=	65540;
		}id;

		class _string{
		public:
		const static int title	=	0;
		const static int ver	=	1;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int gray	=	1;
		const static int green	=	2;
		const static int red	=	3;
		const static int white	=	4;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
