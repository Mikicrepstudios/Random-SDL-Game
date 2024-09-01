int AddChild(Object *parent, Object *child) {
    parent->children = (parent->children == 0) ? malloc((parent->count + 1) * sizeof(void**)) : realloc(parent->children, (parent->count + 1) * sizeof(void**)); //(parent->children == 0) ? malloc((parent->count + 1) * sizeof(void*)) : realloc(parent->children, (parent->count + 1) * sizeof(void*)); // 
    *(parent->children + parent->count) = child;
    child->parent = parent;
    parent->count += 1;
    return 1;
}

int GetChildIndex(Object *parent, Object *child) {
    for(int i = 0; i < parent->count; i++) {
        if((*(Object**)(parent->children + i))->id == child->id) return i;
    }
    return -1;
};

int RemoveChild(Object *parent, Object *child) {
    if(parent->count > 0) {
        int64 index = GetChildIndex(parent, child);
        if(index > -1) {
            RemoveArrayMember((byte**) &parent->children, &parent->count, index, sizeof(void**));
            return 1;
        }
    }
    return 0;
}

int SetObjectParent(Object *object, Object *parent) {
    if(object->parent) RemoveChild(object->parent, object);
    AddChild(parent, object);
    return 1;
}

Object* CreateObject(char name[], uint64 class, void *data) {
    Object *new_object = malloc(sizeof(Object));
    memset(new_object, 0, sizeof(Object));
    new_object->name = name;
    new_object->class = class;
    new_object->id = current_object_id;
    new_object->content = data;
    current_object_id++;
    return new_object;
}

int GetObjectParent(ScriptEnvironment *script_tasks, Script *subject_script, Object *current_object) {
    uint64 parent_object = (uint64) current_object->parent; //pointer marker
    if(!parent_object) return PushScriptStackData(subject_script, 0, NullType);
    return PushScriptStackData(subject_script, (byte*) &parent_object, ObjectType);
}

int GetObjectName(ScriptEnvironment *script_tasks, Script *subject_script, Object *current_object) {
    Object *parent_object = current_object->parent;
    if(!parent_object) return PushScriptStackData(subject_script, 0, NullType);
    return PushScriptStackData(subject_script, (byte*) parent_object, ObjectType);
}

int GetObjectCount(ScriptEnvironment *script_tasks, Script *subject_script, Object *current_object) {
    return PushScriptStackData(subject_script, (byte*) &current_object->count, UnsignedInt64Type); //pointer marker
}

ObjectClassInformation *GetObjectClassInformation(ScriptEnvironment *script_tasks, int class) {
    for(int i = 0; i < script_tasks->object_class_count; i++) {
        ObjectClassInformation *this_info = script_tasks->object_class_info + i;
        if(this_info->class == class) return this_info;
    }
    return 0;
}

int SetObjectParentCallback(ScriptEnvironment *script_tasks, Script *subject_script, Object *current_object) {
    StackData popped_data = PopScriptStackData(subject_script);
    ObjectClassInformation *this_info = GetObjectClassInformation(script_tasks, current_object->class);
    if(this_info) if(this_info->privilege_level > subject_script->rules.privilege_level) return PrivilegeLevelError;
    if(popped_data.error) return popped_data.error;
    if(popped_data.type != ObjectType) return AttemptedToSetObjectParentToNonObjectError;
    Object *new_parent = (Object*) *(uint64*) popped_data.data; //pointer marker
    SetObjectParent(current_object, new_parent);
    return PushScriptStackData(subject_script, 0, 0);
}

int DestroyObjectCallback(ScriptEnvironment *script_tasks, Script *subject_script, Object *current_object) {
    ObjectClassInformation *this_info = GetObjectClassInformation(script_tasks, current_object->class);
    if(this_info) if(this_info->privilege_level > subject_script->rules.privilege_level) return PrivilegeLevelError;
    return DestroyObject(script_tasks, current_object);
}

