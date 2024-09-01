BytecodeInformation *GetBytecodeInformation(byte bytecode) {
    for(int i = 0; i < bytecode_information_count; i++) {
        BytecodeInformation *bytecode_info = (bytecode_information + i);
        if(bytecode_info->code == bytecode) {
            return bytecode_info;
        }
    }
    return 0;
}

void ExecuteStandardPrintCallbacks(ScriptEnvironment *script_tasks, Script *subject_script, byte *valid_string) {
    if(script_tasks->master.callbacks.print) script_tasks->master.callbacks.print(valid_string);
    if(subject_script->callbacks.print && subject_script->id != script_tasks->master.id) subject_script->callbacks.print(valid_string);
    free(valid_string);
}

void ExecuteStandardPrintCallbacksNoDelete(ScriptEnvironment *script_tasks, Script *subject_script, byte *valid_string) {
    if(script_tasks->master.callbacks.print) script_tasks->master.callbacks.print(valid_string);
    if(subject_script->callbacks.print && subject_script->id != script_tasks->master.id) subject_script->callbacks.print(valid_string);
}

void ExecuteStandardWarnCallbacks(ScriptEnvironment *script_tasks, Script *subject_script, byte *valid_string) {
    if(script_tasks->master.callbacks.warning) script_tasks->master.callbacks.warning(valid_string);
    if(subject_script->callbacks.warning && subject_script->id != script_tasks->master.id) subject_script->callbacks.warning(valid_string);
}

void StandardPrintUnsignedInteger(ScriptEnvironment *script_tasks, Script *subject_script, uint64 number) {
    byte *int_string = UnsignedIntegerToString(number, 10);
    ExecuteStandardPrintCallbacks(script_tasks, subject_script, int_string); //idea: make the syntax coloring be shades of red to be opposite of lua's blue
}

void StandardPrintInteger(ScriptEnvironment *script_tasks, Script *subject_script, int64 number) {
    byte *int_string = IntegerToString(number, 10);
    ExecuteStandardPrintCallbacks(script_tasks, subject_script, int_string);
}

void StandardPrintUnsignedIntegerNewLine(ScriptEnvironment *script_tasks, Script *subject_script, uint64 number) {
    byte *int_string = UnsignedIntegerToString(number, 10);
    ExecuteStandardPrintCallbacks(script_tasks, subject_script, ConcatenateStrings(int_string, "\n", &int_string)); //idea: make the syntax coloring be shades of red to be opposite of lua's blue
}

void StandardPrintIntegerNewLine(ScriptEnvironment *script_tasks, Script *subject_script, int64 number) {
    byte *int_string = IntegerToString(number, 10);
    ExecuteStandardPrintCallbacks(script_tasks, subject_script, ConcatenateStrings(int_string, "\n", &int_string));
}

byte CheckStandardCallPrivilege(Script *subject_script, byte bytecode) {
    BytecodeInformation *bytecode_info = GetBytecodeInformation(bytecode);
    if(bytecode_info) if(bytecode_info->privilege > subject_script->rules.privilege_level) {
        return 1;
    }
    return 0;
}

StackDataArray GetStandardCallArguments(Script *subject_script, byte bytecode) {
    StackDataArray ret_val = {0};
    BytecodeInformation *bytecode_info = GetBytecodeInformation(bytecode);
    if(bytecode_info) {
        if(bytecode_info->privilege > subject_script->rules.privilege_level) {
            ret_val.error = StandardCallPrivilegeError;
            return ret_val;
        }
        ret_val.count = bytecode_info->arguments;
        ret_val.array = malloc(ret_val.count * sizeof(StackData));
        for(int j = ret_val.count; j > 0; j--) {
            int i = j - 1;
            StackData current_data = CheckScriptStackData(subject_script);
            switch(current_data.type) {
                case 0:
                case OperationType:
                case ForLoopSaveType:
                    ret_val.error = TooFewStandardCallArgumentsError;
                    free(ret_val.array);
                    return ret_val;
            }
            *(ret_val.array + i) = PopScriptStackData(subject_script);
        }
        StackData check_data = CheckScriptStackData(subject_script);
        switch(check_data.type) {
            case 0:
            case OperationType:
            case ForLoopSaveType:
                break;
            default:
                ret_val.error = TooManyStandardCallArgumentsError;
                free(ret_val.array);
                return ret_val;
        }
        return ret_val;
    }
    ret_val.error = BytecodeNotFoundError;
    return ret_val;
}

void QuickPrintStringNewLine(ScriptEnvironment *script_tasks, Script *subject_script, uint64 data_address, uint64 data_length) {
    byte *string_buffer = GenerateValidString(script_tasks, subject_script, data_address, data_length);
    string_buffer = ConcatenateStrings(string_buffer, "\n", &string_buffer);
    ExecuteStandardPrintCallbacks(script_tasks, subject_script, string_buffer); //idea: make the syntax coloring be shades of red to be opposite of lua's blue
}

void QuickPrintString(ScriptEnvironment *script_tasks, Script *subject_script, uint64 data_address, uint64 data_length) {
    ExecuteStandardPrintCallbacks(script_tasks, subject_script, GenerateValidString(script_tasks, subject_script, data_address, data_length)); //idea: make the syntax coloring be shades of red to be opposite of lua's blue
}

void RunPrintCallbacks(ScriptEnvironment *script_tasks, Script *subject_script, byte print_settings, byte *string_a, byte *string_b) {
    if(print_settings & MakeNewLine) {
        if(script_tasks->master.callbacks.print) script_tasks->master.callbacks.print(string_a);
        if(subject_script->callbacks.print && subject_script->id != script_tasks->master.id) subject_script->callbacks.print(string_a);
    } else {
        if(script_tasks->master.callbacks.print) script_tasks->master.callbacks.print(string_b);
        if(subject_script->callbacks.print && subject_script->id != script_tasks->master.id) subject_script->callbacks.print(string_b);
    }
}

