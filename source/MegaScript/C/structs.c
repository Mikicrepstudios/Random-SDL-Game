//█████ █████ ████  █   █  ████ █████ █████ 
//█       █   █   █ █   █ █       █   █     
//█████   █   ████  █   █ █       █   █████ 
//    █   █   █   █ █   █ █       █       █ 
//█████   █   █   █ █████  ████   █   █████ 

typedef struct{
    unsigned char *name;
    int class;
    long long unsigned int count;
    long long unsigned int id;
    void *content;
    void **children;
    void *parent;
} Object; //STATIC VELOCITY, DIRECTORIES, BALL CUBE AND CYLYNDER COLLISSIONS, AND MUCH MORE

typedef struct{
    int type;
    int error;
    char is_signed;
    char is_static; //this gets set to 1 when a size specifier is used (ex. b, w, d, q)
    long long unsigned int bit_length; //this member will be used for both regular bit sizes, and pseudo whole number sizes
    long long unsigned int decimal_length;
    long long unsigned int whole_number;
    long double floating_point;
    unsigned char *whole_number_bytes;
    unsigned char *decimal_bytes;
} NumberContainer;

typedef struct{
    int brackets_entered;
    int brackets_skipped;
} OperatorContainer;

typedef struct{
    unsigned char privilege_level;
    unsigned char settings;
    double script_timeout;
} ScriptRules;

typedef struct{
    unsigned char dimension; //used by c-like pointers to tell how many pointers are contained
    unsigned char array_dimension;
    unsigned char length;
    unsigned char type;
    unsigned char sub_type;
    unsigned int hash;
    unsigned int size;
    unsigned int privilege_level;
    long long int scope;
    long long unsigned int id;
    long long unsigned int address; //used by labels
    long long unsigned int created_at;
    long long unsigned int name; //must be 0 terminated
    long long unsigned int sub_size;
    long long unsigned int list_pointer; //pointer marker
    unsigned char settings;
} Symbol;

typedef struct{
    unsigned char dimension;
    unsigned char array_dimension;
    unsigned char type;
    unsigned char sub_type;
    unsigned int size;
    long long unsigned int id;
    long long unsigned int address;
    long long unsigned int list_pointer;
    unsigned char settings;
} TemporarySymbol;

typedef struct{
    unsigned char *name;
    int class;
    int inherited_class;
    void *callbacks;
    unsigned char privilege_level;
    long long unsigned int callbacks_count;
    unsigned int content_size;
    void *preset;
    TemporarySymbol content;
} ObjectClassInformation;

typedef struct{
    long long int id;
    long long int parent;
    long long unsigned int address;
    long long unsigned int length;
} Scope;

typedef struct{
    unsigned char type;
    unsigned char size;
    unsigned char *name;
    unsigned char *value;
} Keyword;

typedef struct{
    unsigned char key;
    unsigned char value;
} KeyValue;

typedef struct{
    unsigned char *name;
    unsigned char value;
} Word;

typedef struct{
    int state;
    unsigned char *source;
    unsigned char *stack;
    unsigned char *name;
    long long int scope;
    long long int stack_pointer;
    long long unsigned int program_starting_point;
    long long unsigned int program_counter;
    long long unsigned int scope_count;
    long long unsigned int symbol_count;
    long long unsigned int definition_count;
    unsigned int privilege_level;
    double started_at;
    double waiting_until;
    Symbol *symbol_table; //variables for sub scopes are removed once the scope is left. their contents will also be //freed.
    Scope *scopes;
    Object *object;
    Keyword *definitions;
} ScriptComponents;

typedef struct{
    unsigned char *operator_storage;
    unsigned char is_master;
    unsigned int operator_count;
    unsigned int operator_max;
    unsigned int address32; //temporary variable used to not have to malloc more memory for 32 bit addresses
    unsigned char settings;
    int line;
    int column;
    long long int scope_id_auto_increment;
    unsigned int thread;
    long long unsigned int append_count;
    long long unsigned int *append_jumps;
    long long unsigned int current_append;
    long long unsigned int enum_auto_increment;
    long long unsigned int enum_value_increment;
    long long unsigned int debris_count;
    unsigned short argument_count;
    TemporarySymbol *debris; //for temporary memory allocation used by string concatenation and more (todo)
} ScriptSystem;

typedef struct{
    unsigned char *data; //must be 0 terminated
    unsigned char type; //used by labels
    unsigned int sub_type;
    long long unsigned int length;
    unsigned char settings;
    int error;
} StackData;

typedef struct{
    int count;
    int error;
    StackData *array;
} StackDataArray;

typedef struct{
    unsigned char *data;
    long long unsigned int pointer; //pointer marker
    long long unsigned int size;
    int error;
} StructStackData;