#define root_object_method_count 5
static ObjectMethod root_object_methods[] = {
    {"Parent", LowPrivilege, GetObjectParent},
    {"Name", LowPrivilege, GetObjectName},
    {"Count", LowPrivilege, GetObjectCount},
    {"SetParent", LowPrivilege, SetObjectParentCallback, FunctionMethodType},
    {"Destroy", LowPrivilege, DestroyObjectCallback, FunctionMethodType}
};

int64 WordIsObjectMethod(byte *string, byte length) {
    short search_size = (short) length + 1;
    for(int i = 0; i < root_object_method_count; i++) {
        short member_size = strlen(root_object_methods[i].name);
        if(root_object_methods[i].type == FunctionMethodType) continue;
        if(member_size == search_size) if(CompareSymbolStrings(root_object_methods[i].name, string, search_size)) return i;
    }
    return -1;
}

Object *WordIsObjectChild(Object *inspect_object, byte *string, byte length) {
    if(!inspect_object) return 0;
    short search_size = (short) length + 1;
    if(!inspect_object->children) return 0;
    for(uint64 i = 0; i < inspect_object->count; i++) {
        Object *this_child = *(inspect_object->children + i);
        if(this_child) {
            short member_size = strlen(this_child->name);
            if(member_size == search_size) if(CompareSymbolStrings(this_child->name, string, search_size)) return this_child;
        }
    }
    return 0;
}

void *WordIsObjectCallback(ScriptEnvironment *script_tasks, ObjectClassInformation *class_info, byte *string, byte length) {
    short search_size = (short) length + 1;
    for(uint64 i = 0; i < root_object_method_count; i++) {
        if(search_size == strlen(root_object_methods[i].name)) if(CompareSymbolStrings(root_object_methods[i].name, string, search_size)) return root_object_methods[i].callback;
    }
    if(class_info) {
        for(uint64 i = 0; i < class_info->callbacks_count; i++) {
            ObjectMethod *callback = (ObjectMethod*) class_info->callbacks + i;
            if(!callback) return 0;
            short member_size = strlen(callback->name);
            if(member_size == search_size) if(CompareSymbolStrings((byte*) callback->name, string, member_size)) return callback->callback;
        }
    }
    return 0;
}

ObjectClassInformation *GetObjectClassInformationByName(ScriptEnvironment *script_tasks, char *string_name) {
    for(int i = 0; i < script_tasks->object_class_count; i++) {
        ObjectClassInformation *this_info = script_tasks->object_class_info + i;
        if(strcmp(this_info->name, string_name) == 0) return this_info;
    }
    return 0;
}

int RemoveObjectFromScript(ScriptEnvironment *script_tasks, Script *subject_script, Object *subject_object) {
    for(uint64 i = 0; i < subject_script->values.symbol_count; i++) {
        Symbol *this_symbol = subject_script->values.symbol_table + i;
        if(this_symbol->type == ObjectType) if(this_symbol->address == (uint64) subject_object) this_symbol->address = 0;
    }
    byte *stack_pointer = subject_script->values.stack + subject_script->values.stack_pointer;
    int multiply_factor = bit_mode / 8;
    while(stack_pointer > subject_script->values.stack) {
        byte data_type = *stack_pointer;
        stack_pointer--;
        switch(data_type) {
            case ObjectType:
                stack_pointer -= multiply_factor;
                memset(stack_pointer, 0, multiply_factor);
                break;
            case StructType:
                int64 size = GetRecursiveStructSize(script_tasks, subject_script, *(uint64*) stack_pointer); //pointer marker
                if(size < 0) return 0;
                stack_pointer -= size;
                break;
            default:
                stack_pointer -= GetDataTypeLength(data_type);
        }
    }
    return 0;
}

int DestroyObject(ScriptEnvironment *script_tasks, Object *subject_object) {
    if(!subject_object) return 0;
    if(subject_object->parent) RemoveChild(subject_object->parent, subject_object);
    for(unsigned int i = 0; i < subject_object->count; i++) DestroyObject(script_tasks, *(subject_object->children + i));
    for(uint64 i = 0; i < script_tasks->script_count; i++) RemoveObjectFromScript(script_tasks, *(script_tasks->scripts + i), subject_object);
    RemoveObjectFromScript(script_tasks, &script_tasks->master, subject_object);
    return 0;
}