int PrintCallDataHandler(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_symbol, byte print_settings) {
    byte *string_pointer = 0;
    uint64 data_address = 0;
    int decimal_length = 8;
    char response[32];
    char *data_type_name = 0;
    byte data_type = 0;
            
    switch(stack_symbol.type) {
        case VoidType:
            if(print_settings & MakeNewLine) {
                string_pointer = malloc(6);
                *(string_pointer + 4) = '\n';
                *(string_pointer + 5) = '\0';
            } else {
                string_pointer = malloc(5);
                *(string_pointer + 4) = '\0';
            }
            memcpy(string_pointer, "void", 4);
            ExecuteStandardPrintCallbacks(script_tasks, subject_script, string_pointer);
            string_pointer = 0;
            break;
        case NullType:
            if(print_settings & MakeNewLine) {
                string_pointer = malloc(6);
                *(string_pointer + 4) = '\n';
                *(string_pointer + 5) = '\0';
            } else {
                string_pointer = malloc(5);
                *(string_pointer + 4) = '\0';
            }
            memcpy(string_pointer, "null", 4);
            ExecuteStandardPrintCallbacks(script_tasks, subject_script, string_pointer);
            string_pointer = 0;
            break;
        case SymbolType:
            data_address = GetPointerStackData(stack_symbol);
            int64 symbol_index = FindSymbolById(subject_script, data_address);
            if(symbol_index != -1) {
                Symbol *temp_symbol = subject_script->values.symbol_table + symbol_index;
                int error_code = 0;
                if(temp_symbol->settings & IsCLike) {
                    stack_symbol.data = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, temp_symbol->size);
                    if(!stack_symbol.data) return FailedToReadPointerError;
                    stack_symbol.type = temp_symbol->type;
                    stack_symbol.sub_type = temp_symbol->sub_type;
                    stack_symbol.length = temp_symbol->size;
                } else {
                    data_address = temp_symbol->address;
                    stack_symbol = GetStackDataFromMemory(script_tasks, subject_script, &data_address);
                }
                if(temp_symbol->type == CPointerType && temp_symbol->sub_type == CharType && temp_symbol->dimension == 1) {
                    uint64 string_length = 0;
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, bit_mode / 8);
                    if(!string_pointer) return CouldNotReadMemoryError;
                    uint64 string_address = *(uint64*) string_pointer;
                    while(1) {
                        byte this_character = ReadScriptMemory(script_tasks, subject_script, string_address + string_length);
                        string_length++;
                        if(this_character == 0) break;
                    }
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, string_address, string_length);
                    if(string_pointer) {
                        if(print_settings & MakeNewLine) {
                            byte *new_string = malloc(string_length + 1);
                            *(new_string + string_length) = '\0';
                            *(new_string + (string_length - 1)) = '\n';
                            memcpy(new_string, string_pointer, string_length - 1);
                            ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, new_string);
                            free(new_string);
                        } else ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, string_pointer);
                    }
                } else if(error_code = PrintCallDataHandler(script_tasks, subject_script, stack_symbol, print_settings)) return error_code;
            }
            break;
        case TemporarySymbolType:
            data_address = GetPointerStackData(stack_symbol);
            int64 debris_index = FindDebris(subject_script, data_address);
            if(debris_index != -1) {
                TemporarySymbol *temp_symbol = subject_script->system.debris + debris_index;
                int error_code = 0;
                if(temp_symbol->settings & IsCLike) {
                    stack_symbol.data = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, temp_symbol->size);
                    if(!stack_symbol.data) return FailedToReadPointerError;
                    stack_symbol.type = temp_symbol->type;
                    stack_symbol.sub_type = temp_symbol->sub_type;
                    stack_symbol.length = temp_symbol->size;
                } else {
                    data_address = temp_symbol->address;
                    stack_symbol = GetStackDataFromMemory(script_tasks, subject_script, &data_address);
                }
                if(temp_symbol->type == CPointerType && temp_symbol->sub_type == CharType && temp_symbol->dimension == 1) {
                    uint64 string_length = 0;
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, temp_symbol->address, bit_mode / 8);
                    if(!string_pointer) return CouldNotReadMemoryError;
                    uint64 string_address = *(uint64*) string_pointer;
                    while(1) {
                        byte this_character = ReadScriptMemory(script_tasks, subject_script, string_address + string_length);
                        string_length++;
                        if(this_character == 0) break;
                    }
                    string_pointer = ReadScriptMemoryRange(script_tasks, subject_script, string_address, string_length);
                    if(string_pointer) {
                        if(print_settings & MakeNewLine) {
                            byte *new_string = malloc(string_length + 1);
                            *(new_string + string_length) = '\0';
                            *(new_string + (string_length - 1)) = '\n';
                            memcpy(new_string, string_pointer, string_length - 1);
                            ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, new_string);
                            free(new_string);
                        } else ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, string_pointer);
                    }
                } else if(error_code = PrintCallDataHandler(script_tasks, subject_script, stack_symbol, print_settings)) return error_code;
                return DeleteDebris(script_tasks, subject_script, debris_index);
            }
            return PushScriptStackData(subject_script, 0, 0);
        case PointerType:
            data_address = GetPointerStackData(stack_symbol);
            if(print_settings & MakeNewLine) {
                StandardPrintUnsignedIntegerNewLine(script_tasks, subject_script, data_address);
            } else {
                StandardPrintUnsignedInteger(script_tasks, subject_script, data_address);
            }
            break;
        case EnumType:
            RunPrintCallbacks(script_tasks, subject_script, print_settings, "enum\n", "enum");
            break;
        case FunctionType:
            RunPrintCallbacks(script_tasks, subject_script, print_settings, "function\n", "function");
            break;
        case StringType:
            if(print_settings & MakeNewLine) {
                QuickPrintStringNewLine(script_tasks, subject_script, stack_symbol.data - script_tasks->memory, stack_symbol.length);
            } else {
                QuickPrintString(script_tasks, subject_script, stack_symbol.data - script_tasks->memory, stack_symbol.length);
            }
            break;
        case CharType:
            if(print_settings & MakeNewLine) {
                string_pointer = malloc(3);
                *(string_pointer + 1) = '\n';
                *(string_pointer + 2) = '\0';
            } else {
                string_pointer = malloc(2);
                *(string_pointer + 1) = '\0';
            }
            *string_pointer = *stack_symbol.data;
            ExecuteStandardPrintCallbacks(script_tasks, subject_script, string_pointer);
            string_pointer = 0;
            break;
        case UnsignedInt8Type:
            if(print_settings & MakeNewLine) {
                StandardPrintUnsignedIntegerNewLine(script_tasks, subject_script, (uint64) *(byte*) stack_symbol.data);
            } else {
                StandardPrintUnsignedInteger(script_tasks, subject_script, (uint64) *(byte*) stack_symbol.data);
            }
            break;
        case UnsignedInt16Type:
            if(print_settings & MakeNewLine) {
                StandardPrintUnsignedIntegerNewLine(script_tasks, subject_script, (uint64) *(uint16*) stack_symbol.data);
            } else {
                StandardPrintUnsignedInteger(script_tasks, subject_script, (uint64) *(uint16*) stack_symbol.data);
            }
            break;
        case UnsignedInt32Type:
            if(print_settings & MakeNewLine) {
                StandardPrintUnsignedIntegerNewLine(script_tasks, subject_script, (uint64) *(uint32*) stack_symbol.data);
            } else {
                StandardPrintUnsignedInteger(script_tasks, subject_script, (uint64) *(uint32*) stack_symbol.data);
            }
            break;
        case CPointerType:
            stack_symbol.type = stack_symbol.sub_type;
            if(stack_symbol.type == StringType) {
                data_address = *(uint64*) stack_symbol.data; //pointer marker
                stack_symbol.length = GetDataTypeLength(stack_symbol.type);
                stack_symbol.data = ReadScriptMemoryRange(script_tasks, subject_script, data_address, stack_symbol.length);
                if(!stack_symbol.data) return MemoryReadError;
                return PrintCallDataHandler(script_tasks, subject_script, stack_symbol, print_settings);
            }
        case UnsignedInt64Type:
            if(print_settings & MakeNewLine) {
                StandardPrintUnsignedIntegerNewLine(script_tasks, subject_script, *(uint64*) stack_symbol.data);
            } else {
                StandardPrintUnsignedInteger(script_tasks, subject_script, *(uint64*) stack_symbol.data);
            }
            break;
        case Int8Type:
            if(print_settings & MakeNewLine) {
                StandardPrintIntegerNewLine(script_tasks, subject_script, (int64) *(char*) stack_symbol.data);
            } else {
                StandardPrintInteger(script_tasks, subject_script, (int64) *(char*) stack_symbol.data);
            }
            break;
        case Int16Type:
            if(print_settings & MakeNewLine) {
                StandardPrintIntegerNewLine(script_tasks, subject_script, (int64) *(int16*) stack_symbol.data);
            } else {
                StandardPrintInteger(script_tasks, subject_script, (int64) *(int16*) stack_symbol.data);
            }
            break;
        case Int32Type:
            if(print_settings & MakeNewLine) {
                StandardPrintIntegerNewLine(script_tasks, subject_script, (int64) *(int32*) stack_symbol.data);
            } else {
                StandardPrintInteger(script_tasks, subject_script, (int64) *(int32*) stack_symbol.data);
            }
            break;
        case Int64Type:
            if(print_settings & MakeNewLine) {
                StandardPrintIntegerNewLine(script_tasks, subject_script, *(int64*) stack_symbol.data);
            } else {
                StandardPrintInteger(script_tasks, subject_script, *(int64*) stack_symbol.data);
            }
            break;
        case FloatType:
            ftoa(*(float32*) stack_symbol.data, (byte*) response, decimal_length);
            if(print_settings & MakeNewLine) {
                char *temp_buffer = ConcatenateStrings((byte*) response, "\n", 0);
                memcpy(response, temp_buffer, 32);
                free(temp_buffer);
            }
            ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, (byte*) response);
            break;
        case DoubleType:
            ftoa((float32) *(float64*) stack_symbol.data, (byte*) response, decimal_length);
            if(print_settings & MakeNewLine) {
                char *temp_buffer = ConcatenateStrings((byte*) response, "\n", 0);
                memcpy(response, temp_buffer, 32);
                free(temp_buffer);
            }
            ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, (byte*) response);
            break;
        case LongDoubleType:
            ftoa((float32) *(float128*) stack_symbol.data, (byte*) response, decimal_length);
            if(print_settings & MakeNewLine) {
                char *temp_buffer = ConcatenateStrings((byte*) response, "\n", 0);
                memcpy(response, temp_buffer, 32);
                free(temp_buffer);
            }
            ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, (byte*) response);
            break;
        case DataType:
            data_type = *stack_symbol.data;
            switch(data_type) {
                case 0:
                    if(print_settings & MakeNewLine) data_type_name = "void\n"; else data_type_name = "void";
                    break;
                case NullType:
                    if(print_settings & MakeNewLine) data_type_name = "null\n"; else data_type_name = "null";
                    break;
                case Int8Type:
                    if(print_settings & MakeNewLine) data_type_name = "int8\n"; else data_type_name = "int8";
                    break;
                case Int16Type:
                    if(print_settings & MakeNewLine) data_type_name = "int16\n"; else data_type_name = "int16";
                    break;
                case Int32Type:
                    if(print_settings & MakeNewLine) data_type_name = "int32\n"; else data_type_name = "int32";
                    break;
                case Int64Type:
                    if(print_settings & MakeNewLine) data_type_name = "int64\n"; else data_type_name = "int64";
                    break;
                case UnsignedInt8Type:
                    if(print_settings & MakeNewLine) data_type_name = "uint8\n"; else data_type_name = "uint8";
                    break;
                case UnsignedInt16Type:
                    if(print_settings & MakeNewLine) data_type_name = "uint16\n"; else data_type_name = "uint16";
                    break;
                case UnsignedInt32Type:
                    if(print_settings & MakeNewLine) data_type_name = "uint32\n"; else data_type_name = "uint32";
                    break;
                case UnsignedInt64Type:
                    if(print_settings & MakeNewLine) data_type_name = "uint64\n"; else data_type_name = "uint64";
                    break;
                case PointerType:
                    if(print_settings & MakeNewLine) data_type_name = "pointer\n"; else data_type_name = "pointer";
                    break;
                case FloatType:
                    if(print_settings & MakeNewLine) data_type_name = "float32\n"; else data_type_name = "float32";
                    break;
                case DoubleType:
                    if(print_settings & MakeNewLine) data_type_name = "float64\n"; else data_type_name = "float64";
                    break;
                case LongDoubleType:
                    if(print_settings & MakeNewLine) data_type_name = "float128\n"; else data_type_name = "float128";
                    break;
                case ObjectType:
                    if(print_settings & MakeNewLine) data_type_name = "object\n"; else data_type_name = "object";
                    break;
                case StructType:
                    if(print_settings & MakeNewLine) data_type_name = "struct\n"; else data_type_name = "struct";
                    break;
                case ClassType:
                    if(print_settings & MakeNewLine) data_type_name = "class\n"; else data_type_name = "class";
                    break;
                case GenericType:
                    if(print_settings & MakeNewLine) data_type_name = "generic\n"; else data_type_name = "generic";
                    break;
                case CPointerType:
                    if(print_settings & MakeNewLine) data_type_name = "c-pointer\n"; else data_type_name = "c-pointer";
                    break;
                case Bool8Type:
                    if(print_settings & MakeNewLine) data_type_name = "bool8\n"; else data_type_name = "bool8";
                    break;
                case Bool16Type:
                    if(print_settings & MakeNewLine) data_type_name = "bool16\n"; else data_type_name = "bool16";
                    break;
                case Bool32Type:
                    if(print_settings & MakeNewLine) data_type_name = "bool32\n"; else data_type_name = "bool32";
                    break;
                case Bool64Type:
                    if(print_settings & MakeNewLine) data_type_name = "bool64\n"; else data_type_name = "bool64";
                    break;
                case ListType:
                    if(print_settings & MakeNewLine) data_type_name = "list\n"; else data_type_name = "list";
                    break;
                case CFunctionType:
                    if(print_settings & MakeNewLine) data_type_name = "asm-function\n"; else data_type_name = "asm-function";
                    break;
                case CustomDataType:
                    if(print_settings & MakeNewLine) data_type_name = "typedef\n"; else data_type_name = "typedef";
                    break;
                default:
                    if(print_settings & MakeNewLine) data_type_name = "data\n"; else data_type_name = "data";
            };
            ExecuteStandardPrintCallbacksNoDelete(script_tasks, subject_script, data_type_name);
            break;
        default:
            RunPrintCallbacks(script_tasks, subject_script, print_settings, "undefined\n", "undefined");
    }
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardPrintCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    return PrintCallDataHandler(script_tasks, subject_script, stack_symbol, 0);
    //return 0; //27; //23; //136999;
}

int StandardSizeofCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_data = PopScriptStackData(subject_script);
    if(!stack_data.data) return StackEmptyError;
    stack_data = ExpandStackData(script_tasks, subject_script, stack_data);
    byte data_length = (byte) stack_data.length;
    switch(stack_data.type) {
        case Bool8Type:
        case Bool16Type:
        case Bool32Type:
        case Bool64Type:
            data_length = GetDataTypeSize(stack_data.type);
    }
    return PushScriptStackData(subject_script, (byte*) &data_length, UnsignedInt8Type);
}

int StandardSpaceofCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_data = PopScriptStackData(subject_script);
    if(!stack_data.data) return StackEmptyError;
    stack_data = ExpandStackData(script_tasks, subject_script, stack_data);
    uint16 extra_length = 0;
    if(stack_data.type == CustomSymbolType) {
        extra_length = 4;
    }
    uint64 data_length = (uint64) stack_data.length + 1 + extra_length;
    if(stack_data.type == PointerType) {
        data_length = GetPointerStackData(stack_data);
        data_length = GetMemoryDataSize(script_tasks, subject_script, &data_length);
    }
    return PushScriptStackData(subject_script, (byte*) &data_length, UnsignedInt64Type);
}

int StandardJumpCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(!stack_symbol.data) return StackEmptyError;
    stack_symbol = ExpandStackData(script_tasks, subject_script, stack_symbol);
    int64 scope_id;
    switch(stack_symbol.type) {
        case PointerType:
            subject_script->values.program_counter = GetPointerStackData(stack_symbol);
            scope_id = GetScopeByAddress(subject_script, subject_script->values.program_counter);
            subject_script->values.scope = scope_id;
            break;
    }
    return PushScriptStack(subject_script, 0, 0);
}

int StandardWaitCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(!stack_symbol.data) return StackEmptyError;
    subject_script->values.waiting_until = GetCurrentOSTime() + CastToFloat64(stack_symbol);
    subject_script->values.state = WaitingState;
    return PushScriptStackData(subject_script, 0, 0);
}

int ExecuteMegaScriptOperation(ScriptEnvironment*, Script*, uint32);

int CallMegaScriptFunction(ScriptEnvironment *script_tasks, Script *subject_script, uint16 parameter_count, uint64 destination_pointer, uint32 operation_save) {
    int error_code = 0;
    int64 last_scope = subject_script->values.scope;
    uint64 starting_point = destination_pointer;
    uint64 data_length = starting_point;
    data_length = GetMemoryDataSize(script_tasks, subject_script, &data_length);
    StackData function_data = GetStackDataFromMemory(script_tasks, subject_script, &destination_pointer);
    switch(function_data.type) {
        case FunctionType:
            int64 parent_scope = GetScopeByAddress(subject_script, starting_point);
            uint64 scope_index = CreateScope(subject_script, starting_point, data_length, parent_scope);
            Scope current_scope = *(subject_script->values.scopes + scope_index);
            subject_script->values.scope = current_scope.id;
            uint64 original_pc = subject_script->values.program_counter;
            subject_script->values.program_counter = destination_pointer;
            uint64 *pc = &subject_script->values.program_counter;
            StackData *data_array = 0;
            if(parameter_count) {
                data_array = malloc(parameter_count * sizeof(StackData));
                for(int i = parameter_count - 1; i >= 0; i--) {
                    StackData *new_data = data_array + i;
                    *new_data = PopScriptStackData(subject_script);
                    int data_length = GetDataTypeLength(new_data->type);
                    byte *new_buffer = malloc(data_length);
                    memcpy(new_buffer, new_data->data, data_length);
                    new_data->data = new_buffer;
                }
            }
            if(ReadScriptMemory(script_tasks, subject_script, *pc) == CallOperator) {
                (*pc)++;
                int count_index = 0;
                while(1) {
                    if(ReadScriptMemory(script_tasks, subject_script, *pc) == CallEndOperator) {
                        (*pc)++;
                        break;
                    }
                    if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, IsFunctionArgument)) return error_code;
                    StackData parameter = PopScriptStackData(subject_script);
                    if(count_index < parameter_count) {
                        StackData new_value = *(data_array + count_index);
                        new_value = ExpandStackData(script_tasks, subject_script, new_value);
                        if(parameter.error) return parameter.error;
                        error_code = AssignData(script_tasks, subject_script, new_value, parameter);
                        PopScriptStackData(subject_script);
                    }
                    count_index++;
                }
            }
            error_code = PushScriptStackData(subject_script, (byte*) &last_scope, Int64Type);
            if(error_code) return error_code;
            error_code = PushScriptStackPointer(subject_script, original_pc);
            if(error_code) return error_code;
            error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType);
            if(error_code) return error_code;
            error_code = PushScriptStackData(subject_script, 0, 0);
            if(error_code) return error_code;
            break;
        default:
            return NotAFunctionError;
    }
    return error_code;
}

