#include <lsp/types.h>

ConstStringU8 to_string(MessageType type) {
    ConstStringU8 result = SCu8("[null]");
    
    switch (type) {
        case MessageType_Error:
        result = SCu8("[" "\x1b\x5b" "31m" "Error" "\x1b\x5b" "0m" "]");
        break;
        case MessageType_Warning:
        result = SCu8("[" "\x1b\x5b" "33m" "Warning" "\x1b\x5b" "0m" "]");
        break;
        case MessageType_Info:
        result = SCu8("[" "\x1b\x5b" "32m" "Info" "\x1b\x5b" "0m" "]");
        break;
        case MessageType_Log:
        result = SCu8("[" "\x1b\x5b" "34m" "Log" "\x1b\x5b" "0m" "]");
        break;
    }
    
    return result;
}
