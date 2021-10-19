// Parent Header
#include "LAL.hpp"



ptr<const WChar> operator""_wc(ptr<const wchar_t> _um, uIntDM /*_umSize*/)
{
	return RCast<const WChar>(_um);
}
