uint8_t get_keyboard_code(){
    if(inb(0x64) & 1)
        return inb(0x60);

    else
        return 0;
}

Key_Info get_key_info(uint8_t scancode){
    Key_Info out;            
    out.key = KEY_NULL;

    if(scancode){
        switch (scancode)
        {
        case KEY_LSHIFT:
            lshift = true;
            break;
        
        case 0xAA:
        lshift = false;
            break;
        
        case KEY_RSHIFT:
            rshift = true;
            break;

        
        case 0xB6:
            rshift = false;
            break;
    
        case KEY_LCTRL:
            lctrl = true;
            break;
    
        //ctrl release
        case 0x9D:
            lctrl = false;
            break;
    
        case KEY_LALT:
            lalt = true;
            break;
    
        case 0xB8:
            lalt = false;
        
        default:
            break;
        }
         
        out.key = KEY_A;

        if(scancode > 0x58){
            return out;
        }
        
        out.key = (Keys)scancode;
        out.shift = lshift || rshift;
        out.ctrl = lctrl;
        out.alt = lalt;
        
        out.ch = kbd_US[scancode];

        return out;
        
    }

    else{
        return out;
    }
}

char get_char(Key_Info key){
    if(key.ch != 0){
        if(key.shift){
            return kbd_US_shift[key.key];
        }

        return kbd_US[key.key];
    }

    return 0;
}