byte StackDataIsTrue(ScriptEnvironment *script_tasks, Script *subject_script, StackData stack_data) {
    stack_data = ExpandStackData(script_tasks, subject_script, stack_data);
    if(stack_data.error) return stack_data.error;

    switch(stack_data.type) {
        case Int8Type:
        case Int16Type:
        case Int32Type:
        case Int64Type:
        case UnsignedInt8Type:
        case UnsignedInt16Type:
        case UnsignedInt32Type:
        case UnsignedInt64Type:
            return (byte) (CastToUnsignedInt64(stack_data) != 0);
        case FloatType:
            return (byte) (CastToFloat32(stack_data) != 0.0f);
        case DoubleType:
            return (byte) (CastToFloat64(stack_data) != 0.0f);
        case LongDoubleType:
            return (byte) (CastToFloat128(stack_data) != 0.0f);
    }
}

int StandardLoadCall(ScriptEnvironment *script_tasks, Script *subject_script, byte script_type) {
    FILE *file_ptr;
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(!stack_symbol.data) return StackEmptyError;
    uint64 data_address = 0;

    byte *string_ptr = GetStringFromStackData(script_tasks, subject_script, stack_symbol);
    if(!string_ptr) return StringGenerationError;
    file_ptr = fopen(string_ptr, "r");
    free(string_ptr);
    if(!file_ptr) return UnopenedFileError;
    int chr = 0;
    Script *new_megascript = CreateMegaScript();
    new_megascript->values.name = "Spawned";
    int length = 0;
    while(1) {
        chr = fgetc(file_ptr);
        if(chr == EOF) break;
        new_megascript->values.source = realloc(new_megascript->values.source, length + 1);
        *(new_megascript->values.source + length) = chr;
        length++;
    }
    new_megascript->values.source = realloc(new_megascript->values.source, length + 1);
    *(new_megascript->values.source + length) = '\0';
    AddMegaScriptToEnvironment(script_tasks, new_megascript);
    EnableMegaScript(script_tasks, new_megascript);
    if(script_tasks->settings.bools & ScriptsInheritFromMaster) {
        for(int i = 0; i < script_tasks->master.values.symbol_count; i++) {
            Symbol temp_symbol = *(script_tasks->master.values.symbol_table + i);
            if(CreateSymbol(script_tasks, new_megascript, temp_symbol) == -1) return SymbolDefiningError;
        }
    }
    new_megascript->type = script_type;
    int error_code = 0;
    switch(script_type) {
        case LibraryScriptType:
            new_megascript->rules.privilege_level = script_tasks->settings.default_library_privilege_level;
            if(error_code = InitializeLibrary(script_tasks, new_megascript)) return error_code;
            //printf("%s\n", new_megascript->values.source);
            new_megascript->rules.settings |= IsReadOnly;
            new_megascript->rules.settings &= ~MemoryProtectionOn;
            break;
        case StandardScriptType:
            new_megascript->rules.privilege_level = script_tasks->settings.default_script_privilege_level;
    }
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardIfCall(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save) {
    int error_code = 0;
    unsigned short operator_count = (unsigned short) (operation_save >> 16);
    byte opcode = (byte) ((operation_save >> 8) & 255);
    StackData return_value = PopScriptStackData(subject_script);
    if(return_value.error) return return_value.error;
    byte bool_value = StackDataIsTrue(script_tasks, subject_script, return_value);
    uint64 *pc = &subject_script->values.program_counter;
    uint64 loop_head = *pc;
    uint64 program_counter = *pc;
    int data_value = 1;
    byte bytecode = 0;
    while(data_value) {
        bytecode = ReadScriptMemory(script_tasks, subject_script, program_counter);
        StackData stack_data = GetStackDataFromMemory(script_tasks, subject_script, &program_counter);
        switch(bytecode) {
            case ElseIfOpcode:
            case ElseOpcode:
            case StopOpcode:
                data_value--;
                break;
            case FunctionOpcode:
            case IfOpcode:
            case WhileOpcode:
                data_value++;
                break;
        }
    }
    uint64 data_length = program_counter - loop_head;
    if(bool_value != 0) {
        uint64 scope_index = CreateScope(subject_script, loop_head, data_length, subject_script->values.scope);
        subject_script->values.scope = (subject_script->values.scopes + scope_index)->id;
        operation_save = (operator_count << 16) | (operation_save & HasReturn);
        if(error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType)) return error_code;
        return PushScriptStackData(subject_script, 0, 0);
    } else {
        *pc = program_counter;
        uint64 scope_index = 0;
        switch(bytecode) {
            case ElseOpcode:
                scope_index = CreateScope(subject_script, subject_script->values.program_counter, data_length, subject_script->values.scope);
                subject_script->values.scope = (subject_script->values.scopes + scope_index)->id;
                operation_save = (operator_count << 16) | (opcode << 8) | (operation_save & HasReturn);
                if(error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType)) return error_code;
                return ExecuteMegaScriptOperation(script_tasks, subject_script, 0);
            case ElseIfOpcode:
                operation_save = (operator_count << 16) | (bytecode << 8) | ResumePrevious | (operation_save & HasReturn);
                error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType);
                if(error_code) return error_code;
                return ExecuteMegaScriptOperation(script_tasks, subject_script, HasReturn);
        }
        return PushScriptStackData(subject_script, 0, 0);
    }
    return IfStatementFailureError;
};

