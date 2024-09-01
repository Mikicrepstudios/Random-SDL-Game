byte *GenerateValidString(ScriptEnvironment *script_tasks, Script *subject_script, uint64 address, uint64 length) {
    byte *ret_val = 0;
    byte *char_buffer = ReadScriptMemoryRange(script_tasks, subject_script, address, length);
    if(char_buffer) {
        ret_val = malloc(length + 1);
        *(ret_val + length) = '\0';
        for(int i = 0; i < length; i++) {
            *(ret_val + i) = *(char_buffer + i); //I can just memcpy but I'll leave this here for now because I'm lazy lol.
        }
    }
    return ret_val;
}

byte GetCharacterByNumber(int number) {
    if(number < 10) {
        return 48 + number;
    } else {
        return 55 + number;
    }
}

byte *ConcatenateStrings(byte *string_0, byte *string_1, byte **old_pointer) {
    int string_length = strlen(string_1);
    int former_length = strlen(string_0);
    byte *message_buffer = malloc(former_length + string_length + 1);
    memcpy(message_buffer, string_0, former_length);
    *(message_buffer + former_length + string_length) = '\0';
    memcpy(message_buffer + former_length, string_1, string_length);
    if(old_pointer) {
        free(*old_pointer);
        *old_pointer = 0;
    }
    return message_buffer;
}

byte *UnsignedIntegerToString(uint64, byte);

byte *UnsignedIntegerToString(uint64 number, byte base/* = 10*/) {
    uint64 buffer_position = 0;
    byte *buffer = 0;
    if(!number) {
        buffer = malloc(2);
        *buffer = '0';
        *(buffer + 1) = '\0';
        return buffer;
    }
    uint64 carry = number;
    while(carry > 0) {
        byte chr = GetCharacterByNumber(carry % base);
        carry = carry / base;
        byte *new_buffer = malloc(buffer_position + 2);
        buffer_position++;
        *(new_buffer + buffer_position) = '\0';
        *new_buffer = chr;
        if(buffer) {
            memcpy(new_buffer + 1, buffer, buffer_position);
            free(buffer);
        }
        buffer = new_buffer;
    }
    return buffer;
}

byte *IntegerToString(int64 number, byte base/* = 10*/) {
    byte negative = 0;
    byte *buffer = 0;
    if(number < 0LL) {
        negative = 1;
        number *= -1;
    }
    buffer = UnsignedIntegerToString((uint64) number, base);
    if(negative) {
        int string_length = strlen(buffer);
        byte *new_buffer = malloc(string_length + 2);
        *new_buffer = '-';
        memcpy(new_buffer + 1, buffer, string_length + 1);
        free(buffer);
        buffer = new_buffer;
    }
    return buffer;
}

byte *FloatToString(float number, byte base/* = 10*/) {
    byte negative = 0;
    byte *buffer = 0;
    if(number < 0.0) {
        negative = 1;
        number *= -1.0;
    }
    buffer = UnsignedIntegerToString((uint64) number, base);
    if(negative) {
        int string_length = strlen(buffer);
        byte *new_buffer = malloc(string_length + 2);
        *new_buffer = '-';
        memcpy(new_buffer + 1, buffer, string_length + 1);
        free(buffer);
        buffer = new_buffer;
    }
    number = number - (float) ((uint64) number);
    if(number > 0.0) {
        int string_length = strlen(buffer);
        byte decimal_left = 1;
        buffer = realloc(buffer, string_length + 2);
        *(buffer + string_length) = '.';
        string_length++;
        while(decimal_left) {
            number = number * base;
            if(number <= 0.0) {
                decimal_left = 0;
            } else {
                buffer = realloc(buffer, string_length + 2);
                *(buffer + string_length) = GetCharacterByNumber((uint32) number);
                string_length++;
                number = number - (float) ((uint64) number);
            }
        }
        *(buffer + string_length) = '\0';
    }
    return buffer;
}

byte *DoubleToString(double number, byte base/* = 10*/) {
    byte negative = 0;
    byte *buffer = 0;
    if(number < 0.0) {
        negative = 1;
        number *= -1.0;
    }
    buffer = UnsignedIntegerToString((uint64) number, base);
    if(negative) {
        int string_length = strlen(buffer);
        byte *new_buffer = malloc(string_length + 2);
        *new_buffer = '-';
        memcpy(new_buffer + 1, buffer, string_length + 1);
        free(buffer);
        buffer = new_buffer;
    }
    number = number - (double) ((uint64) number);
    if(number > 0.0) {
        int string_length = strlen(buffer);
        byte decimal_left = 1;
        buffer = realloc(buffer, string_length + 2);
        *(buffer + string_length) = '.';
        string_length++;
        while(decimal_left) {
            number = number * base;
            if(number <= 0.0) {
                decimal_left = 0;
            } else {
                buffer = realloc(buffer, string_length + 2);
                *(buffer + string_length) = GetCharacterByNumber((uint32) number);
                string_length++;
                number = number - (double) ((uint64) number);
            }
        }
        *(buffer + string_length) = '\0';
    }
    return buffer;
}

