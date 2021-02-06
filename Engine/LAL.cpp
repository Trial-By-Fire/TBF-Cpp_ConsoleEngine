// Parent Header
#include "LAL.hpp"



ptr<ro WChar> operator""_wc(ptr<ro wchar_t> _um, uIntDM /*_umSize*/)
{
	return RCast<ro WChar>(_um);
}