int StandardWhileCall(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save) {
    int error_code = 0;
    StackData return_value = PopScriptStackData(subject_script);
    if(return_value.error) return return_value.error;
    return_value = ExpandStackData(script_tasks, subject_script, return_value);
    if(return_value.error) return return_value.error;
    byte bool_value = StackDataIsTrue(script_tasks, subject_script, return_value);
    return_value = PopScriptStackData(subject_script);
    uint64 loop_head = GetPointerStackData(return_value);
    uint64 *pc = &subject_script->values.program_counter;
    uint64 program_counter = *pc;
    unsigned short operator_count = (unsigned short) (operation_save >> 16);
    SkipScopeData(script_tasks, subject_script, &program_counter);
    if(bool_value != 0) {
        uint64 data_length = loop_head - *pc;
        uint64 scope_index = CreateScope(subject_script, loop_head, data_length, subject_script->values.scope);
        subject_script->values.scope = (subject_script->values.scopes + scope_index)->id;
        operation_save = (operator_count << 16) | IsLoop | (operation_save & HasReturn);
    } else {
        *pc = program_counter;
    }
    if(error_code = PushScriptStackPointer(subject_script, loop_head)) return error_code;
    if(error_code = PushScriptStackData(subject_script, (byte*) &operation_save, OperationType)) return error_code;
    return PushScriptStackData(subject_script, 0, 0);
}

int ClimbScope(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    if(subject_script->values.scope != -1) {
        int64 scope_index = GetScopeIndex(&subject_script->values, subject_script->values.scope);
        if(scope_index >= 0) {
            Scope *this_scope = (subject_script->values.scopes + scope_index);
            int64 parent_id = this_scope->parent;
            int64 this_scope_id = subject_script->values.scope;
            subject_script->values.scope = parent_id;
            return DeleteScope(script_tasks, subject_script, this_scope_id);
        }
    }
    return CannotGoHigherError;
}

int StandardReturnCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    uint64 global_return_address = 0;
    uint64 *pc = &subject_script->values.program_counter;
    StackData return_value = PopScriptStackData(subject_script);
    return_value = ExpandStackData(script_tasks, subject_script, return_value);
    StackData loop_data = PopScriptStackData(subject_script);
    while(1) {
        switch(loop_data.type) {
            case OperationType:
                uint32 operation_save = *(uint32*) loop_data.data;
                switch(operation_save & IsCallReturn) {
                    case IsCallReturn:
                        StackData return_address = PopScriptStackData(subject_script);
                        global_return_address = GetPointerStackData(return_address);
                        return_address = PopScriptStackData(subject_script);
                        if(error_code = DeleteScope(script_tasks, subject_script, subject_script->values.scope)) return error_code;
                        subject_script->values.scope = *(int64*) return_address.data;
                        *pc = global_return_address;
                        if(error_code = PushScriptStackData(subject_script, return_value.data, return_value.type)) return error_code;
                        return ExecuteMegaScriptOperation(script_tasks, subject_script, operation_save);
                    default:
                        if(operation_save & IsLoop) {
                            ClimbScope(script_tasks, subject_script);
                            loop_data = PopScriptStackData(subject_script);
                            loop_data = PopScriptStackData(subject_script);
                        } else {
                            ClimbScope(script_tasks, subject_script);
                            loop_data = PopScriptStackData(subject_script);
                            if(error_code = SkipScopeData(script_tasks, subject_script, pc)) return error_code;
                        }
                }
                break;
            case ForLoopSaveType:
                ClimbScope(script_tasks, subject_script);
                loop_data = PopScriptStackData(subject_script);
                break;
            default:
                return InvalidStackDataInReturnError;
        }
    }
    return MissingOperationSaveError;
};

int HandleForLoop(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save, char skip_increment) {
    int error_code = 0;
    if(error_code = PushScriptStackPointerNumber(subject_script, subject_script->values.program_counter, ForLoopSaveType)) return error_code;
    uint64 checker_operation = subject_script->values.program_counter;
    uint64 ending_address = 0;

    if(!skip_increment) {
        if(error_code = SkipStatement(script_tasks, subject_script, &subject_script->values.program_counter)) return error_code;
        if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, IsFunctionArgument)) return error_code;
        StackData end_data = PopScriptStackData(subject_script);
        if(end_data.error) return end_data.error;
        ending_address = subject_script->values.program_counter;
        subject_script->values.program_counter = checker_operation;
    }

    if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, IsFunctionArgument)) return error_code;
    StackData middle_data = PopScriptStackData(subject_script);
    if(middle_data.error) return middle_data.error;
    byte bool_value = StackDataIsTrue(script_tasks, subject_script, middle_data);

    if(bool_value) {
        if(skip_increment) {
            if(error_code = SkipStatement(script_tasks, subject_script, &subject_script->values.program_counter)) return error_code;
        } else {
            subject_script->values.program_counter = ending_address;
        }
    } else {
        StackData end_data = PopScriptStackData(subject_script);
        if(end_data.type != ForLoopSaveType) return ForLoopSaveError;
        if(error_code = SkipScopeData(script_tasks, subject_script, &subject_script->values.program_counter)) return error_code;
        int64 scope_index = GetScopeIndex(&subject_script->values, subject_script->values.scope);
        if(scope_index != -1) {
            Scope *scope_current = subject_script->values.scopes + scope_index;
            subject_script->values.scope = scope_current->parent;
            if(error_code = DeleteScope(script_tasks, subject_script, scope_current->id)) return error_code;
        } else {
            return InvalidScopeError;
        }
    }

    return PushScriptStackData(subject_script, 0, 0);
}

