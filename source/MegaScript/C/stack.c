int PushScriptStack(Script *subject_script, byte *bytes, int length) {
    if(length == 0) return 0;
    if(subject_script->values.stack_pointer + length <= megascript_stack_size) {
        memcpy(subject_script->values.stack + subject_script->values.stack_pointer, bytes, length);
        subject_script->values.stack_pointer += length;
    } else {
        return StackOverflowError;
    }
    return 0;
}

byte *PopScriptStack(Script *subject_script, int length) {
    if(subject_script->values.stack_pointer - length >= 0) {
        subject_script->values.stack_pointer -= length;
    } else {
        return 0;
    }
    return subject_script->values.stack + subject_script->values.stack_pointer;
}

int ConvertStackDataToCode(StackData stack_data, uint64 *byte_count, uint64 *chunk_count, byte **script_code) {
    byte bytecode = 0; //this is still unfinished
    switch(stack_data.type) {
        case UnsignedInt8Type:
        case UnsignedInt16Type:
        case UnsignedInt32Type:
        case UnsignedInt64Type:
            bytecode = IntegerOpcode;
            break;
        case Int8Type:
        case Int16Type:
        case Int32Type:
        case Int64Type:
            bytecode = SignedIntegerOpcode;
            break;
        case FloatType:
        case DoubleType:
        case LongDoubleType:
            bytecode = FloatOpcode;
            break;
        case CharType:
            bytecode = CharacterOpcode;
            AddMegaScriptBytes(&bytecode, byte_count, 1, chunk_count, script_code); //unfinished
            AddMegaScriptBytes(stack_data.data, byte_count, 1, chunk_count, script_code);
            return 0;
    }
    AddMegaScriptBytes(&bytecode, byte_count, 1, chunk_count, script_code);
    bytecode = (byte) stack_data.length;
    AddMegaScriptBytes(&bytecode, byte_count, 1, chunk_count, script_code);
    AddMegaScriptBytes(stack_data.data, byte_count, stack_data.length, chunk_count, script_code);
    return 0;
}

int GetDataTypeSize(byte data_type) {
    switch(data_type) {
        case Bool8ContainerType:
        case Bool8Type:
            return 1;
        case Bool16ContainerType:
        case Bool16Type:
            return 2;
        case Bool32ContainerType:
        case Bool32Type:
            return 4;
        case Bool64ContainerType:
        case Bool64Type:
            return 8;
    }
    return 0;
}

int GetDataTypeLength(byte data_type) {
    switch(data_type) {
        case TemporarySymbolType:
        case ForLoopSaveType:
        case WhileLoopSaveType:
        case SwitchStatementSaveType:
        case ListType:
        case SymbolType:
        case CPointerType:
        case PointerType:
        case ElementType:
        case Bool8Type:
        case Bool16Type:
        case Bool32Type:
        case Bool64Type:
        case StructType:
        case ObjectType:
        case ObjectClassType:
        case CFunctionType:
            return bit_mode / 8;
        case EnumStackSaveType:
        case EnumType:
        case ObjectCallbackType:
            return (bit_mode / 8) * 2;
        case Bool8ContainerType:
        case Bool16ContainerType:
        case Bool32ContainerType:
        case Bool64ContainerType:
            return bit_mode / 8 + 2;
        case DataType:
        case CharType:
        case Int8Type:
        case UnsignedInt8Type:
            return 1;
        case Int16Type:
        case UnsignedInt16Type:
            return 2;
        case OperationType:
        case FloatType:
        case Int32Type:
        case UnsignedInt32Type:
            return 4;
        case DoubleType:
        case Int64Type:
        case UnsignedInt64Type:
            return 8;
        case LongDoubleType:
            return 16;
        case NewDataType:
        case AutoDeleteType:
        case VoidType:
        case NullType:
        default:
            return 0;
    }
}

