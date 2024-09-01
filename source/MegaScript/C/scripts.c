Script *CreateMegaScript();

ScriptEnvironment *CreateMegaScriptEnvironmentWithSettings(unsigned int passed_settings) {
    ScriptEnvironment *master_script_container = malloc(sizeof(ScriptEnvironment));
    memset(master_script_container, 0, sizeof(ScriptEnvironment));
    master_script_container->settings.default_script_privilege_level = LowPrivilege;
    master_script_container->settings.default_module_privilege_level = LowPrivilege;
    master_script_container->settings.default_library_privilege_level = MediumPrivilege;
    master_script_container->master.rules.script_timeout = default_script_timeout_megascript;
    if(master_script_container == 0) return 0;
    master_script_container->settings.bools = passed_settings;
    if(passed_settings & EnvironmentUsesRealAddresses) {
        master_script_container->memory = 0;
    } else {
        master_script_container->memory = malloc(default_min_memory_megascript);
        if(master_script_container->memory == 0) return 0;
        master_script_container->memory_size = default_min_memory_megascript;
        master_script_container->min_memory = default_min_memory_megascript;
        master_script_container->max_memory = default_max_memory_megascript;
        master_script_container->memory_reserve = default_reserve_memory_megascript;
        master_script_container->memory_manager.sections = 0;
        master_script_container->memory_manager.count = 0;
    }
    master_script_container->master.id = script_id_megascript;
    script_id_megascript++;
    master_script_container->master.values.state = StandbyState;
    master_script_container->master.system.operator_max = megascript_operator_storage_size;
    master_script_container->master.system.is_master = 1;
    master_script_container->master.values.name = "master";
    master_script_container->master.callbacks.error_message = GetErrorMessage;
    master_script_container->master.rules.privilege_level = MasterPrivilege;
    master_script_container->master.values.scope = -1;
    master_script_container->keyword_count = 1;
    master_script_container->keywords = malloc(master_script_container->keyword_count * sizeof(Keyword));
    master_script_container->settings.bools |= ScriptsInheritFromMaster;
    Keyword default_keywords[] = {
        {UnsignedInt8Type, 1, "current_os", &current_os}
    };
    memcpy(master_script_container->keywords, default_keywords, master_script_container->keyword_count * sizeof(Keyword));
    master_script_container->operator_info = malloc(256 * sizeof(OperatorInformation)); //1024 bytes in total
    memset(master_script_container->operator_info, 0, 256 * sizeof(OperatorInformation));
    for(int i = 0; i < operator_configuration_count; i++) {
        byte current_operator = operator_configuration[i].key;
        OperatorInformation *current_info = master_script_container->operator_info + current_operator;
        current_info->bytecode_type = OperatorBytecodeType;
        current_info->type = operator_configuration[i].value;
        for(int j = 0; j < precedence_groups_count; j++) {
            PrecedenceGroup current_group = precedence_groups[j];
            for(int k = 0; k < current_group.operator_count; k++) {
                if(current_group.operators[k] == current_operator) {
                    current_info->associativity = current_group.associativity;
                    current_info->precedence = precedence_groups_count - j; //Precedence starts at one.
                }
            }
        }
    }
    for(int i = 0; i < syntax_bytecodes_count; i++) {
        (master_script_container->operator_info + syntax_bytecodes[i])->bytecode_type = SyntaxBytecodeType;
    }
    return master_script_container;
}

extern ScriptEnvironment *CreateMegaScriptEnvironment() {
    return CreateMegaScriptEnvironmentWithSettings(0);
}

extern ScriptEnvironment *CreateScriptEnvironment() {
    return CreateMegaScriptEnvironmentWithSettings(0);
}

extern ScriptEnvironment *CreateNonVirtualizedScriptEnvironment() {
    return CreateMegaScriptEnvironmentWithSettings(EnvironmentUsesRealAddresses);
}