int StandardForCall(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save) {
    int error_code = 0;
    subject_script->values.program_counter++;
    uint64 loop_end = subject_script->values.program_counter;
    if(error_code = SkipScopeData(script_tasks, subject_script, &loop_end)) return error_code;
    uint64 data_length = loop_end - subject_script->values.program_counter;
    uint64 scope_index = CreateScope(subject_script, subject_script->values.program_counter, data_length, subject_script->values.scope);
    subject_script->values.scope = (subject_script->values.scopes + scope_index)->id;
    if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, IsFunctionArgument)) return error_code;
    StackData stack_data = PopScriptStackData(subject_script);
    if(error_code = stack_data.error) return error_code;
    return HandleForLoop(script_tasks, subject_script, operation_save, 1);
}

int HandleCallReturn(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save) {
    int error_code = 0;
    uint64 *pc = &subject_script->values.program_counter;
    StackData return_address = PopScriptStackData(subject_script);
    uint64 global_return_address = GetPointerStackData(return_address);
    return_address = PopScriptStackData(subject_script);
    if(error_code = DeleteScope(script_tasks, subject_script, subject_script->values.scope)) return error_code;
    subject_script->values.scope = *(int64*) return_address.data;
    *pc = global_return_address;
    if(error_code = ExecuteMegaScriptOperation(script_tasks, subject_script, operation_save)) return error_code;
    return error_code;
}

int HandleLoopReturn(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save) {
    uint64 *pc = &subject_script->values.program_counter;
    uint64 global_return_address = 0;
    if(operation_save & IsLoop) {
        StackData return_address = PopScriptStackData(subject_script); //gus staging fake car robbery on jessie is genius manipulation to trick jessie into thinking he gained more honor/respect
        global_return_address = GetPointerStackData(return_address);
        *pc = global_return_address;
    }
    return ClimbScope(script_tasks, subject_script);
}

int StandardContinueCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    uint32 operation_save;
    uint64 *pc = &subject_script->values.program_counter; //make an array to specify if an error is an error or a warning
    StackData loop_data = PopScriptStackData(subject_script);
    while(1) {
        switch(loop_data.type) {
            case OperationType:
                operation_save = *(uint32*) loop_data.data;
                switch(operation_save & IsCallReturn) {
                    case IsCallReturn:
                        return ContinueOutsideOfLoopError;
                    default:
                        if(operation_save & IsLoop) return HandleLoopReturn(script_tasks, subject_script, operation_save);
                }
                ClimbScope(script_tasks, subject_script);
                loop_data = PopScriptStackData(subject_script);
                if(error_code = SkipScopeData(script_tasks, subject_script, pc)) return error_code;
                break;
            case ForLoopSaveType:
                *pc = GetPointerStackData(loop_data);
                return HandleForLoop(script_tasks, subject_script, operation_save, 0);
            default:
                return ContinueOutsideOfLoopError;
        }
    }
}

int EvadeScope(ScriptEnvironment *script_tasks, Script *subject_script) {
    int64 scope_id = subject_script->values.scope;
    
    if(scope_id != -1) {
        Scope *scope_ptr = GetScope(subject_script, scope_id);
        if(scope_ptr) {
            SkipScopeData(script_tasks, subject_script, &subject_script->values.program_counter);
            subject_script->values.scope = scope_ptr->parent; //huge error because I put this after I deleted the pointer data at the next line. c memory can be confusing sometimes lol
            DeleteScope(script_tasks, subject_script, scope_id);
        }
    }
    return 0;
}

int StandardCancelCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    uint32 operation_save = 0;
    uint64 *pc = &subject_script->values.program_counter; //make an array to specify if an error is an error or a warning
    StackData loop_data = PopScriptStackData(subject_script);
    while(1) {
        switch(loop_data.type) {
            case OperationType:
                operation_save = *(uint32*) loop_data.data;
                switch(operation_save & IsCallReturn) {
                    case IsCallReturn:
                        return ContinueOutsideOfLoopError;
                    default:
                        if(operation_save & IsLoop) {
                            ClimbScope(script_tasks, subject_script);
                            loop_data = PopScriptStackData(subject_script);
                            loop_data = PopScriptStackData(subject_script);
                        } else {
                            if(error_code = EvadeScope(script_tasks, subject_script)) return error_code;
                            return PushScriptStackData(subject_script, 0, 0);
                        }
                }
                break;
            case ForLoopSaveType:
                ClimbScope(script_tasks, subject_script);
                loop_data = PopScriptStackData(subject_script);
                break;
            default:
                return ContinueOutsideOfLoopError;
        }
    }
}

int StandardBreakCall(ScriptEnvironment *script_tasks, Script *subject_script, uint32 operation_save) {
    int error_code = 0;
    uint64 *pc = &subject_script->values.program_counter; //make an array to specify if an error is an error or a warning
    StackData loop_data = PopScriptStackData(subject_script);
    while(1) {
        switch(loop_data.type) {
            case OperationType:
                operation_save = *(uint32*) loop_data.data;
                switch(operation_save & IsCallReturn) {
                    case IsCallReturn:
                        return BreakOutsideOfLoopError;
                    default:
                        if(operation_save & IsLoop) {
                            loop_data = PopScriptStackData(subject_script); //maybe put this back
                            if(error_code = EvadeScope(script_tasks, subject_script)) return error_code;
                            return PushScriptStackData(subject_script, 0, 0);
                        }
                }
                ClimbScope(script_tasks, subject_script);
                loop_data = PopScriptStackData(subject_script);
                if(error_code = SkipScopeData(script_tasks, subject_script, pc)) return error_code;
                break;
            case ForLoopSaveType:
                if(error_code = EvadeScope(script_tasks, subject_script)) return error_code;
                return PushScriptStackData(subject_script, 0, 0);
            default:
                return BreakOutsideOfLoopError;
        }
    }
}

int StandardStopCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    int error_code = 0;
    StackData return_data = PopScriptStackData(subject_script);
    uint64 *pc = &subject_script->values.program_counter;
    uint64 global_return_address = 0;
    uint32 operation_save = 0;
    (*pc)++;
    switch(return_data.type) {
        case OperationType:
            operation_save = *(uint32*) return_data.data;
            switch(operation_save & IsCallReturn) {
                case IsCallReturn:
                    if(error_code = HandleCallReturn(script_tasks, subject_script, operation_save)) return error_code;
                    break;
                default:
                    if(error_code = HandleLoopReturn(script_tasks, subject_script, operation_save)) return error_code;
            }
            return PushScriptStackData(subject_script, 0, 0);
        case ForLoopSaveType:
            *pc = GetPointerStackData(return_data);
            return HandleForLoop(script_tasks, subject_script, operation_save, 0);
        case EnumStackSaveType:
            subject_script->system.enum_auto_increment = 0;
            return PushScriptStackData(subject_script, 0, 0);
        case NewDataType:
            subject_script->system.enum_auto_increment = 0;
            return 0;
        default:
            return MissingOperationSaveError;
    }
}

