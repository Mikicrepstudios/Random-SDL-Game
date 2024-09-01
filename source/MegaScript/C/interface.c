Symbol CreateSymbolFromData(ScriptEnvironment *script_tasks, Script *subject_script, byte *class_name, byte data_type) {
    Symbol ret_val = {0};
    if(class_name) {
        ret_val.length = (byte) (strlen(class_name) - 1);
        uint64 class_name_length = (uint64) ret_val.length + 2;
        ret_val.name = AllocateScriptMemory(script_tasks, subject_script, class_name_length);
        if(ret_val.name == 0) return ret_val;
        byte *actual_name = script_tasks->memory + ret_val.name;
        memcpy(actual_name, class_name, class_name_length);
        *(actual_name + class_name_length) = '\0';
    }
    ret_val.type = data_type;
    ret_val.size = GetDataTypeLength(ret_val.type);
    ret_val.created_at = 0;
    return ret_val;
}

Symbol CreateBasicDataTypeSymbol(ScriptEnvironment *script_tasks, Script *subject_script, byte *class_name, byte data_type, void *data) {
    Symbol ret_val = {0};
    switch(data_type) {
        case Int8Type:
        case Int16Type:
        case Int32Type:
        case Int64Type:
        case UnsignedInt8Type:
        case UnsignedInt16Type:
        case UnsignedInt32Type:
        case UnsignedInt64Type:
        case PointerType:
        case CharType:
        case DataType:
        case FloatType:
        case DoubleType:
        case LongDoubleType:
        case NullType:
            ret_val = CreateSymbolFromData(script_tasks, subject_script, class_name, data_type);
            ret_val.address = AllocateScriptMemory(script_tasks, subject_script, ret_val.size);
            if(ret_val.address == 0) return ret_val;
            ret_val.settings |= IsCLike;
            byte *actual_data = ReadScriptMemoryRange(script_tasks, subject_script, ret_val.address, ret_val.type);
            if(!actual_data) return ret_val;
            memcpy(actual_data, data, ret_val.size);
    }
    return ret_val;
}

Symbol CreateStructElementSymbol(ScriptEnvironment *script_tasks, Script *subject_script, byte *class_name, byte data_type, byte *typedef_type) {
    Symbol ret_val = {0};
    int multiply_factor = bit_mode / 8;
    switch(data_type) {
        case Int8Type:
        case Int16Type:
        case Int32Type:
        case Int64Type:
        case UnsignedInt8Type:
        case UnsignedInt16Type:
        case UnsignedInt32Type:
        case UnsignedInt64Type:
        case PointerType:
        case CharType:
        case DataType:
        case FloatType:
        case DoubleType:
        case LongDoubleType:
        case NullType:
        case CustomDataType:
            uint64 class_name_length = strlen(class_name);
            ret_val.name = AllocateScriptMemory(script_tasks, subject_script, class_name_length + 1);
            if(ret_val.name == 0) return ret_val;
            byte *actual_name = script_tasks->memory + ret_val.name;
            *actual_name = (byte) (class_name_length - 1);
            memcpy(actual_name + 1, class_name, class_name_length);
            ret_val.type = data_type;
            if(ret_val.type == CustomDataType) {
                byte typedef_length = (byte) (strlen(typedef_type) - 1);
                int64 typedef_index = FindTypedef(script_tasks, subject_script, typedef_type, typedef_length);
                if(typedef_index == -1) return ret_val;
                Symbol *current_symbol = subject_script->values.symbol_table + typedef_index;
                ret_val.type = current_symbol->sub_type;
                ret_val.list_pointer = current_symbol->list_pointer;
                switch(ret_val.type) {
                    case StructType:
                        ret_val.size = GetRecursiveStructSize(script_tasks, subject_script, ret_val.list_pointer);
                        break;
                    default:
                        ret_val.size = GetDataTypeLength(ret_val.type);
                }
            } else {
                ret_val.size = GetDataTypeLength(ret_val.type);
            }
            if(ret_val.address == 0) return ret_val;
            ret_val.settings |= IsCLike;
            ret_val.scope = -1;
            ret_val.created_at = 0;
    }
    return ret_val;
}

Symbol CreateStructTypedefSymbol(ScriptEnvironment *script_tasks, Script *subject_script, byte *struct_name, Symbol *element_symbols, uint64 element_count) {
    Symbol ret_val = CreateSymbolFromData(script_tasks, subject_script, struct_name, CustomDataType);
    int multiply_factor = bit_mode / 8;
    if(ret_val.name == 0) return ret_val;
    ret_val.sub_type = StructType;
    ret_val.address = AllocateScriptMemory(script_tasks, subject_script, multiply_factor * 4);
    ret_val.list_pointer = ret_val.address;
    if(ret_val.address == 0) return ret_val;
    uint64 *addresses = (uint64*) (script_tasks->memory + ret_val.address);
    *addresses = element_count;
    *(addresses + 1) = AllocateScriptMemory(script_tasks, subject_script, multiply_factor * element_count);
    *(addresses + 2) = AllocateScriptMemory(script_tasks, subject_script, element_count);
    *(addresses + 3) = AllocateScriptMemory(script_tasks, subject_script, multiply_factor * element_count);
    if(!(*(addresses + 1)) || !(*(addresses + 2)) || !(*(addresses + 3))) return ret_val;
    uint64 *element_list = (uint64*) (script_tasks->memory + *(addresses + 1));
    byte *data_list = (byte*) (script_tasks->memory + *(addresses + 2));
    uint64 *auxilary_list = (uint64*) (script_tasks->memory + *(addresses + 3));
    for(uint64 i = 0; i < element_count; i++) {
        Symbol *this_symbol = element_symbols + i;
        *(element_list + i) = this_symbol->name;
        *(data_list + i) = this_symbol->type;
        *(auxilary_list + i) = this_symbol->list_pointer;
    }
    return ret_val;
}

Symbol CreateBoolTypedefSymbol(ScriptEnvironment *script_tasks, Script *subject_script, byte *struct_name, Symbol *element_symbols, uint64 element_count, byte bool_type) {
    Symbol ret_val = CreateSymbolFromData(script_tasks, subject_script, struct_name, CustomDataType);
    int multiply_factor = bit_mode / 8;
    if(ret_val.name == 0) return ret_val;
    ret_val.sub_type = bool_type;
    ret_val.address = AllocateScriptMemory(script_tasks, subject_script, multiply_factor * GetDataTypeSize(bool_type) * 8);
    ret_val.list_pointer = ret_val.address;
    int error_code = 0;
    for(uint64 i = 0; i < element_count; i++) {
        Symbol *this_symbol = element_symbols + i;
        if(error_code = WriteScriptMemoryRange(script_tasks, subject_script, ret_val.address + i * multiply_factor, multiply_factor, (byte*) &this_symbol->name)) return ret_val;
    }
    return ret_val;
}

int LinkCallbackToScript(ScriptEnvironment *script_tasks, Script *subject_script, char *new_name, int (*callback) (ScriptEnvironment*, Script*)) {
    Symbol function_pointer = CreateSymbolFromData(script_tasks, subject_script, new_name, CFunctionType);
    uint64 callback_address = (uint64) callback; //pointer marker
    int error_code = WriteScriptMemoryRange(script_tasks, subject_script, function_pointer.address, bit_mode / 8, (void*) &callback_address);
    if(error_code) return error_code;
    if(-1 == CreateSymbol(script_tasks, subject_script, function_pointer)) return FailedToCreateASMFunctionError;
    return 0;
}