int DeleteMegaScriptEnvironment(ScriptEnvironment *master_script_container) {
    for(int i = 0; i < master_script_container->keyword_count; i++) {
        Keyword current_kw = *(master_script_container->keywords + i);
        free(current_kw.value);
    }
    free(master_script_container->keywords);
    free(master_script_container->operator_info);
    free(master_script_container->memory);
    free(master_script_container->master.values.symbol_table);
    free(master_script_container->master.values.source);
    free(master_script_container->master.system.operator_storage);
    free(master_script_container->scripts);
    free(master_script_container);
    return 1;
}

int AddMegaScriptToEnvironment(ScriptEnvironment *master_script_container, Script *subject_script) {
    master_script_container->scripts = (master_script_container->scripts == 0) ? malloc((master_script_container->script_count + 1) * sizeof(Script**)) : realloc(master_script_container->scripts, (master_script_container->script_count + 1) * sizeof(Script**));
    *(master_script_container->scripts + master_script_container->script_count) = subject_script;
    master_script_container->script_count += 1;
    if(subject_script->values.state == RunningState) subject_script->values.started_at = GetCurrentOSTime();
    return 1;
}

int GetMegaScriptIndex(ScriptEnvironment *master_script_container, Script *subject_script) {
    for(int i = 0; i < master_script_container->script_count; i++) {
        if((*(Script**)(master_script_container->scripts + i))->id == subject_script->id) return i;
    }
    return -1;
};

int RemoveMegaScriptFromEnvironment(ScriptEnvironment *master_script_container, Script *subject_script) {
    if(master_script_container->script_count > 0) {
        int index = GetMegaScriptIndex(master_script_container, subject_script);
        if(index > -1) {
            return RemoveArrayMember((byte**) &master_script_container->scripts, &master_script_container->script_count, (uint64) index, sizeof(void**));
        }
    }
    return 0;
}

int InheritSymbolsFromMaster(ScriptEnvironment *script_tasks, Script *subject_script) {
    for(int i = 0; i < script_tasks->master.values.symbol_count; i++) {
        Symbol temp_symbol = *(script_tasks->master.values.symbol_table + i);
        int64 symbol_index = CreateSymbol(script_tasks, subject_script, temp_symbol);
        if(symbol_index == -1) return SymbolDefiningError;
        Symbol *second_temp_symbol = subject_script->values.symbol_table + symbol_index;
        second_temp_symbol->scope = temp_symbol.scope;
    }
    return 0;
}

int AddObjectClassToEnvironment(ScriptEnvironment *script_tasks, Symbol *class_data, uint64 class_id, byte *class_name, uint32 content_size, byte privilege_level, void *preset) {
    ObjectClassInformation new_info = {0};
    uint64 class_name_length = strlen(class_name);
    new_info.name = malloc(class_name_length + 1);
    new_info.content_size = content_size;
    memcpy(new_info.name, class_name, class_name_length);
    *(new_info.name + class_name_length) = '\0';
    new_info.class = class_id;
    new_info.content = ConvertSymbolToDebris(*class_data);
    new_info.privilege_level = privilege_level;
    new_info.preset = preset;
    script_tasks->object_class_info = realloc(script_tasks->object_class_info, (script_tasks->object_class_count + 1) * sizeof(ObjectClassInformation));
    *(script_tasks->object_class_info + script_tasks->object_class_count) = new_info;
    script_tasks->object_class_count++;
    return 0;
}

int RegisterObjectClass(ScriptEnvironment *script_tasks, Symbol *class_data, uint64 class_id, byte *class_name, uint32 content_size, byte privilege_level, void *preset) {
    return AddObjectClassToEnvironment(script_tasks, class_data, class_id, class_name, content_size, privilege_level, preset);
}

ObjectClassInformation *FindObjectClass(ScriptEnvironment *script_tasks, uint64 class_id) {
    for(uint64 i = 0; i < script_tasks->object_class_count; i++) {
        ObjectClassInformation *this_info = script_tasks->object_class_info + i;
        if(this_info->class == class_id) return this_info;
    }
    return 0;
}

