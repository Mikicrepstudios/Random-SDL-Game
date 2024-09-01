char CheckSetting(uint64 passed_setting) {
    return (megascript_bool_settings & passed_setting) != 0;
}

int WaitOSTime(double wait_time) {
#ifdef _WIN32
    Sleep(wait_time);
#elif linux
    usleep(wait_time * 1000.0f);
#endif
    return 0;
}

double GetCurrentOSTime() {
    double ret_val = 0.0f;
#ifdef _WIN32
    struct timeb start;
    ftime(&start);
    ret_val = (double)start.time + (double)start.millitm / 1000;
#elif linux
    struct timeval os_timeval_temp;
    gettimeofday(&os_timeval_temp, 0);
    ret_val = (double)os_timeval_temp.tv_usec / 1000000 + (double) os_timeval_temp.tv_sec;
#endif
    return ret_val;
}

int RemoveArrayMember(unsigned char **array, long long unsigned int *array_count, long long unsigned int array_index, long long unsigned int array_size) {
    if(*array_count == 1) {
        *array_count = 0;
        free(*array);
        *array = 0;
    } else if(*array_count > 0) {
        for(int i = array_index; i < *array_count - 1; i++) {
            memcpy(*array + (i * array_size), *array + ((i + 1) * array_size), array_size);
        }
        (*array_count)--;
        *array = realloc(*array, *array_count * array_size);
    } else {
        return 1;
    }
    return 0;
}

int InsertArrayMember(unsigned char **array, long long unsigned int *array_count, long long unsigned int array_index, long long unsigned int array_size, void *new_member) {
    if(array_index > *array_count) return 1;
    *array = realloc(*array, array_size * (*array_count + 1));
    if(array_index == *array_count) {
        memcpy(*array + *array_count, new_member, array_size);
        (*array_count)++;
        return 0;
    }
    for(long long unsigned int i = *array_count - 1; i >= array_index; i--) {
        memcpy(*array + ((i + 1) * array_size), *array + (i * array_size), array_size);
    }
    memcpy(*array + (array_index * array_size), new_member, array_size);
    (*array_count)++;
    return 0;
}