typedef struct{
    int (*print)(char*);
    int (*info)(char*);
    int (*warning)(char*);
    int (*error)(char*);
    char *(*error_message)(int);
} ScriptCallbacks;

typedef struct{
    unsigned char type;
    unsigned int id;
    Object *object;
    ScriptRules rules;
    ScriptComponents values;
    ScriptCallbacks callbacks;
    ScriptSystem system;
} Script;

typedef struct{
    int length;
    int type;
    long long unsigned int address; //used by labels
    unsigned char *name;
} UnparsedVariable;

typedef struct{
    long long unsigned int id;
    UnparsedVariable *variables;
} ScopeManager;

typedef struct{
    long long unsigned int pointer; //must be changed to an int so reallocing the environment block translates
    long long unsigned int length;
    Script *owner;
} MemorySection;

typedef struct{
    MemorySection *sections;
    MemorySection *cache;
    long long unsigned int count;
    unsigned int busy;
} MemoryManager;

typedef struct{
    long long unsigned int symbol_id_auto_increment;
    long long unsigned int object_id_auto_increment;
} ScriptEnvironmentSystem;

typedef struct{
    unsigned char default_script_privilege_level;
    unsigned char default_library_privilege_level;
    unsigned char default_module_privilege_level;
    unsigned int bools;
} ScriptEnvironmentSettings;

typedef struct{
    unsigned char bytecode_type; //Unused since the information array uses index to specify the operator. Maybe use this later on for something else.
    unsigned char precedence;
    unsigned char type;
    unsigned char associativity; //This just means which direction it's executed in. (Right-to-left, or left-to-right.)
} OperatorInformation;

typedef struct{
    unsigned short code;
    unsigned char privilege;
    unsigned char arguments; //in the future, 255 might be used to signify unlimited possible arguments 
} StandardCallInformation;

typedef struct{
    unsigned char *memory;
    long long unsigned int memory_reserve;
    long long unsigned int min_memory;
    long long unsigned int max_memory;
    long long unsigned int memory_size;
    long long unsigned int script_count;
    long long unsigned int keyword_count;
    long long unsigned int object_class_count;
    //unsigned short standard_calls_count;
    Keyword *keywords;
    MemoryManager memory_manager;
    Script master;
    Script **scripts;
    Object *root;
    ObjectClassInformation *object_class_info;
    OperatorInformation *operator_info;
    //void *standard_calls;
    ScriptEnvironmentSystem system;
    ScriptEnvironmentSettings settings;
} ScriptEnvironment;

typedef struct{
    char val1_8;
    unsigned char val1_8_u;
    short int val1_16;
    unsigned short int val1_16_u;
    int val1_32;
    unsigned int val1_32_u;
    long long int val1_64;
    long long unsigned int val1_64_u;
    float f1_32;
    double f1_64;
    long double f1_128;
} CalculationValues;

typedef struct{
    unsigned char code;
    unsigned char privilege;
    unsigned char arguments; //in the future, 255 might be used to signify unlimited possible arguments 
} BytecodeInformation;
//Higher precedence has a lower index value. This means that the first group in the array is the highest precedence.
typedef struct{
    unsigned char associativity; //Zero for left-to-right. One for right-to-left. (Direction order of execution)
    unsigned char operator_count;
    unsigned char *operators;
} PrecedenceGroup;

typedef struct{
    char *name;
    unsigned short parameters;
    unsigned int privilege_level;
    int (*callback) (ScriptEnvironment*, Script*);
} StandardCall;

typedef struct{
    char *name;
    unsigned int privilege_level;
    int (*callback) (ScriptEnvironment*, Script*, Object*);
    int type;
} ObjectMethod;

typedef struct{
    int length;
    char *string;
} StringStruct;

typedef struct{
    char *name;
    unsigned char type;
    char *typedef_name;
} StructElementInitializer;

typedef struct{
    char *name;
    int element_count;
    StructElementInitializer *elements;
} StructInitializer;

typedef struct{
    char *name;
    char *typedef_name;
    int class;
    int inherited_class;
    unsigned int size;
    unsigned char privilege_level;
    int method_count;
    ObjectMethod *methods;
    void *preset;
} ClassInformationInitializer;

typedef struct{
    char *name;
    void *data;
} BasicDataInitializer;

typedef struct{
    char *name;
} BoolElementInitializer;

typedef struct{
    char *name;
    int element_count;
    BoolElementInitializer *elements;
} BoolInitializer;

typedef struct{
    char *name;
    int (*callback) (ScriptEnvironment*, Script*);
} CFunctionInitializer;

typedef struct{
    unsigned int type;
    void *content;
} SymbolInitializer;