int PrepareFunctionCall(ScriptEnvironment *script_tasks, Script *subject_script, uint16 operator_count, uint32 operation_save) {
    StackData function_symbol = PopScriptStackData(subject_script);
    if(function_symbol.error) return function_symbol.error;
    if(function_symbol.type != PointerType) return FunctionCallValuesScrambledError;
    StackData parameter_symbol = PopScriptStackData(subject_script);
    if(parameter_symbol.error) return parameter_symbol.error;
    if(parameter_symbol.type != UnsignedInt16Type) return FunctionCallValuesScrambledError;
    uint16 parameter_count = *(uint16*) parameter_symbol.data;
    operation_save = (operator_count << 16) | (operation_save & 65280) | IsCallReturn | ResumePrevious | (operation_save & HasReturn) | (operation_save & IsLoop);
    return CallMegaScriptFunction(script_tasks, subject_script, parameter_count, GetPointerStackData(function_symbol), operation_save);
}

int StandardStandardCallCall(ScriptEnvironment *script_tasks, Script *subject_script, uint16 standard_call_id) {
    if(standard_call_id < standard_call_count) {
        //StandardCall *current_call = (StandardCall*) script_tasks->standard_calls + standard_call_id;
        StandardCall *current_call = &default_standard_calls[standard_call_id];
        if(current_call->parameters > subject_script->system.argument_count) {
            return TooFewStandardCallArgumentsError;
        } else if(current_call->parameters < subject_script->system.argument_count) {
            return TooManyStandardCallArgumentsError;
        } else if(current_call->privilege_level > subject_script->rules.privilege_level) {
            return StandardCallPrivilegeError;
        } else if(!current_call->callback) {
            return MissingStandardCallbackError;
        }
        int error_code = 0;
        if(error_code = current_call->callback(script_tasks, subject_script)) return error_code;
    } else {
        return StandardCallIdDoesNotExistError;
    }
    return 0;
}

int StandardArrayAccessCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_symbol = PopScriptStackData(subject_script);
    if(!stack_symbol.data) return StackEmptyError;
    stack_symbol = ExpandStackData(script_tasks, subject_script, stack_symbol);
    int64 scope_id;
    switch(stack_symbol.type) {
        case PointerType:
            subject_script->values.program_counter = GetPointerStackData(stack_symbol);
            scope_id = GetScopeByAddress(subject_script, subject_script->values.program_counter);
            subject_script->values.scope = scope_id;
            break;
    }
    return PushScriptStack(subject_script, 0, 0);
}

void PrintScriptError(ScriptEnvironment *script_tasks, Script *subject_script, int error_code, byte error_mode) {
    byte *error_title = GetErrorModeTitle(error_mode);
    subject_script->values.state = StandbyState;
    byte *error_message = 0;
    if(subject_script->callbacks.error_message && !subject_script->system.is_master) {
        error_message = subject_script->callbacks.error_message(error_code);
    } else if(script_tasks->master.callbacks.error_message) {
        error_message = script_tasks->master.callbacks.error_message(error_code);
    } else {
        error_message = "Error message cannot be displayed due to missing callback. (Sorry)"; //GetErrorMessage(error_code);
    }
    byte *error_string = UnsignedIntegerToString(error_code, 10);
    byte *message_buffer = ConcatenateStrings(error_title, "\nError Code: ", 0);
    message_buffer = ConcatenateStrings(message_buffer, error_string, &message_buffer); //"Error code: " //make a function that counts the end statement opcodes to get the line number
    if(error_mode == ExecutionErrorMode || error_code == EnvironmentErrorMode) {
        message_buffer = ConcatenateStrings(message_buffer, "\nStart: ", &message_buffer);
        free(error_string);
        error_string = UnsignedIntegerToString(subject_script->values.program_starting_point, 10);
        message_buffer = ConcatenateStrings(message_buffer, error_string, &message_buffer);
        message_buffer = ConcatenateStrings(message_buffer, "\nAddress: ", &message_buffer);
        free(error_string);
        error_string = UnsignedIntegerToString(subject_script->values.program_counter, 10);
        message_buffer = ConcatenateStrings(message_buffer, error_string, &message_buffer);
        uint64 statement_count = CountScriptStatements(script_tasks, subject_script, subject_script->values.program_counter);
        if(statement_count) {
            message_buffer = ConcatenateStrings(message_buffer, "\nStatement: ", &message_buffer);
            free(error_string);
            error_string = UnsignedIntegerToString(statement_count, 10);
            message_buffer = ConcatenateStrings(message_buffer, error_string, &message_buffer);
        }
    }
    message_buffer = ConcatenateStrings(message_buffer, "\nInfo: ", &message_buffer);
    message_buffer = ConcatenateStrings(message_buffer, error_message, &message_buffer);
    message_buffer = ConcatenateStrings(message_buffer, "\n", &message_buffer);

    if(subject_script->callbacks.error && !subject_script->system.is_master) subject_script->callbacks.error(message_buffer);
    if(script_tasks->master.callbacks.error) {
        message_buffer = ConcatenateStrings(message_buffer, "Script: \"", &message_buffer);
        message_buffer = ConcatenateStrings(message_buffer, subject_script->values.name, &message_buffer);
        message_buffer = ConcatenateStrings(message_buffer, "\"\nId: ", &message_buffer);
        free(error_string);
        error_string = UnsignedIntegerToString(subject_script->id, 10);
        message_buffer = ConcatenateStrings(message_buffer, error_string, &message_buffer);
        message_buffer = ConcatenateStrings(message_buffer, "\n", &message_buffer);
        script_tasks->master.callbacks.error(message_buffer);
    }
    
    free(error_string);
    free(message_buffer);
}

