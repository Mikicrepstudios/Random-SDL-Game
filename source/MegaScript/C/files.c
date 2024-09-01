char *GetFileString(byte *file_name) {
    StringStruct ss = {0};
    FILE *file_ptr;
    file_ptr = fopen(file_name, "r");
    if(!file_ptr) return 0;
    int chr = 0;
    while(1) {
        chr = fgetc(file_ptr);
        if(chr == EOF) break;
        ss.string = realloc(ss.string, ss.length + 1);
        *(ss.string + ss.length) = chr;
        ss.length++;
    }
    ss.string = realloc(ss.string, ss.length + 1);
    *(ss.string + ss.length) = '\0';
    ss.length++;
    return ss.string;
}