uint64 GetStringLength(byte *string_input) {
    uint64 string_length;
    while(1) if(*(string_input + string_length) == '\0') break; else string_length++;
    return string_length;
} //this whole function is broken right now for some reason. it causes a segmentation fault and I don't know why

char *HandlePointerTypeString(ScriptEnvironment *script_tasks, Script *subject_script, StackData title_string) {
    uint64 address = GetPointerFromStackData(title_string);
    byte multiply_factor = bit_mode / 8;
    uint64 length = 0;
    byte bytecode = ReadScriptMemory(script_tasks, subject_script, address);
    byte *string_buffer = 0;
    switch(bytecode) {
        case StringOpcode:
            address++;
            if(!(string_buffer = ReadScriptMemoryRange(script_tasks, subject_script, address, multiply_factor))) return 0;
            length = *(uint64*) string_buffer;
            address += multiply_factor;
            if(!(string_buffer = ReadScriptMemoryRange(script_tasks, subject_script, address, length))) return 0;
            byte *new_string = malloc(length + 1);
            *(new_string + length) = '\0';
            memcpy(new_string, string_buffer, length);
            return new_string;
        default:
            return 0;
    }
}

char *GetStringFromStackData(ScriptEnvironment *script_tasks, Script *subject_script, StackData title_string) {
    StackData stack_symbol = title_string;
    uint64 data_address = 0;
    byte *string_pointer = 0;
    switch(title_string.type) {
        case PointerType:
            return HandlePointerTypeString(script_tasks, subject_script, title_string);
        case SymbolType:
            data_address = GetPointerStackData(stack_symbol);
            int64 symbol_index = FindSymbolById(subject_script, data_address);
            if(symbol_index != -1) {
                Symbol *temp_symbol = subject_script->values.symbol_table + symbol_index;
                int error_code = 0;
                if(temp_symbol->type == CPointerType && temp_symbol->sub_type == CharType && temp_symbol->dimension == 1) {
                    uint64 string_length = 0;
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, bit_mode / 8);
                    if(!string_pointer) return 0;
                    uint64 string_address = *(uint64*) string_pointer;
                    while(1) {
                        byte this_character = ReadScriptMemory(script_tasks, subject_script, string_address + string_length);
                        string_length++;
                        if(this_character == 0) break;
                    }
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, string_address, string_length);
                    byte *new_string = malloc(string_length);
                    *(new_string + (string_length - 1)) = '\0';
                    memcpy(new_string, string_pointer, string_length - 1);
                    return new_string;
                } else if(temp_symbol->type == PointerType) {
                    stack_symbol.data = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, bit_mode / 8);
                    if(!stack_symbol.data) return 0;
                    stack_symbol.type = PointerType;
                    return HandlePointerTypeString(script_tasks, subject_script, stack_symbol);
                } else return 0;
            }
            break;
        case TemporarySymbolType:
            data_address = GetPointerStackData(stack_symbol);
            symbol_index = FindDebris(subject_script, data_address);
            if(symbol_index != -1) {
                TemporarySymbol *temp_symbol = subject_script->system.debris + symbol_index;
                int error_code = 0;
                if(temp_symbol->type == CPointerType && temp_symbol->sub_type == CharType && temp_symbol->dimension == 1) {
                    uint64 string_length = 0;
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, bit_mode / 8);
                    if(!string_pointer) return 0;
                    uint64 string_address = *(uint64*) string_pointer;
                    while(1) {
                        byte this_character = ReadScriptMemory(script_tasks, subject_script, string_address + string_length);
                        string_length++;
                        if(this_character == 0) break;
                    }
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, string_address, string_length);
                    byte *new_string = malloc(string_length);
                    *(new_string + (string_length - 1)) = '\0';
                    memcpy(new_string, string_pointer, string_length - 1);
                    DeleteDebris(script_tasks, subject_script, symbol_index);
                    return new_string;
                } else if(temp_symbol->type == PointerType) {
                    stack_symbol.data = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, bit_mode / 8);
                    if(!stack_symbol.data) return 0;
                    stack_symbol.type = PointerType;
                    DeleteDebris(script_tasks, subject_script, symbol_index);
                    return HandlePointerTypeString(script_tasks, subject_script, stack_symbol);
                }
                DeleteDebris(script_tasks, subject_script, symbol_index);
                return 0;
            }
            break;
        default:
            return 0;
    }
    return 0;
}

int RemoveEndingWhiteSpace(byte *string_buffer) {
    int64 last_space = -1;
    uint64 current_index = 0;
    while(1) {
        byte current_char = *(string_buffer + current_index);
        switch(current_char) {
            case ' ':
            case '\n':
            case '\t':
                if(last_space == -1) last_space = current_index;
                break;
            case '\0':
                if(last_space != -1) *(string_buffer + last_space) = '\0';
                break;
            default:
                last_space = -1;
        }
        current_index++;
    }
    return 0;
}