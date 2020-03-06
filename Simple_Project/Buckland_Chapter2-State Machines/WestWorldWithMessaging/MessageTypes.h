#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
	Msg_HiHoneyImHome,
	Msg_StewReady,
	/* Message d'interraction entre le mineur et le saoulard */
	Msg_HiFriendIWillDrink,
	Msg_GoBackToYourHome,
	Msg_YourAreAlreadyDrunk,
	Msg_GoodLuck
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case 0:
    
    return "HiHoneyImHome"; 

  case 1:
    
    return "StewReady";

  /* Message d'interraction entre le mineur et le saoulard */
  case 2:

	  return "HiFriendIWillDrink";

  case 3:

	  return "GoBackToYourHome";
	  
  case 4:
	  
	  return "YourAreAlreadyDrunk";
	  
  case 5:
	  
	  return "GoodLuck";

  default:

    return "Not recognized!";
  }
}

#endif