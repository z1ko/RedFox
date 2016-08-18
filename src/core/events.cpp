#include "events.hpp"

namespace RedFox { namespace Events {
	
	namespace Input
	{
		Event<u32, u32> KeyAction;
		Event<u32, u32> BtnAction;
		Event<f64, f64> MouseMove;
	}

} }