int RefreshStackData(StackData *stack_data) {
    int error_code = 0;
    int stack_data_length = GetDataTypeLength(stack_data->type);
    byte *data_buffer = malloc(stack_data_length);
    memcpy(data_buffer, stack_data->data, stack_data_length); //marker: remember to free this later
    stack_data->data = data_buffer;
    return error_code;
}

StackData CheckScriptStackData(Script *subject_script) {
    StackData ret_val = {0};
    ret_val.type = *(subject_script->values.stack + (subject_script->values.stack_pointer - 1));
    int data_length = GetDataTypeLength(ret_val.type);
    ret_val.data = subject_script->values.stack + (subject_script->values.stack_pointer - data_length - 1);
    if(ret_val.data < subject_script->values.stack || ret_val.data >= ret_val.data + megascript_stack_size) ret_val.data = 0;
    return ret_val;
}

int64 GetRecursiveStructSize(ScriptEnvironment *script_tasks, Script *subject_script, uint64 pointer) {
    int multiply_factor = bit_mode / 8;
    uint64 *list_data = (uint64*) ReadScriptMemoryRange(script_tasks, subject_script, pointer, multiply_factor * 4);
    if(list_data) {
        uint64 bytes = 0;
        for(uint64 i = 0; i < *list_data; i++) {
            byte *data_type = (byte*) ReadScriptMemoryRange(script_tasks, subject_script, *(list_data + 2) + i, 1);
            if(data_type) {
                switch(*data_type) {
                    case StructType:
                        uint64 *recursive_pointer = ReadScriptMemoryRange(script_tasks, subject_script, *(list_data + 3) + (i * multiply_factor), multiply_factor);
                        if(recursive_pointer) {
                            int64 return_size = GetRecursiveStructSize(script_tasks, subject_script, *recursive_pointer);
                            if(return_size == -1) return -1;
                            bytes += return_size;
                        } else {
                            return -1;
                        }
                        break;
                    case Bool8Type:
                    case Bool16Type:
                    case Bool32Type:
                    case Bool64Type:
                        bytes += GetDataTypeSize(*data_type);
                        break;
                    default:
                        bytes += GetDataTypeLength(*data_type);
                }
            } else {
                return -1;
            }
        }
        return (int64) bytes;
    }
    return -1;
}

StructStackData PopScriptStackStruct(ScriptEnvironment *script_tasks, Script *subject_script, StackData struct_stack_data) {
    StructStackData struct_data = {0}; //The struct list pointer points to a list of 4 64/32 bit values. Value 1 contains the children count value, value 2 points to list of symbol string pointers, value 3 points to the data types, and value 4 points to a list of 64/32 bit auxilary values which are used to find structs inside of structs.
    struct_data.pointer = *(uint64*) struct_stack_data.data; //pointer marker
    int64 size = GetRecursiveStructSize(script_tasks, subject_script, struct_data.pointer);
    if(size == -1) {
        struct_data.error = StructListCouldNotBeFoundError;
        return struct_data;
    }
    struct_data.size = size;
    struct_data.data = PopScriptStack(subject_script, size);
    if(!struct_data.data) struct_data.error = FailedToPopStructError;
    return struct_data;
}

StackData PopScriptStackData(Script *subject_script) {
    StackData ret_val = {0};
    ret_val.data = PopScriptStack(subject_script, 1);
    if(!ret_val.data) {
        ret_val.error = StackEmptyError;
        return ret_val;
    }
    ret_val.type = *ret_val.data;
    int data_length = GetDataTypeLength(ret_val.type);
    ret_val.data = PopScriptStack(subject_script, data_length);
    if(!ret_val.data) {
        ret_val.error = StackEmptyError;
        return ret_val;
    }
    ret_val.length = data_length;
    return ret_val;
};

int PushScriptStackData(Script *subject_script, byte *data, byte data_type) {
    int error_code = 0;
    int data_length = GetDataTypeLength(data_type);
    error_code = PushScriptStack(subject_script, data, data_length);
    if(!error_code) error_code = PushScriptStack(subject_script, &data_type, 1);
    return error_code;
};