int AddCallbackToObjectClass(ScriptEnvironment *script_tasks, uint64 class_id, byte *callback_name, void *passed_callback) {
    ObjectClassInformation *new_info = FindObjectClass(script_tasks, class_id);
    if(new_info) {
        new_info->callbacks = realloc(new_info->callbacks, sizeof(ObjectMethod) * (new_info->callbacks_count + 1));
        ObjectMethod *callback = (new_info->callbacks + new_info->callbacks_count);
        memset(callback, 0, sizeof(ObjectMethod));
        callback->callback = passed_callback;
        callback->privilege_level = LowPrivilege;
        callback->name = callback_name;
        new_info->callbacks_count++;
        return 0;
    }
    return ObjectDoesNotExistError;
}

int InitializeEnvironmentSymbols(ScriptEnvironment *script_tasks, SymbolInitializer *symbols, unsigned int symbol_count) {
    int error_code = 0;
    for(unsigned int i = 0; i < symbol_count; i++) {
        SymbolInitializer *this_symbol = symbols + i;
        int64 index = 0;
        switch(this_symbol->type) {
            case ObjectClassType:
                ClassInformationInitializer *this_class = (ClassInformationInitializer*) this_symbol->content;
                index = FindSymbol(script_tasks, &script_tasks->master, this_class->typedef_name, strlen(this_class->typedef_name) - 1);
                if(index == -1) return CouldNotFindClassTypedefError;
                Symbol *class_symbol = malloc(sizeof(Symbol));
                *class_symbol = *(script_tasks->master.values.symbol_table + index);
                if(error_code = RegisterObjectClass(script_tasks, class_symbol, this_class->class, this_class->name, this_class->size, this_class->privilege_level, this_class->preset)) return error_code;
                for(unsigned int j = 0; j < this_class->method_count; j++) {
                    ObjectMethod *this_method = this_class->methods + j;
                    AddCallbackToObjectClass(script_tasks, this_class->class, this_method->name, this_method->callback);
                };
                break;
            case StructType:
                StructInitializer *this_struct = (StructInitializer*) this_symbol->content;
                Symbol *element_array = malloc(sizeof(Symbol) * this_struct->element_count);
                for(unsigned int k = 0; k < this_struct->element_count; k++) {
                    StructElementInitializer *this_element = this_struct->elements + k;
                    *(element_array + k) = CreateStructElementSymbol(script_tasks, &script_tasks->master, this_element->name, this_element->type, this_element->typedef_name);
                };
                Symbol struct_typedef_struct = CreateStructTypedefSymbol(script_tasks, &script_tasks->master, this_struct->name, element_array, this_struct->element_count);
                CreateSymbol(script_tasks, &script_tasks->master, struct_typedef_struct);
                free(element_array);
                break;
            case Bool8Type:
            case Bool16Type:
            case Bool32Type:
            case Bool64Type:
                BoolInitializer *this_bool = (BoolInitializer*) this_symbol->content;
                if(this_bool->element_count > GetDataTypeSize(this_symbol->type) * 8) return TooManyBoolElementsError;
                element_array = malloc(sizeof(Symbol) * this_bool->element_count);
                for(unsigned int k = 0; k < this_bool->element_count; k++) {
                    BoolElementInitializer *this_element = this_bool->elements + k;
                    *(element_array + k) = CreateStructElementSymbol(script_tasks, &script_tasks->master, this_element->name, PointerType, 0);
                };
                Symbol bool_typedef_struct = CreateBoolTypedefSymbol(script_tasks, &script_tasks->master, this_bool->name, element_array, this_bool->element_count, this_symbol->type);
                CreateSymbol(script_tasks, &script_tasks->master, bool_typedef_struct);
                free(element_array);
                break;
            case CFunctionType:
                CFunctionInitializer *this_function = (CFunctionInitializer*) this_symbol->content;
                if(error_code = LinkCallbackToScript(script_tasks, &script_tasks->master, this_function->name, this_function->callback)) return error_code;
                break;
            default:
                BasicDataInitializer *this_initializer = (BasicDataInitializer*) this_symbol->content;
                Symbol basic_symbol = CreateBasicDataTypeSymbol(script_tasks, &script_tasks->master, this_initializer->name, this_symbol->type, this_initializer->data);
                CreateSymbol(script_tasks, &script_tasks->master, basic_symbol);
        }
    }
    return 0;
}

