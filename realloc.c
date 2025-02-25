void *ft_realloc(void *ptr, size_t new_size)
{
    void    *result;
    size_t  oldsize;
    size_t  copysize;
    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }
    if (!ptr)
    {
        result = malloc(new_size);
        if (!result)
            return NULL;
        return result;
    }
    oldsize = ft_strlen((char *)ptr) + 1;
    if (oldsize < new_size)
        copysize = oldsize;
    else
        copysize = new_size;
    result = malloc(new_size);
    if (!result)
        return NULL; 
    ft_memcpy(result, ptr, copysize);
    free (ptr);
    return result;
}