int PushStackData(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_data) {
    int error_code = 0;
    int multiply_factor = bit_mode / 8;
    switch(stack_data.type) {
        case StructType:
            if(error_code = PushScriptStack(subject_script, stack_data.data, stack_data.length + multiply_factor)) return error_code;
            if(error_code = PushScriptStack(subject_script, &stack_data.type, 1)) return error_code;
            break;
        default:
            error_code = PushScriptStackData(subject_script, stack_data.data, stack_data.type);
    }
    if(stack_data.settings & StackDataWasMallocated) free(stack_data.data);
    return error_code;
}

StackData PopStackData(ScriptEnvironment *script_tasks, Script *subject_script) {
    byte *data_type_ptr = PopScriptStack(subject_script, 1);
    int multiply_factor = bit_mode / 8;
    StackData new_data = {0};
    if(data_type_ptr) {
        byte data_type = *data_type_ptr;
        uint64 *list_address = 0;
        switch(data_type) {
            case StructType:
                list_address = (uint64*) PopScriptStack(subject_script, multiply_factor);
                if(list_address) {
                    new_data.length = GetRecursiveStructSize(script_tasks, subject_script, *list_address);
                    new_data.data = PopScriptStack(subject_script, new_data.length);
                } else {
                    new_data.error = StructListCouldNotBeFoundError;
                }
                break;
            default:
                new_data.length = GetDataTypeLength(data_type);
                new_data.data = PopScriptStack(subject_script, new_data.length);
        }
    }
    return new_data;
}

int PushScriptStackStruct(Script *subject_script, StructStackData stack_data) {
    //First is length, second is elements, third is data types
    int error_code = PushScriptStack(subject_script, stack_data.data, stack_data.size);
    if(error_code) return error_code;
    if(error_code = PushScriptStack(subject_script, (byte*) &stack_data.pointer, bit_mode / 8)) return error_code;
    byte data_type = StructType;
    return PushScriptStack(subject_script, (byte*) &data_type, 1);
}

int PushScriptStackPointerNumber(Script *subject_script, uint64 address, byte data_type) {
    uint32 data_value = (uint32) address;
    switch(bit_mode) {
        case 32:
            if(PushScriptStackData(subject_script, (byte*) &data_value, data_type)) return StackOverflowError;
            break;
        case 64:
            if(PushScriptStackData(subject_script, (byte*) &address, data_type)) return StackOverflowError;
            break;
    }
    return 0;
}

int PushScriptStackPointer(Script *subject_script, uint64 address) {
    return PushScriptStackPointerNumber(subject_script, address, PointerType);
}

int PushScriptStackSymbol(Script *subject_script, uint64 address) {
    return PushScriptStackPointerNumber(subject_script, address, SymbolType);
}

uint64 GetPointerStackData(StackData stack_data) {
    switch(bit_mode) {
        case 32:
            return (uint64) (*(uint32*) stack_data.data);
        case 64:
            return *(uint64*) stack_data.data;
    }
}

uint64 GetPointerFromStackData(StackData stack_data) {
    return GetPointerStackData(stack_data);
}

Symbol *GetSymbolFromStackData(Script *subject_script, StackData stack_data) {
    uint64 symbol_id = 0;
    switch(bit_mode) {
        case 32:
            symbol_id = (uint64) (*(uint32*) stack_data.data);
        case 64:
            symbol_id = *(uint64*) stack_data.data;
    }
    Symbol *current_symbol = 0;
    int64 symbol_index = FindSymbolById(subject_script, symbol_id);
    if(symbol_index != -1) {
        current_symbol = subject_script->values.symbol_table + symbol_index;
    }
    return current_symbol;
}

int DecrementScriptStack(Script *subject_script) {
    byte data_type_length = GetDataTypeLength(CheckScriptStackData(subject_script).type);
    subject_script->values.stack_pointer -= data_type_length + 1;
    return 0;
}

char *PopScriptStackString(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(stack_symbol.error) return 0;
    return GetStringFromStackData(script_tasks, subject_script, stack_symbol);
}