Script *CreateMegaScript() {
    Script *subject_script = malloc(sizeof(Script));
    memset(subject_script, 0, sizeof(Script));
    subject_script->id = script_id_megascript;
    script_id_megascript++;
    subject_script->values.state = StandbyState;
    subject_script->values.name = "Unnamed_Script";
    subject_script->system.operator_max = megascript_operator_storage_size;
    subject_script->rules.settings |= MemoryProtectionOn;
    return subject_script;
}

Script *CreateScript() {
    return CreateMegaScript();
}

int ResetMegaScript(Script *subject_script) {
    free(subject_script->values.stack);
    subject_script->values.stack = 0;
    free(subject_script->system.operator_storage);
    subject_script->system.operator_storage = 0;
    free(subject_script->values.scopes);
    subject_script->values.scopes = 0;
    subject_script->values.program_counter = subject_script->values.program_starting_point;
    subject_script->values.stack_pointer = 0;
    subject_script->system.operator_count = 0;
    subject_script->values.scope = -1;
    return 0;
}

int DeleteMegaScript(Script *subject_script) {
    ResetMegaScript(subject_script);
    free(subject_script->values.source);
    subject_script->values.source = 0;
    free(subject_script->values.symbol_table); //make sure to remove all memory sections owned by the script before deleting it too
    subject_script->values.symbol_table = 0;
    free(subject_script);
    return 0;
}

int DisableMegaScript(Script *subject_script) {
    subject_script->values.state = DisabledState;
    free(subject_script->values.symbol_table);
    subject_script->values.symbol_table = 0;
    ResetMegaScript(subject_script);
    return 0;
}

int ParseMegaScriptSourceCode(ScriptEnvironment*, Script*);

int EnableMegaScript(ScriptEnvironment *script_tasks, Script *subject_script) {
    switch(subject_script->values.state) {
        case StandbyState:
        case DisabledState:
        case FinishedState:
            for(int i = 0; i < subject_script->values.symbol_count; i++) {
                (subject_script->values.symbol_table + i)->created_at = 0;
            }
            if(!ParseMegaScriptSourceCode(script_tasks, subject_script)) {
                subject_script->values.program_starting_point = subject_script->values.program_counter;
                subject_script->values.stack = malloc(megascript_stack_size);
                subject_script->system.operator_storage = malloc(megascript_operator_storage_size);
                subject_script->system.enum_auto_increment = 0;
                subject_script->values.started_at = GetCurrentOSTime();
                subject_script->values.state = RunningState;
                return 1;
            }
    }
    return 0;
}

int64 AddKeywordToEnvironment(ScriptEnvironment *script_tasks, Keyword passed_kw) {
    script_tasks->keywords = realloc(script_tasks->keywords, script_tasks->keyword_count + 1);
    *(script_tasks->keywords + script_tasks->keyword_count) = passed_kw;
    script_tasks->keyword_count++;
    return script_tasks->keyword_count - 1;
}

int CheckMegaScriptEvents(ScriptEnvironment *script_tasks, Script *subject_script) {
    //coming soon
    return 0;
}

int ExecuteMegaScriptStatement(ScriptEnvironment*, Script*);

int ExecuteMegaScript(ScriptEnvironment *script_tasks, Script *subject_script) {
    uint64 *pc = &subject_script->values.program_counter;
    int error_code = CheckMegaScriptEvents(script_tasks, subject_script);
    if(error_code) return error_code;
    return ExecuteMegaScriptStatement(script_tasks, subject_script);
}

int64 FindScriptByName(ScriptEnvironment *script_tasks, byte *string_buffer) {
    for(int i = 0; i < script_tasks->script_count; i++) {
        Script *selected_script = *(script_tasks->scripts + i);
        if(strcmp(selected_script->values.name, string_buffer) == 0) {
            return i;
        }
    }
    return -1;
}

void DisableEnvironmentSetting(ScriptEnvironment *script_tasks, unsigned int chosen_setting) {
    script_tasks->settings.bools &= ~chosen_setting;
}

void EnableEnvironmentSetting(ScriptEnvironment *script_tasks, unsigned int chosen_setting) {
    script_tasks->settings.bools |= chosen_setting;
}