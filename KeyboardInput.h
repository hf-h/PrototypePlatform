#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#define KEY_STATE_UP 1
#define KEY_STATE_DOWN 0

//////////////////////////////////////////////////////////
///                     KEY CODES                      ///
//////////////////////////////////////////////////////////
#define INP_LBUTTON              0x01
#define INP_RBUTTON              0x02
#define INP_CANCEL               0x03
#define INP_MBUTTON              0x04
#define INP_XBUTTON1             0x05
#define INP_XBUTTON2             0x06
#define INP_BACK	         0x08
#define INP_TAB	                 0x09
#define INP_CLEAR                0x0C
#define INP_RETURN               0x0D
#define INP_SHIFT                0x10
#define INP_CONTROL              0x11
#define INP_MENU	         0x12    //ALT key
#define INP_PAUSE                0x13    //PAUSE key
#define INP_CAPITAL              0x14    //CAPS LOCK key
#define INP_KANA                 0x15    //IME Kana mode
#define INP_HANGUL               0x15    //IME Hangul mode
#define INP_IME_ON               0x16    //IME On
#define INP_JUNJA                0x17    //IME Junja mode
#define INP_FINAL                0x18    //IME final mode
#define INP_HANJA                0x19    //IME Hanja mode
#define INP_KANJI                0x19    //IME Kanji mode
#define INP_IME_OFF              0x1A    //IME Off
#define INP_ESCAPE               0x1B    //ESC key
#define INP_CONVERT              0x1C    //IME convert
#define INP_NONCONVERT           0x1D    //IME nonconvert
#define INP_ACCEPT               0x1E    //IME accept
#define INP_MODECHANGE           0x1F    //IME mode change request
#define INP_SPACE                0x20    //SPACEBAR
#define INP_PRIOR                0x21    //PAGE UP key
#define INP_NEXT                 0x22    //PAGE DOWN key
#define INP_END                  0x23    //END key
#define INP_HOME	         0x24    //HOME key
#define INP_LEFT	         0x25    //LEFT ARROW key
#define INP_UP                   0x26    //UP ARROW key
#define INP_RIGHT                0x27    //RIGHT ARROW key
#define INP_DOWN                 0x28    //DOWN ARROW key
#define INP_SELECT               0x29    //SELECT key
#define INP_PRINT                0x2A    //PRINT key
#define INP_EXECUTE              0x2B    //EXECUTE key
#define INP_SNAPSHOT             0x2C    //PRINT SCREEN key
#define INP_INSERT               0x2D    //INS key
#define INP_DELETE               0x2E    //DEL key
#define INP_HELP                 0x2F    //HELP key
#define INP_KEY_0                0x30
#define INP_KEY_1                0x31
#define INP_KEY_2                0x32
#define INP_KEY_3                0x33
#define INP_KEY_4                0x34
#define INP_KEY_5                0x35
#define INP_KEY_6                0x36
#define INP_KEY_7                0x37
#define INP_KEY_8                0x38
#define INP_KEY_9                0x39
#define INP_KEY_A                0x41
#define INP_KEY_B                0x42
#define INP_KEY_C                0x43
#define INP_KEY_D                0x44
#define INP_KEY_E                0x45
#define INP_KEY_F                0x46
#define INP_KEY_G                0x47
#define INP_KEY_H                0x48
#define INP_KEY_I                0x49
#define INP_KEY_J                0x4A
#define INP_KEY_K                0x4B
#define INP_KEY_L                0x4C
#define INP_KEY_M                0x4D
#define INP_KEY_N                0x4E
#define INP_KEY_O                0x4F
#define INP_KEY_P                0x50
#define INP_KEY_Q                0x51
#define INP_KEY_R                0x52
#define INP_KEY_S                0x53
#define INP_KEY_T                0x54
#define INP_KEY_U                0x55
#define INP_KEY_V                0x56
#define INP_KEY_W                0x57
#define INP_KEY_X                0x58
#define INP_KEY_Y                0x59
#define INP_KEY_Z                0x5A
#define INP_LWIN                 0x5B
#define INP_RWIN                 0x5C
#define INP_APPS                 0x5D
#define INP_SLEEP	        0x5F    //Computer Sleep key
#define INP_NUMPAD0	        0x60    //Numeric keypad 0 key
#define INP_NUMPAD1	        0x61    //Numeric keypad 1 key
#define INP_NUMPAD2	        0x62    //Numeric keypad 2 key
#define INP_NUMPAD3	        0x63    //Numeric keypad 3 key
#define INP_NUMPAD4	        0x64    //Numeric keypad 4 key
#define INP_NUMPAD5	        0x65    //Numeric keypad 5 key
#define INP_NUMPAD6	        0x66    //Numeric keypad 6 key
#define INP_NUMPAD7	        0x67    //Numeric keypad 7 key
#define INP_NUMPAD8	        0x68    //Numeric keypad 8 key
#define INP_NUMPAD9	        0x69    //Numeric keypad 9 key
#define INP_MULTIPLY	        0x6A    //Multiply key
#define INP_ADD                  0x6B    //Add key
#define INP_SEPARATOR	        0x6C    //Separator key
#define INP_SUBTRACT	        0x6D    //Subtract key
#define INP_DECIMAL	        0x6E    //Decimal key
#define INP_DIVIDE	        0x6F    //Divide key
#define INP_F1                   0x70    //F1 key
#define INP_F2                   0x71    //F2 key
#define INP_F3                   0x72    //F3 key
#define INP_F4                   0x73    //F4 key
#define INP_F5                   0x74    //F5 key
#define INP_F6                   0x75    //F6 key
#define INP_F7                   0x76    //F7 key
#define INP_F8                   0x77    //F8 key
#define INP_F9                   0x78    //F9 key
#define INP_F10                  0x79    //F10 key
#define INP_F11                  0x7A    //F11 key
#define INP_F12                  0x7B    //F12 key
#define INP_F13                  0x7C    //F13 key
#define INP_F14                  0x7D    //F14 key
#define INP_F15                  0x7E    //F15 key
#define INP_F16                  0x7F    //F16 key
#define INP_F17                  0x80    //F17 key
#define INP_F18                  0x81    //F18 key
#define INP_F19                  0x82    //F19 key
#define INP_F20                  0x83    //F20 key
#define INP_F21                  0x84    //F21 key
#define INP_F22                  0x85    //F22 key
#define INP_F23                  0x86    //F23 key
#define INP_F24                  0x87    //F24 key
#define INP_NUMLOCK              0x90    //NUM LOCK key
#define INP_SCROLL               0x91    //SCROLL LOCK key
#define INP_LSHIFT               0xA0    //Left SHIFT key
#define INP_RSHIFT               0xA1    //Right SHIFT key
#define INP_LCONTROL             0xA2    //Left CONTROL key
#define INP_RCONTROL             0xA3    //Right CONTROL key
#define INP_LMENU                0xA4    //Left ALT key
#define INP_RMENU                0xA5    //Right ALT key
#define INP_BROWSER_BACK         0xA6    //Browser Back key
#define INP_BROWSER_FORWARD      0xA7    //Browser Forward key
#define INP_BROWSER_REFRESH      0xA8    //Browser Refresh key
#define INP_BROWSER_STOP         0xA9    //Browser Stop key
#define INP_BROWSER_SEARCH       0xAA    //Browser Search key
#define INP_BROWSER_FAVORITES    0xAB    //Browser Favorites key
#define INP_BROWSER_HOME         0xAC    //Browser Start and Home key
#define INP_VOLUME_MUTE          0xAD    //Volume Mute key
#define INP_VOLUME_DOWN          0xAE    //Volume Down key
#define INP_VOLUME_UP            0xAF    //Volume Up key
#define INP_MEDIA_NEXT_TRACK     0xB0    //Next Track key
#define INP_MEDIA_PREV_TRACK     0xB1    //Previous Track key
#define INP_MEDIA_STOP           0xB2    //Stop Media key
#define INP_MEDIA_PLAY_PAUSE     0xB3    //Play/Pause Media key
#define INP_LAUNCH_MAIL          0xB4    //Start Mail key
#define INP_LAUNCH_MEDIA_SELECT	0xB5    //Select Media key
#define INP_LAUNCH_APP1          0xB6    //Start Application 1 key
#define INP_LAUNCH_APP2          0xB7    //Start Application 2 key
#define INP_OEM_1                0xBA    //Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ;: key
#define INP_OEM_PLUS             0xBB    //For any country/region, the + key
#define INP_OEM_COMMA            0xBC    //For any country/region, the , key
#define INP_OEM_MINUS            0xBD    //For any country/region, the - key
#define INP_OEM_PERIOD           0xBE    //For any country/region, the . key
#define INP_OEM_2                0xBF    //Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the /? key
#define INP_OEM_3                0xC0    //Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the `~ key
#define INP_OEM_4                0xDB    //Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the [{ key
#define INP_OEM_5                0xDC    //Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \\| key
#define INP_OEM_6                0xDD    //Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ]} key
#define INP_OEM_7                0xDE    //Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '" key
#define INP_OEM_8                0xDF    //Used for miscellaneous characters; it can vary by keyboard.
#define INP_OEM_102              0xE2    //The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard
#define INP_PROCESSKEY           0xE5    //IME PROCESS key
#define INP_PACKET               0xE7    //Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
#define INP_ATTN                 0xF6    //Attn key
#define INP_CRSEL                0xF7    //CrSel key
#define INP_EXSEL                0xF8    //ExSel key
#define INP_EREOF                0xF9    //Erase EOF key
#define INP_PLAY                 0xFA    //Play key
#define INP_ZOOM                 0xFB    //Zoom key
#define INP_NONAME               0xFC    //Reserved
#define INP_PA1                  0xFD    //PA1 key
#define INP_OEM_CLEAR            0xFE    //Clear key

#endif
